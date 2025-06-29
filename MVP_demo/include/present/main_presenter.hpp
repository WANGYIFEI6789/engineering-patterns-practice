#ifndef MAIN_PRESENTER_H
#define MAIN_PRESENTER_H

#include "model/vehicle_model.hpp"
#include "view/view_interface.hpp"
#include <memory>
#include <thread>
#include <atomic>
#include <chrono>
#include <mutex>

namespace duan {

/**
 * @brief 主控制器 - MVP中的Presenter
 */
class MainPresenter : public VehicleDataObserver, 
                      public std::enable_shared_from_this<MainPresenter> {
private:
    std::unique_ptr<VehicleModel> model_;
    std::unique_ptr<IDashboardView> view_;
    
    // 更新线程
    std::thread update_thread_;
    std::atomic<bool> running_;
    std::mutex presenter_mutex_;
    
    // 更新频率控制
    std::chrono::milliseconds update_interval_;
    std::chrono::system_clock::time_point last_update_;
    
    // 系统状态
    bool system_initialized_;
    bool emergency_mode_;
    int safety_check_failures_;
    static constexpr int MAX_SAFETY_FAILURES = 3;

public:
    MainPresenter(std::unique_ptr<VehicleModel> model, 
                  std::unique_ptr<IDashboardView> view);
    ~MainPresenter();
    
    // 系统控制
    bool initialize();
    void start();
    void stop();
    void restart();
    
    // VehicleDataObserver接口实现
    void onVehicleStateChanged(const VehicleState& state) override;
    void onSensorDataChanged(const SensorData& data) override;
    void onSystemAlert(const std::string& message, int priority) override;
    
    // 状态查询
    bool isRunning() const;
    bool isInitialized() const;
    bool isEmergencyMode() const;

private:
    // 核心业务逻辑
    void updateLoop();
    void updateView();
    void processSystemLogic();
    void performPeriodicSafetyCheck();
    
    // 用户事件处理
    void onAutopilotToggle(bool enabled);
    void onModeChange(const std::string& mode);
    void onSpeedChange(double speed);
    void onEmergencyStop();
    
    // 安全和错误处理
    bool checkSystemSafety();
    void handleEmergencyStop();
    void handleSystemFailure(const std::string& reason);
    void resetSafetyFailures();
    
    // 模式切换逻辑
    bool canSwitchToAutonomous();
    bool canSwitchToAssisted();
    void switchToManualMode(const std::string& reason);
    
    // 辅助功能
    void setupViewCallbacks();
    void logSystemStatus();
    bool shouldUpdateView();
};

} // namespace duan

#endif // MAIN_PRESENTER_H