#ifndef MODERN_CAMERA_H
#define MODERN_CAMERA_H

#include "sensor_interface.hpp"

namespace duan {

/*
 * 现代摄像头类
 * 模拟一个现代摄像头，其接口与我们的标准兼容
*/
class ModernCamera : public SensorInterface {
private:
    bool is_initialized_; // 摄像头是否已初始化
    std::string camera_name_; // 摄像头名称

public:
    explicit ModernCamera(const std::string& name);
    virtual ~ModernCamera();

    bool init() override;
    SensorDate getSensorData() override;
    void stop() override;
    std::string getName() const override;
};

}

#endif