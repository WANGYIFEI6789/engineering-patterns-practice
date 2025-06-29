#include "adaptor/modern_camera.hpp"
#include <iostream>
#include <chrono>
#include <random>

namespace duan {

ModernCamera::ModernCamera(const std::string& name)
    : is_initialized_(false), camera_name_(name) {
    std::cout << "[ModernCamera] 创建摄像头: " << camera_name_ << std::endl;
}

ModernCamera::~ModernCamera() {
    if (is_initialized_) {
        stop();
    }
    std::cout << "[ModernCamera] 销毁摄像头: " << camera_name_ << std::endl;
}

bool ModernCamera::init() {
    if (is_initialized_) {
        std::cout << "[ModernCamera] 摄像头已初始化: " << camera_name_ << std::endl;
        return false;
    }
    is_initialized_ = true;
    std::cout << "[ModernCamera] 初始化摄像头: " << camera_name_ << std::endl;
    return true;
}

SensorInterface::SensorDate ModernCamera::getSensorData() {
    if (!is_initialized_) {
        std::cerr << "[ModernCamera] 摄像头未初始化，无法获取数据: " << camera_name_ << std::endl;
        return SensorInterface::SensorDate();
    }

    SensorInterface::SensorDate data;
    data.timestamp = static_cast<double>(std::chrono::system_clock::now().time_since_epoch().count()) / 1000000.0; // 转换为秒
    data.frame_id = "camera_" + camera_name_;

    // 模拟生成一些摄像头数据点
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0.0, 255.0); // 模拟RGB值

    for (int i = 0; i < 100; ++i) { // 假设每次获取10个数据点
        data.points.push_back(dist(gen));
    }

    std::cout << "[ModernCamera] 获取摄像头数据: " << camera_name_ << std::endl;
    return data;
}

void ModernCamera::stop() {
    if (!is_initialized_) {
        std::cout << "[ModernCamera] 摄像头未初始化，无法停止: " << camera_name_ << std::endl;
        return;
    }
    is_initialized_ = false;
    std::cout << "[ModernCamera] 停止摄像头: " << camera_name_ << std::endl;
}

std::string ModernCamera::getName() const {
    return camera_name_;
}

}