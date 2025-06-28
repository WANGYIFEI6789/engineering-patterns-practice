#include "component.hpp"
#include "registry.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include "nlohmann/json.hpp"

int main(){
    std::ifstream fin("../config/pipeline_config.json");
    if(!fin){
        std::cerr << "Config file open failed!" << std::endl;
    }

    nlohmann::json config;
    fin >> config;

    std::vector<std::shared_ptr<duan::Component>> sensors;
    std::vector<std::shared_ptr<duan::Component>> algos;

    // 创建所有传感器对象并启动
    for(const auto& sn : config["sensors"]){
        auto inst = duan::ComponentRegistry::Create(sn.at("type"), sn);
        if(inst){
            sensors.push_back(inst);
            inst->start();
        } else {
            std::cerr << "Sensor create failed" << sn["type"] << std::endl;
        }
    }

    // 创建所有算法对象并启动
    for (const auto& algo : config["algorithms"]) {
        auto inst = duan::ComponentRegistry::Create(algo["type"], algo);
        if (inst) {
            algos.push_back(inst);
            inst->start();
        }
        else {
            std::cout << "Algo create failed: " << algo["type"] << std::endl;
        }
    }

    return 0;
}