#ifndef VIEW_INTERFACE_H
#define VIEW_INTERFACE_H

#include "model/vehicle_model.hpp"
#include <functional>
#include <string>

namespace duan {

/**
 * @brief 仪表盘视图接口 - MVP中的View接口
 */
class IDashboardView {
public:
    virtual ~IDashboardView() = default;
    
    // 显示接口
    virtual void displayVehicleSpeed(double speed) = 0;
    virtual void displaySteeringAngle(double angle) = 0;
    virtual void displayThrottlePosition(double position) = 0;
    virtual void displayBrakePosition(double position) = 0;
    virtual void displayDriveMode(const std::string& mode) = 0;
    virtual void displayAutopilotStatus(bool enabled) = 0;
    virtual void displaySensorStatus(const SensorData& data) = 0;
    virtual void displaySystemAlert(const std::string& message, int priority) = 0;
    virtual void displayBatteryLevel(double level) = 0;
    virtual void displaySystemWarnings(const std::vector<std::string>& warnings) = 0;
    virtual void displayWelcomeMessage() = 0;
    virtual void displayHelpMenu() = 0;
    
    // 清屏和刷新
    virtual void clearScreen() = 0;
    virtual void refreshDisplay() = 0;
    
    // 用户交互回调设置
    virtual void setAutopilotToggleCallback(std::function<void(bool)> callback) = 0;
    virtual void setModeChangeCallback(std::function<void(const std::string&)> callback) = 0;
    virtual void setSpeedChangeCallback(std::function<void(double)> callback) = 0;
    virtual void setEmergencyStopCallback(std::function<void()> callback) = 0;
    
    // 用户输入模拟
    virtual void simulateUserInput() = 0;
};

}

#endif