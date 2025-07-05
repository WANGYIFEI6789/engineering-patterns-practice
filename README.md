# 工程思想
## 注册表模式
注册表模式是一种通过全局或统一的注册表来集中管理和动态查找（创建）各类对象或服务实例，从而实现模块解耦与灵活扩展的设计模式
```bash
# registry_demo目录下
mkdir build && cd build
cmake ..
make
./registry_demo
```
## 构建动态链接库
构建动态链接库的目的是为了实现代码的复用和模块化，使多个程序可以共享同一份功能实现并便于后续独立升级或维护  
静态库是在程序编译（链接）时合入到可执行文件的，动态库是程序运行到时由系统动态加载的
```bash
# stringlib_demo
# 通过CMakeLists.txt 生成动态库 放在lib文件夹下
mkdir build && cd build
cmake ..
make
# 在test文件夹下进行测试
cd ../test
g++ test.cpp -I../include -L../lib -lstrutil -o test  #  -I 指定头文件查找目录 -L 指定库文件查找目录 -lxxx 会自动链接名为libxxx.so或libxxx.a的库
# 设置LD_LIBRARY_PATH，动态库路径查找
export LD_LIBRARY_PATH=../lib:$LD_LIBRARY_PATH
./test
```
## 适配器模式
适配器模式的应用，非常适合自动驾驶系统的传感器集成场景。通过适配器模式，可以轻松集成各种不同接口的传感器，而无需修改现有的系统架构  
简单来说就是引入一个新的包装器进行接口转换  
```bash
mkdir build && cd build
cmake ..
make
./adaptor_demo
# 测试代码
cd test/
./test_adaptor
```
## MVP架构
MVP = model + view + present  
MVP架构在自动驾驶系统中能够很好地管理复杂的车辆状态、传感器数据和用户交互  
model: 存储车辆状态、传感器数据  
view: 展示数据给用户，接收用户输入，但不包含任何业务逻辑  
present: 负责协调、控制和处理  
```bash
mkdir build && cd build
cmake ..
make -j4
# 运行程序
./autonomous_mvp
# 交互式运行
./autonomous_mvp --interactive
# 测试用例
make test    # ./test/test_mvp
```
## 日志系统
这个日志系统轻量级、易用且功能完整，适合大多数C++项目使用  
```bash
mkdir build && cd build
cmake ..
make
./logger_example
./logger_test
# 若需要使用我的日志系统
make install
```
## C++新特性
包含智能指针、锁、lambda表达式、模板、偏特化、全特化
```bash
mkdir build && cd build
cmake ..
make
./bin/CppFeaturesDemo
```