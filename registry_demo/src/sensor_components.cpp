#include "component.hpp"
#include "registry.hpp"
#include <iostream>

namespace duan {

// RoboSense 激光雷达
class RoboSenseLidar : public duan::Component {
    std::string topic_;
public:
    RoboSenseLidar(const nlohmann::json& cfg) : topic_(cfg.at("topic")) {}

    void start() override {
        std::cout << "[RoboSenseLidar] listening " << topic_ << std::endl;
    }
};

// Hikvision 摄像头
class HikvisionCamera : public Component {
    std::string topic_;
public:
    HikvisionCamera(const nlohmann::json& cfg) : topic_(cfg.at("topic")) {}
    void start() override {
        std::cout << "[HikvisionCamera] streaming " << topic_ << std::endl;
    }
};

// Ublox GNSS
class UbloxGnss : public Component {
    std::string topic_;
public:
    UbloxGnss(const nlohmann::json& cfg) : topic_(cfg.at("topic")) {}
    void start() override {
        std::cout << "[UbloxGNSS] acquiring " << topic_ << std::endl;
    }
};

}

using ComponentRegistry = duan::ComponentRegistry;

// 静态注册模块，实现自动注册
namespace {

    // 会放到map中
    const bool reg1 = [](){
        ComponentRegistry::Register("robosense", [](const nlohmann::json& cfg) {
            return std::make_shared<duan::RoboSenseLidar>(cfg);
        });
        return true;
    }();

    const bool reg2 = []{
        ComponentRegistry::Register("hikvision", [](const nlohmann::json& cfg){ return std::make_shared<duan::HikvisionCamera>(cfg); });
        return true;
    }();

    const bool reg3 = []{
        ComponentRegistry::Register("ublox", [](const nlohmann::json& cfg){ return std::make_shared<duan::UbloxGnss>(cfg); });
        return true;
    }();
}