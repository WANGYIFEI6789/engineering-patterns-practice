#include <iostream>
#include "demos.h"

using namespace duan::demo;

int main(){
    std::cout << "Modern C++ Features Demo" << std::endl;

    // 演示智能指针
    smart_pointers_demo();

    // 演示 Lambda 表达式
    lambda_demo();

    // 演示锁和多线程
    locks_demo();

    // 演示模板
    template_demo();

    // 演示模板特化和 SFINAE
    template_specialization_demo();

    return 0;
}