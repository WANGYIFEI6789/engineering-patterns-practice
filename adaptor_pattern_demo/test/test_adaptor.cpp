#include <iostream>
#include <cassert>
#include "adaptor/lidar_adaptor.hpp"
#include "adaptor/modern_camera.hpp"

using namespace duan;

void testLidarAdapter() {
    std::cout << "测试激光雷达适配器..." << std::endl;
    
    LidarAdaptor adaptor("TEST_LIDAR");
    
    // 测试初始化
    assert(adaptor.init());
    assert(adaptor.getName().find("LidarAdaptor") != std::string::npos);
    
    // 测试数据获取
    auto data = adaptor.getSensorData();
    assert(data.timestamp > 0);
    assert(!data.points.empty());
    assert(data.frame_id.find("lidar") != std::string::npos);
    
    // 测试关闭
    adaptor.stop();
    
    std::cout << "激光雷达适配器测试通过！" << std::endl;
}

void testModernCamera() {
    std::cout << "测试现代摄像头..." << std::endl;
    
    ModernCamera camera("TEST_CAMERA");
    
    // 测试初始化
    assert(camera.init());
    assert(camera.getName() == "TEST_CAMERA");
    
    // 测试数据获取
    auto data = camera.getSensorData();
    assert(data.timestamp > 0);
    assert(!data.points.empty());
    assert(data.frame_id.find("camera") != std::string::npos);
    
    // 测试关闭
    camera.stop();
    
    std::cout << "现代摄像头测试通过！" << std::endl;
}

int main() {
    std::cout << "=== 适配器模式单元测试 ===" << std::endl;
    
    try {
        testLidarAdapter();
        testModernCamera();
        
        std::cout << "所有测试通过！" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "测试失败: " << e.what() << std::endl;
        return 1;
    }
}