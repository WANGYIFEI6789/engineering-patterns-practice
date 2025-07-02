#include <iostream>
#include <vector>
#include <functional>
#include <functional>
#include <numeric>
#include "demos.h"

namespace duan{
    namespace lambda_demo{
        // 基本lambda示例
        void basic_lambda_demo() {
            std::cout << "basic_lambda_demo" << std::endl;

            // 无参lambda
            auto greet = [](){
                std::cout << "Hello Lambda!" << std::endl;
            };
            greet();

            // 有参lambda
            auto add = [](int a, int b) {
                return a + b;
            };
            int result = add(3, 5);
            std::cout << "Result of add(3, 5): " << result << std::endl;

            // 捕获外部变量
            int multiply = 10;
            auto mul = [multiply](int x) {
                return x * multiply;
            };
            std::cout << "Result of mul(5)    5 * 10: " << mul(5) << std::endl;
        }

        // 捕获lambda示例
        void capture_lambda_demo() {
            std::cout << "capture_lambda_demo" << std::endl;

            int a = 5, b = 5;
            // 值捕获 只能使用
            auto capture_by_value = [a, b]() {
                // a = 10; // 编译错误，不能修改值捕获的值
                std::cout << "a = " << a << std::endl;
                std::cout << "b = " << b << std::endl;
            };

            // 引用捕获
            auto capture_by_ref = [&a, &b]() {
                a = 10; // 可以修改引用捕获的值
                b = 20; // 可以修改引用捕获的值
                std::cout << "a = " << a << std::endl;
                std::cout << "b = " << b << std::endl;
            };

            // 混合捕获
            int c = 30;
            auto mixed_capture = [=, &c](int x) mutable {
                a += x;  // 修改拷贝的值
                c += x;  // 修改引用的值
                std::cout << "混合捕获: a=" << a << ", c=" << c << std::endl;
            };
    
            capture_by_value();
            capture_by_ref();
            mixed_capture(15);
            
            std::cout << "外部变量: a=" << a << ", b=" << b << ", c=" << c << std::endl;
        }
    }
}