#ifndef LIDAR_ADAPTOR_H
#define LIDAR_ADAPTOR_H

#include "sensor_interface.hpp"
#include "legacy_lidar.hpp"
#include <memory>

namespace duan {

/*
    * 激光雷达适配器类
    * 适配老式激光雷达接口到统一的传感器接口
*/
class LidarAdaptor : public SensorInterface {
private:
    std::unique_ptr<LegacyLidar> legacy_lidar_; // 老式激光雷达实例
    std::string adaptor_name_; // 适配器名称

public:
    explicit LidarAdaptor(const std::string& device_id);
    virtual ~LidarAdaptor();

    // 实现标准传感器接口
    bool init() override;
    SensorDate getSensorData() override;
    void stop() override;
    std::string getName() const override;

private:
    // 数据转换辅助函数
    std::vector<double> convertFloatToDouble(const std::vector<float>& input);
    double convertTimestamp(long long legacy_timestamp);
};

}

#endif