#include "view/dashboard_view.hpp"
#include <iostream>
#include <sstream>   // 字符串流，用于处理用户输入
#include <chrono>    
#include <random>

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
#else
    #include <termios.h>    // 用于终端输入处理
    #include <unistd.h>     
#endif

namespace duan {

ConsoleDashboardView::ConsoleDashboardView() 
    : input_running_(false), display_detailed_(true), alert_priority_(0) {
    std::cout << "[DashboardView] 控制台仪表盘初始化" << std::endl;
}

ConsoleDashboardView::~ConsoleDashboardView() {
    if (input_running_) {
        input_running_ = false;
        if (input_thread_.joinable()) {
            input_thread_.join();
        }
    }
    std::cout << "[DashboardView] 控制台仪表盘销毁" << std::endl;
}

void ConsoleDashboardView::displayVehicleSpeed(double speed) {
    std::lock_guard<std::mutex> lock(display_mutex_);
    if (display_detailed_) {
        std::cout << "🚗 速度: " << std::fixed << std::setprecision(1) 
                  << speed << " km/h";
        
        if (speed > 100) {
            std::cout << " [高速]";
        } else if (speed > 60) {
            std::cout << " [中速]";
        } else if (speed > 0) {
            std::cout << " [低速]";
        } else {
            std::cout << " [停止]";
        }
        std::cout << std::endl;
    }
}

void ConsoleDashboardView::displaySteeringAngle(double angle) {
    std::lock_guard<std::mutex> lock(display_mutex_);
    if (display_detailed_) {
        std::cout << "方向盘: " << std::fixed << std::setprecision(1) 
                  << angle << "°";
        
        if (std::abs(angle) > 30) {
            std::cout << " [大转角]";
        } else if (std::abs(angle) > 10) {
            std::cout << " [转弯中]";
        } else {
            std::cout << " [直行]";
        }
        std::cout << std::endl;
    }
}

void ConsoleDashboardView::displayThrottlePosition(double position) {
    std::lock_guard<std::mutex> lock(display_mutex_);
    if (display_detailed_) {
        std::cout << "⚡ 油门: " << std::fixed << std::setprecision(1) 
                  << position << "%";
        
        if (position > 80) {
            std::cout << " [全力加速]";
        } else if (position > 50) {
            std::cout << " [加速中]";
        } else if (position > 0) {
            std::cout << " [轻加速]";
        } else {
            std::cout << " [无输入]";
        }
        std::cout << std::endl;
    }
}

void ConsoleDashboardView::displayBrakePosition(double position) {
    std::lock_guard<std::mutex> lock(display_mutex_);
    if (display_detailed_) {
        std::cout << "🛑 制动: " << std::fixed << std::setprecision(1) 
                  << position << "%";
        
        if (position > 70) {
            std::cout << " [紧急制动]";
        } else if (position > 30) {
            std::cout << " [制动中]";
        } else if (position > 0) {
            std::cout << " [轻制动]";
        } else {
            std::cout << " [无制动]";
        }
        std::cout << std::endl;
    }
}

void ConsoleDashboardView::displayDriveMode(const std::string& mode) {
    std::lock_guard<std::mutex> lock(display_mutex_);
    std::cout << "🚦 驾驶模式: " << getColorCode(1) << mode;
    resetColor();
    
    if (mode == "Autonomous") {
        std::cout << " [完全自动]";
    } else if (mode == "Assisted") {
        std::cout << " [辅助驾驶]";
    } else {
        std::cout << " [手动驾驶]";
    }
    std::cout << std::endl;
}

void ConsoleDashboardView::displayAutopilotStatus(bool enabled) {
    std::lock_guard<std::mutex> lock(display_mutex_);
    std::cout << "🤖 自动驾驶: ";
    
    if (enabled) {
        std::cout << getColorCode(2) << "启用" << getColorCode(0);
    } else {
        std::cout << getColorCode(3) << "禁用" << getColorCode(0);
    }
    std::cout << std::endl;
}

void ConsoleDashboardView::displaySensorStatus(const SensorData& data) {
    std::lock_guard<std::mutex> lock(display_mutex_);
    if (display_detailed_) {
        std::cout << "📡 传感器状态:" << std::endl;
        std::cout << "  📷 摄像头: " << (data.camera_active ? "✅ 正常" : "❌ 离线") << std::endl;
        std::cout << "  📶 雷达: " << (data.radar_active ? "✅ 正常" : "❌ 离线") << std::endl;
        std::cout << "  🛰️ GPS精度: " << std::fixed << std::setprecision(1) 
                  << data.gps_accuracy << "m" << std::endl;
        std::cout << "  🔋 电池: " << std::fixed << std::setprecision(0) 
                  << data.battery_level << "%" << std::endl;
        
        // 显示部分激光雷达数据
        if (!data.lidar_distances.empty()) {
            std::cout << " 前方距离: ";
            for (size_t i = 170; i <= 190; i += 5) {  // 显示前方40度范围
                if (i < data.lidar_distances.size()) {
                    std::cout << std::fixed << std::setprecision(1) 
                              << data.lidar_distances[i] << "m ";
                }
            }
            std::cout << std::endl;
        }
    }
}

void ConsoleDashboardView::displaySystemAlert(const std::string& message, int priority) {
    std::lock_guard<std::mutex> lock(display_mutex_);
    current_alert_ = message;
    alert_priority_ = priority;
    
    std::cout << getColorCode(priority);
    
    switch (priority) {
        case 0: std::cout << "ℹ️  信息: "; break;
        case 1: std::cout << "⚠️  警告: "; break;
        case 2: std::cout << "🚨 错误: "; break;
        case 3: std::cout << "💥 紧急: "; break;
        default: std::cout << "📢 系统: "; break;
    }
    
    std::cout << message;
    resetColor();
    std::cout << std::endl;
}

void ConsoleDashboardView::displayBatteryLevel(double level) {
    std::lock_guard<std::mutex> lock(display_mutex_);
    if (display_detailed_) {
        std::cout << "🔋 电池电量: " << std::fixed << std::setprecision(1) << level << "%";
        
        if (level < 20) {
            std::cout << getColorCode(2) << " [电量不足]" << getColorCode(0);
        } else if (level < 50) {
            std::cout << getColorCode(1) << " [电量较低]" << getColorCode(0);
        } else {
            std::cout << " [电量充足]";
        }
        std::cout << std::endl;
    }
}

void ConsoleDashboardView::displaySystemWarnings(const std::vector<std::string>& warnings) {
    std::lock_guard<std::mutex> lock(display_mutex_);
    if (!warnings.empty()) {
        std::cout << getColorCode(1) << "⚠️  系统警告:" << getColorCode(0) << std::endl;
        for (size_t i = 0; i < warnings.size(); ++i) {
            std::cout << "  " << (i + 1) << ". " << warnings[i] << std::endl;
        }
    }
}

void ConsoleDashboardView::clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void ConsoleDashboardView::refreshDisplay() {
    std::lock_guard<std::mutex> lock(display_mutex_);
    printSeparator('=');
    printCentered("🚗 自动驾驶车辆仪表盘 🚗");
    printSeparator('=');
    
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::cout << "⏰ 当前时间: " << std::ctime(&time_t);
    
    if (!current_alert_.empty()) {
        std::cout << "📢 最新提示: " << getColorCode(alert_priority_) 
                  << current_alert_ << getColorCode(0) << std::endl;
    }
    
    printSeparator('-');
}

void ConsoleDashboardView::setAutopilotToggleCallback(std::function<void(bool)> callback) {
    autopilot_callback_ = callback;
}

void ConsoleDashboardView::setModeChangeCallback(std::function<void(const std::string&)> callback) {
    mode_callback_ = callback;
}

void ConsoleDashboardView::setSpeedChangeCallback(std::function<void(double)> callback) {
    speed_callback_ = callback;
}

void ConsoleDashboardView::setEmergencyStopCallback(std::function<void()> callback) {
    emergency_stop_callback_ = callback;
}

void ConsoleDashboardView::simulateUserInput() {
    if (!input_running_) {
        input_running_ = true;
        input_thread_ = std::thread(&ConsoleDashboardView::inputLoop, this);
    }
}

void ConsoleDashboardView::setDetailedDisplay(bool detailed) {
    display_detailed_ = detailed;
}

void ConsoleDashboardView::displayWelcomeMessage() {
    clearScreen();
    printSeparator('*');
    printCentered("欢迎使用自动驾驶MVP演示系统");
    printSeparator('*');
    std::cout << "\n🚗 系统功能:" << std::endl;
    std::cout << "  • 车辆状态实时监控" << std::endl;
    std::cout << "  • 传感器数据可视化" << std::endl;
    std::cout << "  • 驾驶模式切换" << std::endl;
    std::cout << "  • 安全系统监控" << std::endl;
    std::cout << "\n💡 提示: 输入 'help' 查看可用命令\n" << std::endl;
    printSeparator('-');
}

void ConsoleDashboardView::displayHelpMenu() {
    std::cout << "\n📋 可用命令:" << std::endl;
    std::cout << "  auto [on/off]  - 切换自动驾驶模式" << std::endl;
    std::cout << "  mode [manual/assisted/autonomous] - 切换驾驶模式" << std::endl;
    std::cout << "  speed [0-120]  - 设置目标速度" << std::endl;
    std::cout << "  stop          - 紧急停车" << std::endl;
    std::cout << "  detail [on/off] - 切换详细显示" << std::endl;
    std::cout << "  clear         - 清屏" << std::endl;
    std::cout << "  help          - 显示帮助" << std::endl;
    std::cout << "  quit          - 退出程序" << std::endl;
    std::cout << std::endl;
}

void ConsoleDashboardView::printSeparator(char c, int length) {
    std::cout << std::string(length, c) << std::endl;
}

void ConsoleDashboardView::printCentered(const std::string& text, int width) {
    int padding = (width - static_cast<int>(text.length())) / 2;
    std::cout << std::string(std::max(0, padding), ' ') << text << std::endl;
}

void ConsoleDashboardView::inputLoop() {
    std::string command;
    while (input_running_) {
        std::cout << "🎮 命令> ";
        std::getline(std::cin, command);
        
        if (!input_running_) break;
        
        if (command == "quit" || command == "exit") {
            input_running_ = false;
            if (emergency_stop_callback_) {
                emergency_stop_callback_();
            }
            break;
        }
        
        processUserCommand(command);
    }
}

void ConsoleDashboardView::processUserCommand(const std::string& command) {
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;
    
    if (cmd == "auto") {
        std::string state;
        iss >> state;
        if (autopilot_callback_) {
            if (state == "on" || state == "true" || state == "1") {
                autopilot_callback_(true);
            } else if (state == "off" || state == "false" || state == "0") {
                autopilot_callback_(false);
            }
        }
    }
    else if (cmd == "mode") {
        std::string mode;
        iss >> mode;
        if (mode_callback_) {
            if (mode == "manual" || mode == "assisted" || mode == "autonomous") {
                mode_callback_(mode);
            }
        }
    }
    else if (cmd == "speed") {
        double speed;
        iss >> speed;
        if (speed_callback_) {
            speed_callback_(speed);
        }
    }
    else if (cmd == "stop") {
        if (emergency_stop_callback_) {
            emergency_stop_callback_();
        }
    }
    else if (cmd == "detail") {
        std::string state;
        iss >> state;
        display_detailed_ = (state == "on" || state == "true" || state == "1");
    }
    else if (cmd == "clear") {
        clearScreen();
    }
    else if (cmd == "help") {
        displayHelpMenu();
    }
    else if (!cmd.empty()) {
        std::cout << "❌ 未知命令: " << cmd << " (输入 'help' 查看帮助)" << std::endl;
    }
}

std::string ConsoleDashboardView::getColorCode(int priority) {
    #ifdef _WIN32
        return "";     // Windows控制 台颜色较复杂，暂时不实现
    #else
        switch (priority) {
            case 0: return "\033[37m";   // 白色 - 信息
            case 1: return "\033[33m";   // 黄色 - 警告
            case 2: return "\033[31m";   // 红色 - 错误
            case 3: return "\033[35m";   // 紫色 - 紧急
            default: return "\033[0m";   // 重置
        }
    #endif
}

void ConsoleDashboardView::resetColor() {
    #ifndef _WIN32
        std::cout << "\033[0m";
    #endif
}

} // namespace duan