#include "adaptor/lidar_adaptor.hpp"
#include <iostream>

namespace duan {

LidarAdaptor::LidarAdaptor(const std::string& device_id)
    : legacy_lidar_(std::make_unique<LegacyLidar>(device_id)), adaptor_name_("LidarAdaptor_" + device_id) {
    std::cout << "[LidarAdaptor] 创建适配器: " << adaptor_name_ << " for device: " << device_id << std::endl;
}

LidarAdaptor::~LidarAdaptor() {
    std::cout << "[LidarAdaptor] 销毁适配器: " << adaptor_name_ << std::endl;
}

bool LidarAdaptor::init() {
    std::cout << "[LidarAdaptor] 初始化适配器: " << adaptor_name_ << std::endl;
    return legacy_lidar_->startDevice();
}

SensorInterface::SensorDate LidarAdaptor::getSensorData() {
    std::cout << "[LidarAdaptor] 获取传感器数据: " << adaptor_name_ << std::endl;
    SensorInterface::SensorDate data;
    
    if (legacy_lidar_->isDeviceRunning()) {
        auto points = legacy_lidar_->readLidarPoints();
        data.points = convertFloatToDouble(points);
        data.timestamp = convertTimestamp(legacy_lidar_->getCurrentTimestamp());
        data.frame_id = "lidar_" + legacy_lidar_->getDeviceId();
    } else {
        std::cerr << "[LidarAdaptor] 设备未运行，无法获取数据: " << adaptor_name_ << std::endl;
    }
    
    return data;
}

void LidarAdaptor::stop() {
    std::cout << "[LidarAdaptor] 停止适配器: " << adaptor_name_ << std::endl;
    legacy_lidar_->stopDevice();
}

std::string LidarAdaptor::getName() const {
    return adaptor_name_;
}

std::vector<double> LidarAdaptor::convertFloatToDouble(const std::vector<float>& input) {
    std::vector<double> output;
    output.reserve(input.size());
    for (const auto& point : input) {
        output.push_back(static_cast<double>(point));
    }
    return output;
}

double LidarAdaptor::convertTimestamp(long long legacy_timestamp) {
    // 假设legacy_timestamp是以微秒为单位的时间戳
    return static_cast<double>(legacy_timestamp) / 1000000.0; // 转换为秒
}

}