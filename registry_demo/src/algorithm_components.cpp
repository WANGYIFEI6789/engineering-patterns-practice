#include "component.hpp"
#include "registry.hpp"
#include <iostream>
#include <vector>

namespace duan {

// YOLOX目标检测算法
class YoloX_Detector : public Component {
    std::vector<std::string> input_;
public:
    YoloX_Detector(const nlohmann::json& cfg) : input_(cfg.at("input").get<std::vector<std::string>>()) {}
    void start() override {
        std::cout << "[YOLOX] Object detector with input: ";
        for (auto& in : input_) std::cout << in << " ";
        std::cout << std::endl;
    }
};

// EKF定位算法
class EKF_Localizer : public Component {
    std::vector<std::string> input_;
public:
    EKF_Localizer(const nlohmann::json& cfg) : input_(cfg.at("input").get<std::vector<std::string>>()) {}
    void start() override {
        std::cout << "[EKF] Localization with input: ";
        for (auto& in : input_) std::cout << in << " ";
        std::cout << std::endl;
    }
};

// 融合算法FusionV2
class FusionV2 : public Component {
    std::vector<std::string> input_;
public:
    FusionV2(const nlohmann::json& cfg) : input_(cfg.at("input").get<std::vector<std::string>>()) {}
    void start() override {
        std::cout << "[FusionV2] Sensor Fusion with input: ";
        for (auto& in : input_) std::cout << in << " ";
        std::cout << std::endl;
    }
};

}

using ComponentRegistry = duan::ComponentRegistry;

// 自动注册
namespace {
    const bool reg1 = []{
        ComponentRegistry::Register("yolox", [](const nlohmann::json& cfg){ return std::make_shared<duan::YoloX_Detector>(cfg); });
        return true;
    }();
    const bool reg2 = []{
        ComponentRegistry::Register("ekf", [](const nlohmann::json& cfg){ return std::make_shared<duan::EKF_Localizer>(cfg); });
        return true;
    }();
    const bool reg3 = []{
        ComponentRegistry::Register("fusion_v2", [](const nlohmann::json& cfg){ return std::make_shared<duan::FusionV2>(cfg); });
        return true;
    }();
}