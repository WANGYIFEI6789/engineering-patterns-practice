#include <iostream>
#include <vector>
#include <memory>
#include <iomanip> // 输入输出格式化头文件
#include "adaptor/sensor_interface.hpp"
#include "adaptor/modern_camera.hpp"
#include "adaptor/lidar_adaptor.hpp"

namespace duan
{

/*
自动驾驶传感器管理器
*/
class SensorManager {
private:
    std::vector<std::unique_ptr<SensorInterface>> sensors_; // 存储传感器的容器

public:
    // 添加传感器
    void addSensor(std::unique_ptr<SensorInterface> sensor) {
        sensors_.push_back(std::move(sensor));
    }

    // 初始化所有传感器
    bool initSensors() {
        std::cout << "Initializing sensors..." << std::endl;
        bool all_success = true;

        for(auto& sensor : sensors_) {
            bool success = sensor->init();
            if (success) {
                std::cout << "Sensor " << sensor->getName() << " initialized successfully." << std::endl;
                all_success &= success;
            } 
        }
        return all_success;
    }

    // 获取所有传感器数据
    void getAllSensorData() {
        std::cout << "Getting sensor data..." << std::endl;
        for (const auto& sensor : sensors_) {
            SensorInterface::SensorDate data = sensor->getSensorData();
            std::cout << "Sensor: " << sensor->getName() 
                      << ", Timestamp: " << data.timestamp 
                      << ", Points: " << data.points.size() 
                      << ", Frame ID: " << data.frame_id << std::endl;
            // 显示前几个数据点
            if(!data.points.empty()) {
                std::cout << "前5个数据点: ";
                for(size_t i = 0; i < std::min(data.points.size(), size_t(5)); ++i) {
                    std::cout << std::fixed << std::setprecision(2) << data.points[i] << " ";
                }
                std::cout << std::endl;
            } 
        }
    }

    // 停止所有传感器
    void stopAllSensors() {
        std::cout << "Stopping all sensors..." << std::endl;
        for (const auto& sensor : sensors_) {
            sensor->stop();
            std::cout << "Sensor " << sensor->getName() << " stopped." << std::endl;
        }
    }
};
    
} // namespace name

int main(){
    std::cout << "=== 自动驾驶适配器模式演示 ===" << std::endl;
    std::cout << "演示如何使用适配器模式将老式传感器接口适配到现代传感器接口" << std::endl;
    
    // 创建传感器管理器
    duan::SensorManager sensor_manager;

    // 添加现代摄像头
    auto camera = std::make_unique<duan::ModernCamera>("Modern Camera 1");
    sensor_manager.addSensor(std::move(camera));

    // 添加老式激光雷达适配器
    auto lidar_adaptor = std::make_unique<duan::LidarAdaptor>("Legacy Lidar 1");
    sensor_manager.addSensor(std::move(lidar_adaptor));

    // 初始化所有传感器
    if (sensor_manager.initSensors()) {
        // 获取所有传感器数据
        sensor_manager.getAllSensorData();
    } else {
        std::cerr << "Failed to initialize some sensors." << std::endl;
    }

    // 停止所有传感器
    sensor_manager.stopAllSensors();

    return 0;
}