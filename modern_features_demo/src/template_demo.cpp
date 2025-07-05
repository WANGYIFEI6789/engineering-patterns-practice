#include <iostream>
#include <vector>
#include <string>
#include <type_traits>
#include "demos.h"

namespace duan{
namespace demo {

// 基础模板类
template <typename T>
class DuanStack{
public:
    void push(const T& element){
        elements_.push_back(element);
    }

    void pop(){
        if(elements_.empty()){
            std::cout << "Stack is empty, cannot pop." << std::endl;
            return;
        }
        elements_.pop_back();
    }
    size_t size() const{
        return elements_.size();
    }
private:
    std::vector<T> elements_;
};

void basic_template_demo(){
    DuanStack<int> intstack;
    intstack.push(1);
    intstack.push(2);
    intstack.push(3);
    std::cout << "intstack size: " << intstack.size() << std::endl;

    intstack.pop();
    std::cout << "intstack size after pop: " << intstack.size() << std::endl;

    DuanStack<std::string> stringstack;
    stringstack.push("Hello");
    stringstack.push("World");
    std::cout << "stringstack size: " << stringstack.size() << std::endl;
    stringstack.pop();
    std::cout << "stringstack size after pop: " << stringstack.size() << std::endl;
    return;
}

// 函数模板示例
template <typename T>
T max(const T& a, const T& b) {
    return (a > b) ? a : b;
}

void function_template_demo() {
    std::cout << "function_template_demo" << std::endl;

    int a = 5, b = 10;
    double x = 3.14, y = 2.71;

    std::cout << "Max of " << a << " and " << b << " is: " << max(a, b) << std::endl;
    std::cout << "Max of " << x << " and " << y << " is: " << max(x, y) << std::endl;
}

// 多参数模板
template<typename T, typename U>
auto add(const T& a, const U& b) -> decltype(a + b) {
    return a + b;
}

void multi_param_template_demo() {
    std::cout << "multi_param_template_demo" << std::endl;

    int a = 5;
    double b = 3.14;

    std::cout << "Add int and double: " << add(a, b) << std::endl; // int + double
}

void template_demo() {
    std::cout << "template_demo" << std::endl;
    basic_template_demo();
    function_template_demo();
    multi_param_template_demo();
}

}
}