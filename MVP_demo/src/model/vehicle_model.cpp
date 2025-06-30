#include "model/vehicle_model.hpp"
#include <random>
#include <algorithm>
#include <thread>
#include <iostream>

namespace duan {

VehicleState::VehicleState() 
    : speed(0.0), steering_angle(0.0), throttle_position(0.0), 
      brake_pressure(0.0), autopilot_enabled(false), drive_mode("Manual"),
      timestamp(std::chrono::system_clock::now()) {}

SensorData::SensorData() 
    : camera_active(true), radar_active(true), gps_accuracy(2.0), 
      system_status("OK"), battery_level(85.0),
      timestamp(std::chrono::system_clock::now()) {
    lidar_distances.resize(360, 50.0);  // 初始化360度激光雷达数据
}

VehicleModel::VehicleModel() 
    : simulation_running_(false), target_speed_(0.0), acceleration_rate_(2.0) {
    // 初始化传感器数据
    generateRandomSensorData();
}

VehicleModel::~VehicleModel() {
    stopSimulation();
}

VehicleState VehicleModel::getVehicleState() const {
    std::lock_guard<std::mutex> lock(data_mutex_);
    return current_state_;
}

SensorData VehicleModel::getSensorData() const {
    std::lock_guard<std::mutex> lock(data_mutex_);
    return sensor_data_;
}

void VehicleModel::setTargetSpeed(double speed) {
    std::lock_guard<std::mutex> lock(data_mutex_);
    target_speed_ = std::max(0.0, std::min(speed, 120.0));  // 限制在0-120km/h
}

void VehicleModel::setSteeringAngle(double angle) {
    std::lock_guard<std::mutex> lock(data_mutex_);
    current_state_.steering_angle = std::max(-45.0, std::min(angle, 45.0));
    current_state_.timestamp = std::chrono::system_clock::now();
    notifyVehicleStateChanged();
}

void VehicleModel::setThrottlePosition(double position) {
    std::lock_guard<std::mutex> lock(data_mutex_);
    current_state_.throttle_position = std::max(0.0, std::min(position, 100.0));
    current_state_.timestamp = std::chrono::system_clock::now();
    notifyVehicleStateChanged();
}

void VehicleModel::setBrakePosition(double position) {
    std::lock_guard<std::mutex> lock(data_mutex_);
    current_state_.brake_pressure = std::max(0.0, std::min(position, 100.0));
    current_state_.timestamp = std::chrono::system_clock::now();
    notifyVehicleStateChanged();
}

void VehicleModel::setAutopilotMode(bool enabled) {
    std::lock_guard<std::mutex> lock(data_mutex_);
    current_state_.autopilot_enabled = enabled;
    current_state_.timestamp = std::chrono::system_clock::now();
    
    if (enabled) {
        current_state_.drive_mode = "Autonomous";
        notifySystemAlert("自动驾驶模式已启用", 1);
    } else {
        current_state_.drive_mode = "Manual";
        notifySystemAlert("已切换到手动驾驶模式", 1);
    }
    
    notifyVehicleStateChanged();
}

void VehicleModel::setDriveMode(const std::string& mode) {
    std::lock_guard<std::mutex> lock(data_mutex_);
    
    if (mode == "Manual" || mode == "Assisted" || mode == "Autonomous") {
        current_state_.drive_mode = mode;
        current_state_.autopilot_enabled = (mode == "Autonomous");
        current_state_.timestamp = std::chrono::system_clock::now();
        
        notifySystemAlert("驾驶模式切换到: " + mode, 1);
        notifyVehicleStateChanged();
    }
}

void VehicleModel::updateSensorData(const SensorData& data) {
    std::lock_guard<std::mutex> lock(data_mutex_);
    sensor_data_ = data;
    sensor_data_.timestamp = std::chrono::system_clock::now();
    notifySensorDataChanged();
}

void VehicleModel::startSimulation() {
    simulation_running_ = true;
    std::cout << "[VehicleModel] 开始车辆仿真" << std::endl;
}

void VehicleModel::stopSimulation() {
    simulation_running_ = false;
    std::cout << "[VehicleModel] 停止车辆仿真" << std::endl;
}

void VehicleModel::simulateVehicleMovement() {
    if (!simulation_running_) return;
    
    updateSimulation();
    generateRandomSensorData();
}

void VehicleModel::addObserver(std::weak_ptr<VehicleDataObserver> observer) {
    observers_.push_back(observer);
}

void VehicleModel::removeObserver(std::weak_ptr<VehicleDataObserver> observer) {
    auto it = std::remove_if(observers_.begin(), observers_.end(),
        [&observer](const std::weak_ptr<VehicleDataObserver>& weak_obs) {
            return weak_obs.expired() || weak_obs.lock() == observer.lock();
        });
    observers_.erase(it, observers_.end());
}

bool VehicleModel::performSafetyCheck() const {
    std::lock_guard<std::mutex> lock(data_mutex_);
    
    // 检查传感器状态
    if (!sensor_data_.camera_active || !sensor_data_.radar_active) {
        return false;
    }
    
    // 检查GPS精度
    if (sensor_data_.gps_accuracy > 5.0) {
        return false;
    }
    
    // 检查电池电量
    if (sensor_data_.battery_level < 20.0) {
        return false;
    }
    
    // 检查速度是否异常
    if (current_state_.speed > 130.0) {
        return false;
    }
    
    return true;
}

std::vector<std::string> VehicleModel::getSystemWarnings() const {
    std::lock_guard<std::mutex> lock(data_mutex_);
    std::vector<std::string> warnings;
    
    if (!sensor_data_.camera_active) {
        warnings.push_back("摄像头离线");
    }
    
    if (!sensor_data_.radar_active) {
        warnings.push_back("雷达离线");
    }
    
    if (sensor_data_.gps_accuracy > 3.0) {
        warnings.push_back("GPS精度低");
    }
    
    if (sensor_data_.battery_level < 30.0) {
        warnings.push_back("电池电量低");
    }
    
    if (current_state_.speed > 100.0) {
        warnings.push_back("车速过高");
    }
    
    return warnings;
}

void VehicleModel::notifyVehicleStateChanged() {
    auto it = observers_.begin();
    while (it != observers_.end()) {
        /*
        std::weak_ptr是一种不拥有对象所有权的智能指针
        它可以避免循环引用的问题。当观察者被销毁时，weak_ptr会自动失效
        这里使用lock()方法获取一个shared_ptr，如果观察者已经被销毁，
        lock()会返回一个空的shared_ptr，这样就可以安全地跳过这个观察者
        以避免访问已销毁的对象导致的未定义行为

        lock 获取所有权
        */
        if (auto observer = it->lock()) {
            observer->onVehicleStateChanged(current_state_);
            ++it;
        } else {
            it = observers_.erase(it);
        }
    }
}

void VehicleModel::notifySensorDataChanged() {
    auto it = observers_.begin();
    while (it != observers_.end()) {
        if (auto observer = it->lock()) {
            observer->onSensorDataChanged(sensor_data_);
            ++it;
        } else {
            it = observers_.erase(it);
        }
    }
}

void VehicleModel::notifySystemAlert(const std::string& message, int priority) {
    auto it = observers_.begin();
    while (it != observers_.end()) {
        if (auto observer = it->lock()) {
            observer->onSystemAlert(message, priority);
            ++it;
        } else {
            it = observers_.erase(it);
        }
    }
}

void VehicleModel::updateSimulation() {
    std::lock_guard<std::mutex> lock(data_mutex_);
    
    // 更新速度
    if (current_state_.brake_pressure > 0) {
        current_state_.speed = std::max(0.0, current_state_.speed - acceleration_rate_ * 2);
    } else if (current_state_.throttle_position > 0) {
        double target = target_speed_ * current_state_.throttle_position / 100.0;
        if (current_state_.speed < target) {
            current_state_.speed = std::min(target, current_state_.speed + acceleration_rate_);
        } else {
            current_state_.speed = std::max(target, current_state_.speed - acceleration_rate_ * 0.5);
        }
    } else {
        // 自然减速
        current_state_.speed = std::max(0.0, current_state_.speed - 0.5);
    }
    
    current_state_.timestamp = std::chrono::system_clock::now();
}

void VehicleModel::generateRandomSensorData() {
    std::lock_guard<std::mutex> lock(data_mutex_);
    
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(0.0, 1.0);
    static std::uniform_real_distribution<> distance_dis(1.0, 100.0);
    
    // 随机更新激光雷达数据
    for (size_t i = 0; i < sensor_data_.lidar_distances.size(); ++i) {
        if (dis(gen) < 0.1) {  // 10%概率更新
            sensor_data_.lidar_distances[i] = distance_dis(gen);
        }
    }
    
    // 随机传感器故障模拟
    if (dis(gen) < 0.01) {  // 1%概率
        sensor_data_.camera_active = !sensor_data_.camera_active;
    }
    
    if (dis(gen) < 0.005) {  // 0.5%概率
        sensor_data_.radar_active = !sensor_data_.radar_active;
    }
    
    // GPS精度变化
    if (dis(gen) < 0.05) {  // 5%概率
        sensor_data_.gps_accuracy = 0.5 + dis(gen) * 5.0;
    }
    
    // 电池电量下降
    if (dis(gen) < 0.02) {  // 2%概率
        sensor_data_.battery_level = std::max(0.0, sensor_data_.battery_level - dis(gen));
    }
    
    sensor_data_.timestamp = std::chrono::system_clock::now();
}

} // duan