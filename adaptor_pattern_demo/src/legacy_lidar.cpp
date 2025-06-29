#include "adaptor/legacy_lidar.hpp"
#include <iostream>
#include <chrono>
#include <random>

namespace duan {

LegacyLidar::LegacyLidar(const std::string& device_id) 
    : is_running_(false), device_id_(device_id) {
        std::cout << "[LegacyLidar] 创建设备: " << device_id_ << std::endl;
}

LegacyLidar::~LegacyLidar() {
    if (is_running_) {
        stopDevice();
    }
    std::cout << "[LegacyLidar] 销毁设备: " << device_id_ << std::endl;
}

bool LegacyLidar::startDevice() {
    if (is_running_) {
        std::cout << "[LegacyLidar] 设备已在运行: " << device_id_ << std::endl;
        return false;
    }
    is_running_ = true;
    std::cout << "[LegacyLidar] 启动设备: " << device_id_ << std::endl;
    return true;
}

void LegacyLidar::stopDevice() {
    if (!is_running_) {
        std::cout << "[LegacyLidar] 设备未运行: " << device_id_ << std::endl;
        return;
    }
    is_running_ = false;
    std::cout << "[LegacyLidar] 停止设备: " << device_id_ << std::endl;
}

std::vector<float> LegacyLidar::readLidarPoints() {
    if (!is_running_) {
        std::cerr << "[LegacyLidar] 设备未运行，无法读取数据: " << device_id_ << std::endl;
        return {};
    }

    // 模拟生成一些激光点数据
    std::vector<float> points;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0.5f, 100.0f);

    // 生成模拟点云数据
    for(int i = 0; i < 360; ++i){
        points.push_back(dist(gen)); // 模拟每个点的距离
    }
    return points;
}

long long LegacyLidar::getCurrentTimestamp() {
    if (!is_running_) {
        std::cerr << "[LegacyLidar] 设备未运行，无法获取时间戳: " << device_id_ << std::endl;
        return 0;
    }
    // 获取当前时间戳（毫秒）
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
}

bool LegacyLidar::isDeviceRunning() const {
    return is_running_;
}

std::string LegacyLidar::getDeviceId() const {
    return device_id_;
}

}