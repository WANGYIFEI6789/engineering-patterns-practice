#pragma once
#include <memory>
#include <string>
#include <functional>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include "component.hpp"

namespace duan {

// 通用注册表
class ComponentRegistry {
public:
    using Creator = std::function<std::shared_ptr<Component>(const nlohmann::json&)>;
    
    static void Register(const std::string& type, Creator creator) {
        getMap()[type] = creator;
    }

    static std::shared_ptr<Component> Create(const std::string& type, const nlohmann::json& cfg) {
        auto it = getMap().find(type);
        if (it != getMap().end()) {
            return it->second(cfg);
        }
        return nullptr; // 或者抛出异常
    }
private:
    static std::unordered_map<std::string, Creator>& getMap(){
        static std::unordered_map<std::string, Creator> map;
        return map;
    }
};

}

/*
类型别名定义
定义创建函数的统一接口
using Creator = std::function<std::shared_ptr<Component>(const nlohmann::json&)>;

注册函数
将组件类型名称与其创建函数关联
static void Register(const std::string& type, Creator creator)；

作用：根据类型名称查找并调用对应的创建函数
返回：创建的组件实例，如果类型未注册则返回 nullptr
static std::shared_ptr<Component> Create(const std::string& type, const nlohmann::json& cfg);

存储类型名称到创建函数的映射关系
static std::unordered_map<std::string, Creator>& getMap();
*/