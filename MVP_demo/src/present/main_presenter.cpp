#include "present/main_presenter.hpp"
#include <iostream>
#include <chrono>

namespace duan {

MainPresenter::MainPresenter(std::unique_ptr<VehicleModel> model, 
                            std::unique_ptr<IDashboardView> view)
    : model_(std::move(model)), view_(std::move(view)), 
      running_(false), update_interval_(std::chrono::milliseconds(100)),
      system_initialized_(false), emergency_mode_(false), 
      safety_check_failures_(0) {
    
    std::cout << "[MainPresenter] 创建主控制器" << std::endl;
}

MainPresenter::~MainPresenter() {
    stop();
    std::cout << "[MainPresenter] 销毁主控制器" << std::endl;
}

bool MainPresenter::initialize() {
    std::lock_guard<std::mutex> lock(presenter_mutex_);
    
    std::cout << "[MainPresenter] 初始化系统..." << std::endl;
    
    try {
        // 设置观察者关系
        auto shared_this = shared_from_this();
        model_->addObserver(shared_this);
        
        // 设置视图回调
        setupViewCallbacks();
        
        // 显示欢迎消息
        view_->displayWelcomeMessage();
        
        // 启动模拟
        model_->startSimulation();
        
        system_initialized_ = true;
        std::cout << "[MainPresenter] 系统初始化完成" << std::endl;
        
        return true;
    }
    catch (const std::exception& e) {
        std::cout << "[MainPresenter] 初始化失败: " << e.what() << std::endl;
        return false;
    }
}

void MainPresenter::start() {
    if (!system_initialized_) {
        std::cout << "[MainPresenter] 系统未初始化，无法启动" << std::endl;
        return;
    }
    
    std::lock_guard<std::mutex> lock(presenter_mutex_);
    
    if (running_) {
        std::cout << "[MainPresenter] 系统已在运行中" << std::endl;
        return;
    }
    
    running_ = true;
    last_update_ = std::chrono::system_clock::now();
    
    // 启动更新线程
    update_thread_ = std::thread(&MainPresenter::updateLoop, this);
    
    // 启动用户输入
    view_->simulateUserInput();
    
    std::cout << "[MainPresenter] 系统启动完成" << std::endl;
}

void MainPresenter::stop() {
    std::lock_guard<std::mutex> lock(presenter_mutex_);
    
    if (!running_) return;
    
    running_ = false;
    
    if (update_thread_.joinable()) {
        update_thread_.join();
    }
    
    if (model_) {
        model_->stopSimulation();
    }
    
    std::cout << "[MainPresenter] 系统已停止" << std::endl;
}

void MainPresenter::restart() {
    std::cout << "[MainPresenter] 重启系统..." << std::endl;
    stop();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    emergency_mode_ = false;
    safety_check_failures_ = 0;
    
    if (initialize()) {
        start();
    }
}

void MainPresenter::onVehicleStateChanged(const VehicleState& state) {
    // 在updateView中统一更新，避免过于频繁的更新
}

void MainPresenter::onSensorDataChanged(const SensorData& data) {
    // 检查传感器异常
    if (!data.camera_active || !data.radar_active) {
        handleSystemFailure("关键传感器离线");
    }
    
    if (data.gps_accuracy > 10.0) {
        view_->displaySystemAlert("GPS信号弱，定位精度降低", 1);
    }
    
    if (data.battery_level < 15.0) {
        view_->displaySystemAlert("电池电量严重不足", 2);
    }
}

void MainPresenter::onSystemAlert(const std::string& message, int priority) {
    view_->displaySystemAlert(message, priority);
    
    if (priority >= 3) {  // 紧急情况
        handleEmergencyStop();
    }
}

bool MainPresenter::isRunning() const {
    return running_;
}

bool MainPresenter::isInitialized() const {
    return system_initialized_;
}

bool MainPresenter::isEmergencyMode() const {
    return emergency_mode_;
}

void MainPresenter::updateLoop() {
    while (running_) {
        try {
            // 更新模型
            model_->simulateVehicleMovement();
            
            // 处理系统逻辑
            processSystemLogic();
            
            // 更新视图
            if (shouldUpdateView()) {
                updateView();
                last_update_ = std::chrono::system_clock::now();
            }
            
            // 网志系统状态
            logSystemStatus();
            
            // 等待下一次更新
            std::this_thread::sleep_for(update_interval_);
        }
        catch (const std::exception& e) {
            std::cout << "[MainPresenter] 更新循环异常: " << e.what() << std::endl;
            handleSystemFailure("系统更新异常");
        }
    }
}

void MainPresenter::updateView() {
    if (!view_) return;
    
    auto vehicle_state = model_->getVehicleState();
    auto sensor_data = model_->getSensorData();
    
    // 定期刷新显示
    static int refresh_counter = 0;
    if (++refresh_counter % 50 == 0) {  // 每5秒刷新一次界面
        view_->refreshDisplay();
    }
    
    // 更新车辆状态
    view_->displayVehicleSpeed(vehicle_state.speed);
    view_->displaySteeringAngle(vehicle_state.steering_angle);
    view_->displayThrottlePosition(vehicle_state.throttle_position);
    view_->displayBrakePosition(vehicle_state.brake_pressure);
    view_->displayDriveMode(vehicle_state.drive_mode);
    view_->displayAutopilotStatus(vehicle_state.autopilot_enabled);
    
    // 更新传感器状态
    view_->displaySensorStatus(sensor_data);
    view_->displayBatteryLevel(sensor_data.battery_level);
    
    // 显示系统警告
    auto warnings = model_->getSystemWarnings();
    view_->displaySystemWarnings(warnings);
}

void MainPresenter::processSystemLogic() {
    // 周期性安全检查
    static int safety_check_counter = 0;
    if (++safety_check_counter % 20 == 0) {  // 每2秒检查一次
        performPeriodicSafetyCheck();
    }
    
    // 紧急模式处理
    if (emergency_mode_) {
        auto vehicle_state = model_->getVehicleState();
        if (vehicle_state.speed < 1.0) {  // 车辆已停止
            emergency_mode_ = false;
            view_->displaySystemAlert("紧急停车完成，系统恢复正常", 1);
        }
    }
}

void MainPresenter::performPeriodicSafetyCheck() {
    if (!checkSystemSafety()) {
        safety_check_failures_++;
        
        if (safety_check_failures_ >= MAX_SAFETY_FAILURES) {
            handleSystemFailure("连续安全检查失败");
        } else {
            view_->displaySystemAlert("安全检查失败 (" + 
                std::to_string(safety_check_failures_) + "/" + 
                std::to_string(MAX_SAFETY_FAILURES) + ")", 1);
        }
    } else {
        resetSafetyFailures();
    }
}

void MainPresenter::onAutopilotToggle(bool enabled) {
    std::cout << "[MainPresenter] 自动驾驶切换请求: " 
              << (enabled ? "启用" : "禁用") << std::endl;
    
    if (enabled) {
        if (canSwitchToAutonomous()) {
            model_->setAutopilotMode(true);
            view_->displaySystemAlert("自动驾驶已启用", 1);
        } else {
            view_->displaySystemAlert("系统安全检查失败，无法启用自动驾驶", 2);
        }
    } else {
        model_->setAutopilotMode(false);
        view_->displaySystemAlert("自动驾驶已禁用", 1);
    }
}

void MainPresenter::onModeChange(const std::string& mode) {
    std::cout << "[MainPresenter] 驾驶模式切换请求: " << mode << std::endl;
    
    if (mode == "autonomous") {
        if (canSwitchToAutonomous()) {
            model_->setDriveMode("Autonomous");
        } else {
            view_->displaySystemAlert("无法切换到自动驾驶模式", 2);
        }
    } else if (mode == "assisted") {
        if (canSwitchToAssisted()) {
            model_->setDriveMode("Assisted");
        } else {
            view_->displaySystemAlert("无法切换到辅助驾驶模式", 2);
        }
    } else if (mode == "manual") {
        model_->setDriveMode("Manual");
    }
}

void MainPresenter::onSpeedChange(double speed) {
    std::cout << "[MainPresenter] 目标速度设置: " << speed << " km/h" << std::endl;
    
    if (speed < 0 || speed > 120) {
        view_->displaySystemAlert("速度设置超出valid范围 (0-120 km/h)", 1);
        return;
    }
    
    model_->setTargetSpeed(speed);
    
    if (speed > 0) {
        model_->setThrottlePosition(std::min(100.0, speed * 0.8));
        model_->setBrakePosition(0.0);
    } else {
        model_->setThrottlePosition(0.0);
        model_->setBrakePosition(50.0);
    }
    
    view_->displaySystemAlert("目标速度已设置为 " + std::to_string(static_cast<int>(speed)) + " km/h", 1);
}

void MainPresenter::onEmergencyStop() {
    std::cout << "[MainPresenter] 收到紧急停车指令" << std::endl;
    handleEmergencyStop();
}

bool MainPresenter::checkSystemSafety() {
    return model_->performSafetyCheck();
}

void MainPresenter::handleEmergencyStop() {
    std::cout << "[MainPresenter] 执行紧急停车程序" << std::endl;
    
    emergency_mode_ = true;
    
    // 立即切换到手动模式
    model_->setAutopilotMode(false);
    model_->setDriveMode("Manual");
    
    // 强制刹车
    model_->setThrottlePosition(0.0);
    model_->setBrakePosition(100.0);
    model_->setTargetSpeed(0.0);
    
    view_->displaySystemAlert("紧急停车已激活！", 3);
}

void MainPresenter::handleSystemFailure(const std::string& reason) {
    std::cout << "[MainPresenter] 系统故障: " << reason << std::endl;
    
    // 自动切换到安全模式
    switchToManualMode("系统故障: " + reason);
    
    view_->displaySystemAlert("检测到系统故障，已切换到手动模式", 2);
}

void MainPresenter::resetSafetyFailures() {
    if (safety_check_failures_ > 0) {
        safety_check_failures_ = 0;
    }
}

bool MainPresenter::canSwitchToAutonomous() {
    if (emergency_mode_) return false;
    
    auto sensor_data = model_->getSensorData();
    
    // 检查关键传感器
    if (!sensor_data.camera_active || !sensor_data.radar_active) {
        return false;
    }
    
    // 检查GPS精度
    if (sensor_data.gps_accuracy > 3.0) {
        return false;
    }
    
    // 检查电池电量
    if (sensor_data.battery_level < 30.0) {
        return false;
    }
    
    return checkSystemSafety();
}

bool MainPresenter::canSwitchToAssisted() {
    if (emergency_mode_) return false;
    
    auto sensor_data = model_->getSensorData();
    
    // 辅助模式要求较低
    if (sensor_data.battery_level < 15.0) {
        return false;
    }
    
    return true;
}

void MainPresenter::switchToManualMode(const std::string& reason) {
    model_->setAutopilotMode(false);
    model_->setDriveMode("Manual");
    view_->displaySystemAlert("已切换到手动模式: " + reason, 1);
}

void MainPresenter::setupViewCallbacks() {
    view_->setAutopilotToggleCallback(
        [this](bool enabled) { onAutopilotToggle(enabled); });
    
    view_->setModeChangeCallback(
        [this](const std::string& mode) { onModeChange(mode); });
    
    view_->setSpeedChangeCallback(
        [this](double speed) { onSpeedChange(speed); });
    
    view_->setEmergencyStopCallback(
        [this]() { onEmergencyStop(); });
}

void MainPresenter::logSystemStatus() {
    static int log_counter = 0;
    if (++log_counter % 100 == 0) {  // 每10秒记录一次
        auto vehicle_state = model_->getVehicleState();
        std::cout << "[MainPresenter] 系统状态 - 速度: " << vehicle_state.speed 
                  << " km/h, 模式: " << vehicle_state.drive_mode 
                  << ", 安全检查失败次数: " << safety_check_failures_ << std::endl;
    }
}

bool MainPresenter::shouldUpdateView() {
    auto now = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        now - last_update_);
    
    return elapsed >= update_interval_;
}

} // namespace autonomous_driving