#ifndef VEHICLE_MODEL_H
#define VEHICLE_MODEL_H

#include <string>
#include <vector>
#include <chrono>
#include <memory>
#include <functional>
#include <mutex>

namespace duan {

/**
 * @brief 车辆状态数据结构
 */
struct VehicleState {
    double speed;                    // 速度 (km/h)
    double steering_angle;           // 方向盘角度 (degrees)
    double throttle_position;        // 油门位置 (0-100%)
    double brake_pressure;           // 制动压力 (0-100%)
    bool autopilot_enabled;          // 自动驾驶是否启用
    std::string drive_mode;          // 驾驶模式: "Manual", "Assisted", "Autonomous"
    std::chrono::system_clock::time_point timestamp;
    
    VehicleState();
};

/**
 * @brief 传感器数据结构
 */
struct SensorData {
    std::vector<double> lidar_distances;  // 激光雷达距离数据
    bool camera_active;                   // 摄像头是否激活
    bool radar_active;                    // 雷达是否激活
    double gps_accuracy;                  // GPS精度 (米)
    std::string system_status;            // 系统状态
    double battery_level;                 // 电池电量 (%)
    std::chrono::system_clock::time_point timestamp;
    
    SensorData();
};

/**
 * @brief 车辆数据观察者接口
 */
class VehicleDataObserver {
public:
    virtual ~VehicleDataObserver() = default;
    virtual void onVehicleStateChanged(const VehicleState& state) = 0;
    virtual void onSensorDataChanged(const SensorData& data) = 0;
    virtual void onSystemAlert(const std::string& message, int priority) = 0;
};

/**
 * @brief 车辆模型类 - MVP中的Model
 */
class VehicleModel {
private:
    VehicleState current_state_;
    SensorData sensor_data_;
    
    std::vector<std::weak_ptr<VehicleDataObserver>> observers_;
    mutable std::mutex data_mutex_;
    
    // 模拟参数
    bool simulation_running_;
    double target_speed_;
    double acceleration_rate_;

public:
    VehicleModel();
    ~VehicleModel();
    
    // 数据获取接口
    VehicleState getVehicleState() const;
    SensorData getSensorData() const;
    
    // 车辆控制接口
    void setTargetSpeed(double speed);
    void setSteeringAngle(double angle);
    void setThrottlePosition(double position);
    void setBrakePosition(double position);
    void setAutopilotMode(bool enabled);
    void setDriveMode(const std::string& mode);
    
    // 传感器数据更新
    void updateSensorData(const SensorData& data);
    
    // 模拟功能
    void startSimulation();
    void stopSimulation();
    void simulateVehicleMovement();
    
    // 观察者模式
    void addObserver(std::weak_ptr<VehicleDataObserver> observer);
    void removeObserver(std::weak_ptr<VehicleDataObserver> observer);
    
    // 安全检查
    bool performSafetyCheck() const;
    std::vector<std::string> getSystemWarnings() const;

private:
    void notifyVehicleStateChanged();
    void notifySensorDataChanged();
    void notifySystemAlert(const std::string& message, int priority);
    void updateSimulation();
    void generateRandomSensorData();
};

}

#endif