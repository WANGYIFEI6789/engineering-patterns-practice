#include <iostream>
#include <memory>
#include <chrono>
#include <thread>
#include <csignal>
#include <atomic>

#include "model/vehicle_model.hpp"
#include "view/dashboard_view.hpp"
#include "present/main_presenter.hpp"

using namespace duan;

// 全局退出标志
std::atomic<bool> g_exit_requested(false);
std::shared_ptr<MainPresenter> g_presenter;

// 信号处理函数
void signalHandler(int signal) {
    std::cout << "\n[Main] 收到退出信号: " << signal << std::endl;
    g_exit_requested = true;
    
    if (g_presenter) {
        g_presenter->stop();
    }
}

// 程序信息显示
void showProgramInfo() {
    std::cout << "====================================================" << std::endl;
    std::cout << "     自动驾驶MVP架构演示程序 v1.0" << std::endl;
    std::cout << "     Model-View-Presenter Pattern Demo" << std::endl;
    std::cout << "====================================================" << std::endl;
    std::cout << "功能特点:" << std::endl;
    std::cout << "• 实时车辆状态监控" << std::endl;
    std::cout << "• 传感器数据可视化" << std::endl;
    std::cout << "• 驾驶模式切换控制" << std::endl;
    std::cout << "• 安全系统实时监控" << std::endl;
    std::cout << "• 用户交互命令界面" << std::endl;
    std::cout << "====================================================" << std::endl;
    std::cout << "\n";
}

// 程序使用说明
void showUsageInstructions() {
    std::cout << "💡 使用说明:" << std::endl;
    std::cout << "1. 程序启动后会自动开始车辆仿真" << std::endl;
    std::cout << "2. 在命令提示符下输入命令进行交互" << std::endl;
    std::cout << "3. 输入 'help' 查看所有可用命令" << std::endl;
    std::cout << "4. 输入 'quit' 或按 Ctrl+C 退出程序" << std::endl;
    std::cout << "5. 系统会自动进行安全监控和故障处理\n" << std::endl;
}

// 等待用户确认
void waitForUserConfirmation() {
    std::cout << "按 Enter 键开始演示...";
    std::cin.get();
    std::cout << std::endl;
}

// 创建和配置系统组件
std::shared_ptr<MainPresenter> createSystem() {
    std::cout << "[Main] 创建系统组件..." << std::endl;
    
    try {
        // 创建模型
        auto model = std::make_unique<VehicleModel>();
        std::cout << "[Main] 车辆模型创建完成" << std::endl;
        
        // 创建视图
        auto view = std::make_unique<ConsoleDashboardView>();
        std::cout << "[Main] 控制台视图创建完成" << std::endl;
        
        // 创建控制器
        auto presenter = std::make_shared<MainPresenter>(
            std::move(model), std::move(view));
        std::cout << "[Main] 主控制器创建完成" << std::endl;
        
        return presenter;
        
    } catch (const std::exception& e) {
        std::cerr << "[Main] 系统创建失败: " << e.what() << std::endl;
        return nullptr;
    }
}

// 运行演示程序
int runDemo() {
    std::cout << "[Main] 开始运行MVP演示..." << std::endl;
    
    // 创建系统
    g_presenter = createSystem();
    if (!g_presenter) {
        std::cerr << "[Main] 系统创建失败" << std::endl;
        return -1;
    }
    
    // 初始化系统
    std::cout << "[Main] 初始化系统..." << std::endl;
    if (!g_presenter->initialize()) {
        std::cerr << "[Main] 系统初始化失败" << std::endl;
        return -1;
    }
    
    // 启动系统
    std::cout << "[Main] 启动系统..." << std::endl;
    g_presenter->start();
    
    std::cout << "[Main] 系统运行中，等待用户交互..." << std::endl;
    std::cout << "[Main] 提示：输入命令进行交互，输入 'quit' 退出\n" << std::endl;
    
    // 主循环 - 等待退出信号
    while (!g_exit_requested && g_presenter->isRunning()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        // 检查系统状态
        if (!g_presenter->isRunning()) {
            std::cout << "[Main] 系统已停止运行" << std::endl;
            break;
        }
    }
    
    // 停止系统
    std::cout << "\n[Main] 正在停止系统..." << std::endl;
    g_presenter->stop();
    
    std::cout << "[Main] 系统已安全停止" << std::endl;
    return 0;
}

// 清理资源
void cleanup() {
    std::cout << "[Main] 清理系统资源..." << std::endl;
    
    if (g_presenter) {
        g_presenter.reset();
        std::cout << "[Main] 系统资源清理完成" << std::endl;
    }
}

// 主函数
int main(int argc, char* argv[]) {
    // 设置信号处理
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);
    
    try {
        // 显示程序信息
        showProgramInfo();
        showUsageInstructions();
        
        // 可选：等待用户确认（便于演示）
        if (argc > 1 && std::string(argv[1]) == "--interactive") {
            waitForUserConfirmation();
        }
        
        // 运行演示
        int result = runDemo();
        
        // 清理资源
        cleanup();
        
        std::cout << "\n====================================================" << std::endl;
        std::cout << "     感谢使用自动驾驶MVP演示程序!" << std::endl;
        std::cout << "====================================================" << std::endl;
        
        return result;
        
    } catch (const std::exception& e) {
        std::cerr << "[Main] 程序异常: " << e.what() << std::endl;
        cleanup();
        return -1;
    } catch (...) {
        std::cerr << "[Main] 未知异常发生" << std::endl;
        cleanup();
        return -1;
    }
}