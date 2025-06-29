#ifndef DASHBOARD_VIEW_H
#define DASHBOARD_VIEW_H

#include "view/view_interface.hpp"
#include <functional>
#include <iostream>
#include <iomanip>
#include <thread>
#include <atomic>
#include <mutex>

namespace duan {

/**
 * @brief 控制台仪表盘视图实现 - MVP中的View具体实现
 */
class ConsoleDashboardView : public IDashboardView {
private:
    // 回调函数
    std::function<void(bool)> autopilot_callback_;
    std::function<void(const std::string&)> mode_callback_;
    std::function<void(double)> speed_callback_;
    std::function<void()> emergency_stop_callback_;
    
    // 用户输入模拟
    std::thread input_thread_;
    std::atomic<bool> input_running_;
    mutable std::mutex display_mutex_;
    
    // 显示状态
    bool display_detailed_;
    int alert_priority_;
    std::string current_alert_;

public:
    ConsoleDashboardView();
    ~ConsoleDashboardView();
    
    // 实现显示接口
    void displayVehicleSpeed(double speed) override;
    void displaySteeringAngle(double angle) override;
    void displayThrottlePosition(double position) override;
    void displayBrakePosition(double position) override;
    void displayDriveMode(const std::string& mode) override;
    void displayAutopilotStatus(bool enabled) override;
    void displaySensorStatus(const SensorData& data) override;
    void displaySystemAlert(const std::string& message, int priority) override;
    void displayBatteryLevel(double level) override;
    void displaySystemWarnings(const std::vector<std::string>& warnings) override;
    
    // 实现界面控制
    void clearScreen() override;
    void refreshDisplay() override;
    
    // 实现交互回调设置
    void setAutopilotToggleCallback(std::function<void(bool)> callback) override;
    void setModeChangeCallback(std::function<void(const std::string&)> callback) override;
    void setSpeedChangeCallback(std::function<void(double)> callback) override;
    void setEmergencyStopCallback(std::function<void()> callback) override;
    
    // 实现用户输入
    void simulateUserInput() override;
    
    // 辅助功能
    void setDetailedDisplay(bool detailed);
    void displayWelcomeMessage();
    void displayHelpMenu();

private:
    void printSeparator(char c = '-', int length = 60);
    void printCentered(const std::string& text, int width = 60);
    void inputLoop();
    void processUserCommand(const std::string& command);
    std::string getColorCode(int priority);
    void resetColor();
};

} // namespace duan

#endif // DASHBOARD_VIEW_H