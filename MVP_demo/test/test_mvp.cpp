#include <iostream>
#include <cassert>
#include <memory>
#include <thread>
#include <chrono>

#include "model/vehicle_model.hpp"
#include "view/view_interface.hpp"
#include "present/main_presenter.hpp"

using namespace duan;

// 模拟视图类用于测试
class MockDashboardView : public IDashboardView {
private:
    VehicleState last_state_;
    SensorData last_sensor_data_;
    std::string last_alert_;
    int last_priority_;
    
public:
    void displayVehicleSpeed(double speed) override {
        last_state_.speed = speed;
        std::cout << "[MockView] Speed: " << speed << " km/h" << std::endl;
    }
    
    void displaySteeringAngle(double angle) override {
        last_state_.steering_angle = angle;
    }
    
    void displayThrottlePosition(double position) override {
        last_state_.throttle_position = position;
    }
    
    void displayBrakePosition(double position) override {
        last_state_.brake_pressure = position;
    }
    
    void displayDriveMode(const std::string& mode) override {
        last_state_.drive_mode = mode;
        std::cout << "[MockView] Mode: " << mode << std::endl;
    }
    
    void displayAutopilotStatus(bool enabled) override {
        last_state_.autopilot_enabled = enabled;
        std::cout << "[MockView] Autopilot: " << (enabled ? "ON" : "OFF") << std::endl;
    }
    
    void displaySensorStatus(const SensorData& data) override {
        last_sensor_data_ = data;
    }
    
    void displaySystemAlert(const std::string& message, int priority) override {
        last_alert_ = message;
        last_priority_ = priority;
        std::cout << "[MockView] Alert[" << priority << "]: " << message << std::endl;
    }
    
    void displayBatteryLevel(double level) override {
        last_sensor_data_.battery_level = level;
    }
    
    void displaySystemWarnings(const std::vector<std::string>& warnings) override {
        for (const auto& warning : warnings) {
            std::cout << "[MockView] Warning: " << warning << std::endl;
        }
    }

    void displayWelcomeMessage() override {
        std::cout << "[MockView] =========================" << std::endl;
        std::cout << "[MockView] 欢迎使用MVP测试系统" << std::endl;
        std::cout << "[MockView] Mock Dashboard Welcome!" << std::endl;
        std::cout << "[MockView] =========================" << std::endl;
    }
    
    void displayHelpMenu() override {
        std::cout << "[MockView] === 帮助菜单 ===" << std::endl;
        std::cout << "[MockView] 这是Mock测试的帮助信息" << std::endl;
        std::cout << "[MockView] 可用的测试命令:" << std::endl;
        std::cout << "[MockView] - test: 运行测试" << std::endl;
        std::cout << "[MockView] - mock: 模拟操作" << std::endl;
        std::cout << "[MockView] ==================" << std::endl;
    }
    
    void clearScreen() override {}
    void refreshDisplay() override {}
    
    void setAutopilotToggleCallback(std::function<void(bool)> callback) override {}
    void setModeChangeCallback(std::function<void(const std::string&)> callback) override {}
    void setSpeedChangeCallback(std::function<void(double)> callback) override {}
    void setEmergencyStopCallback(std::function<void()> callback) override {}
    
    void simulateUserInput() override {}
    
    // 测试辅助方法
    const VehicleState& getLastState() const { return last_state_; }
    const SensorData& getLastSensorData() const { return last_sensor_data_; }
    const std::string& getLastAlert() const { return last_alert_; }
    int getLastPriority() const { return last_priority_; }
};

// 测试车辆模型
void testVehicleModel() {
    std::cout << "=== 测试车辆模型 ===" << std::endl;
    
    VehicleModel model;
    
    // 测试初始状态
    auto initial_state = model.getVehicleState();
    assert(initial_state.speed == 0.0);
    assert(initial_state.drive_mode == "Manual");
    assert(!initial_state.autopilot_enabled);
    
    // 测试速度设置
    model.setTargetSpeed(60.0);
    model.setThrottlePosition(50.0);
    
    // 测试模式切换
    model.setDriveMode("Autonomous");
    auto new_state = model.getVehicleState();
    assert(new_state.drive_mode == "Autonomous");
    assert(new_state.autopilot_enabled);
    
    // 测试传感器数据
    auto sensor_data = model.getSensorData();
    assert(sensor_data.camera_active);
    assert(sensor_data.radar_active);
    assert(!sensor_data.lidar_distances.empty());
    
    std::cout << "✅ 车辆模型测试通过" << std::endl;
}

// 测试MVP集成
void testMVPIntegration() {
    std::cout << "=== 测试MVP集成 ===" << std::endl;
    
    // 创建组件
    auto model = std::make_unique<VehicleModel>();
    auto mock_view = std::make_unique<MockDashboardView>();
    auto view_ptr = mock_view.get();  // 保存指针用于测试
    
    auto presenter = std::make_shared<MainPresenter>(
        std::move(model), std::move(mock_view));
    
    // 初始化和启动
    assert(presenter->initialize());
    assert(presenter->isInitialized());
    
    presenter->start();
    assert(presenter->isRunning());
    
    // 等待一些更新
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    // 停止系统
    presenter->stop();
    assert(!presenter->isRunning());
    
    std::cout << "✅ MVP集成测试通过" << std::endl;
}

// 测试安全功能
void testSafetyFeatures() {
    std::cout << "=== 测试安全功能 ===" << std::endl;
    
    VehicleModel model;
    
    // 测试正常安全检查
    assert(model.performSafetyCheck());
    
    // 模拟传感器故障
    SensorData faulty_data;
    faulty_data.camera_active = false;  // 摄像头离线
    faulty_data.radar_active = true;
    faulty_data.gps_accuracy = 2.0;
    faulty_data.battery_level = 80.0;
    
    model.updateSensorData(faulty_data);
    
    // 安全检查应该失败
    assert(!model.performSafetyCheck());
    
    // 检查系统警告
    auto warnings = model.getSystemWarnings();
    assert(!warnings.empty());
    
    bool camera_warning_found = false;
    for (const auto& warning : warnings) {
        if (warning.find("摄像头") != std::string::npos) {
            camera_warning_found = true;
            break;
        }
    }
    assert(camera_warning_found);
    
    std::cout << "✅ 安全功能测试通过" << std::endl;
}

// 测试数据观察者模式
void testObserverPattern() {
    std::cout << "=== 测试观察者模式 ===" << std::endl;
    
    class TestObserver : public VehicleDataObserver {
    public:
        bool vehicle_updated = false;
        bool sensor_updated = false;
        bool alert_received = false;
        
        void onVehicleStateChanged(const VehicleState&) override {
            vehicle_updated = true;
        }
        
        void onSensorDataChanged(const SensorData&) override {
            sensor_updated = true;
        }
        
        void onSystemAlert(const std::string&, int) override {
            alert_received = true;
        }
    };
    
    VehicleModel model;
    auto observer = std::make_shared<TestObserver>();
    
    model.addObserver(observer);
    
    // 触发更新
    model.setTargetSpeed(50.0);
    model.setDriveMode("Assisted");
    
    // 检查观察者是否收到通知
    assert(observer->vehicle_updated);
    
    std::cout << "✅ 观察者模式测试通过" << std::endl;
}

// 运行所有测试
int main() {
    std::cout << "🧪 开始MVP模式单元测试" << std::endl;
    std::cout << "=====================================" << std::endl;
    
    try {
        testVehicleModel();
        testSafetyFeatures();
        testObserverPattern();
        testMVPIntegration();
        
        std::cout << "=====================================" << std::endl;
        std::cout << "🎉 所有测试通过！" << std::endl;
        
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "❌ 测试失败: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "❌ 未知测试异常" << std::endl;
        return 1;
    }
}