#ifndef LEGACY_LIDAR_H
#define LEGACY_LIDAR_H

#include <vector>
#include <string>

namespace duan {

/*
 * 老式激光雷达类
 * 模拟一个老式的激光雷达，其接口与我们的标准不兼容
 */
class LegacyLidar {
private:
    bool is_running_; // 激光雷达是否正在运行
    std::string device_id_; // 设备ID

public:
    explicit LegacyLidar(const std::string& device_id);
    ~LegacyLidar();

    // 老式接口 - 与标准接口不兼容
    bool startDevice();
    void stopDevice();
    std::vector<float> readLidarPoints();
    long long getCurrentTimestamp();
    std::string getDeviceId() const;
    bool isDeviceRunning() const;
};

}

#endif