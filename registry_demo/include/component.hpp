#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

namespace duan{

// 所有传感器/算法模块的父类接口
struct Component{
    virtual void start() = 0; // 启动组件
    virtual ~Component() = default; // 虚析构函数
};

}