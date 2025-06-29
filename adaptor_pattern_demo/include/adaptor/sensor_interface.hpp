#ifndef SENSOR_INTERFACE_H
#define SENSOR_INTERFACE_H

#include <vector>
#include <string>

namespace duan{

/*
统一的传感器接口
希望所有传感器都需要实现这个接口
*/
class SensorInterface {
public:
    struct SensorDate
    {
        /* data */
        double timestamp; // 时间戳
        std::vector<double> points; // 传感器数据点
        std::string frame_id; // 坐标系ID

        SensorDate(double ts = 0.0, const std::string& id = "base_link") : timestamp(ts), frame_id(id) {}
    };

    virtual ~SensorInterface() = default;

    /*
    初始化传感器
    return 是否初始化成功
    */
   virtual bool init() = 0;

    /*
    获取传感器数据
    return 传感器数据
    */
    virtual SensorDate getSensorData() = 0;

    /*
    获取传感器名称
    return 传感器名称
    */
    virtual std::string getName() const = 0;

    /*
    停止传感器
    */
    virtual void stop() = 0;
    
};

}

#endif