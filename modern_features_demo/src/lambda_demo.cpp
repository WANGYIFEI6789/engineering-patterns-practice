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


        // STL 算法中使用 lambda
        void stl_lambda_demo(){
            std::cout << "stl_lambda_demo" << std::endl;

            // 使用 lambda 计算向量的总和
            std::vector<int> numbers = {1, 2, 3, 4, 5};
            
            // 使用 std::for_each 遍历向量并打印每个元素
            std::for_each(numbers.begin(), numbers.end(), [](int num) {
                std::cout << num << " ";
            });
            std::cout << std::endl;

            // 使用 lambda 对向量进行排序
            std::sort(numbers.begin(), numbers.end(), [](int a, int b) {
                return a > b; // 降序排序
            });
            std::cout << "Sorted numbers in descending order: ";
            for (const auto& num : numbers) {
                std::cout << num << " ";
            }
            std::cout << std::endl;

            // 使用 std::transform
            std::vector<int> squared;
            squared.resize(numbers.size());
            std::transform(numbers.begin(), numbers.end(), squared.begin(), [](int num) {
                return num * num; // 平方每个元素
            });
            std::cout << "Squared numbers: ";
            for (const auto& num : squared) {
                std::cout << num << " ";    
            };
            std::cout << std::endl;

            // 使用 std::count_if
            auto odd_count = std::count_if(numbers.begin(), numbers.end(), [](int num) {
                return num % 2 != 0; // 计算奇数的数量
            });
            std::cout << "Count of odd numbers: " << odd_count << std::endl;

            // 使用 std::accumulate 计算总和
            int sum = std::accumulate(numbers.begin(), numbers.end(), 0, [](int total, int num) {
                return total + num; // 计算总和
            });
            std::cout << "Sum of numbers: " << sum << std::endl;
        }

        // 泛型lambda
        void generic_lambda_demo() {
            std::cout << "generic_lambda_demo" << std::endl;

            // 泛型lambda
            auto generic_add = [](auto a, auto b) -> auto {
                return a + b;
            };

            std::cout << "int: " << generic_add(3, 5) << std::endl; // 整数相加
            std::cout << "double: " << generic_add(3.5, 2.5) << std::endl; // 浮点数相加
            std::cout << "string: " << generic_add(std::string("duan~"), std::string(" fei")) << std::endl; // 字符串相加
        }

        // 递归lambda
        void recursive_lambda_demo() {
            std::cout << "recursive_lambda_demo" << std::endl;

            // 递归lambda计算阶乘
            // 递归的时候 必须要把返回类型 参数类型 匿名函数类型都显示给出来
            // 因为lambda函数是匿名的，所以需要使用std::function来定义递归函数
            // lambda 表达式要递归，必须捕获自身变量（如 fibonacci），否则 lambda 体内无法引用自己！
            // C++ lambda 并没有天然名字，递归时需依赖外部变量
            std::function<int(int)> fibonacci = [&fibonacci](int n) -> int {
                return (n <= 1) ? 1 : fibonacci(n - 1) * fibonacci(n - 2);
            };

            std::cout << "Fibonacci(8): " << fibonacci(5) << std::endl; // 输出 8 的阶乘
        }

        using namespace duan::demo;
        void lambda_demo() {
            std::cout << "lambda_demo" << std::endl;
            basic_lambda_demo();
            capture_lambda_demo();
            stl_lambda_demo();
            generic_lambda_demo();
            recursive_lambda_demo();
        }
    }
}