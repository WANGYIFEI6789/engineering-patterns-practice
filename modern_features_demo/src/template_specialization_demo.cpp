#include <iostream>
#include <vector>
#include <string>
#include <type_traits>
#include <set>
#include <list>
#include "demos.h"

namespace duan{

namespace demo {

// 原始模板
template<typename T1, typename T2>
class Pair{
public:
    T1 first;
    T2 second;
    void print() const {
        std::cout << "Pair: (" << first << ", " << second << ")" << std::endl;
        std::cout << "Generic Pair" << std::endl;
    }
};

// 偏特化：当两个类型相同时
template<typename T>
class Pair<T, T>{
public:
    T first;
    T second;
    void print() const {
        std::cout << "Pair: (" << first << ", " << second << ")" << std::endl;
        std::cout << "Specialized Pair for same types" << std::endl;
    }  
};

// 偏特化：指针类型
template<typename T>
class Pair<T*, T*>{
public:
    T* first;
    T* second;
    void print() const {
        std::cout << "Pair: (" << *first << ", " << *second << ")" << std::endl;
        std::cout << "Specialized Pair for pointer types" << std::endl;
    }
};

// 全特化：当两个类型都是int时
template<>
class Pair<int, int>{
public:
    int first;
    int second;
    void print() const {
        std::cout << "Pair: (" << first << ", " << second << ")" << std::endl;
        std::cout << "Specialized Pair for int types" << std::endl;
    }
};

void duan_template_specialization_demo() {
    std::cout << "template_specialization_demo" << std::endl;

    // 使用原始模板
    Pair<int, double> p1;
    p1.first = 10;
    p1.second = 20.5;
    p1.print(); // 输出 Pair: (10, 20.5) Generic Pair

    // 使用偏特化的模板
    Pair<int, int> p2;
    p2.first = 30;
    p2.second = 40;
    p2.print(); // 输出 Pair: (30, 40) Specialized Pair for same types

    // 使用指针类型的偏特化模板
    int a = 50, b = 60;
    Pair<int*, int*> p3;
    p3.first = &a;
    p3.second = &b;
    p3.print(); // 输出 Pair: (50, 60) Specialized Pair for pointer types

    // 使用全特化的模板
    Pair<int, int> p4;
    p4.first = 70;
    p4.second = 80;
    p4.print(); // 输出 Pair: (70, 80) Specialized Pair for int

}

// SFINAE 替换而不是失败
// 检查类型是否有push_back方法
template<typename T>
class has_push_back {
private:
    // 使用 decltype 检查是否有 push_back 方法
    template<typename U>
    static auto test(int) -> decltype(std::declval<U>().push_back(std::declval<typename U::value_type>()), std::true_type());

    // 如果没有 push_back 方法，则返回 false_type
    template<typename>
    static std::false_type test(...);
public:
    // 如果有 push_back 方法，则返回 true_type，否则返回 false_type
    static constexpr bool value = decltype(test<T>(0))::value;
};

// 根据是否有push_back使用不同的插入方式
template<typename Container, typename Value>
typename std::enable_if<has_push_back<Container>::value>::type
insert_value(Container& container, const Value& value) {
    container.push_back(value);
    std::cout << "Using push_back" << std::endl;
}

template<typename Container, typename Value>
typename std::enable_if<!has_push_back<Container>::value>::type
insert_value(Container& container, const Value& value) {
    container.insert(value);
    std::cout << "Using insert" << std::endl;
}

void sfinae_demo() {
    std::cout << "SFINAE Demo" << std::endl;

    // 使用 vector
    std::vector<int> vec;
    insert_value(vec, 10); // 使用 push_back

    // 使用 set
    std::set<int> my_set;
    insert_value(my_set, 20); // 使用 insert

    // 使用 list
    std::list<std::string> my_list;
    insert_value(my_list, "Hello"); // 使用 push_back
}

template<typename... Args>
void print(Args... args) {
    ((std::cout << args << " "), ...);  // C++17 fold expression
    std::cout << std::endl;
}

// C++11版本
template<typename T>
void print11(T&& t) {
    std::cout << t << std::endl;
}

// 可变参数模板
template<typename T, typename... Args>
void print11(T&& t, Args&&... args) {
    std::cout << t << " ";
    print11(std::forward<Args>(args)...);
}

void print_demo() {
    std::cout << "print_demo" << std::endl;

    // 使用 C++17 fold expression
    print(1, 2.5, "Hello", 'A');

    // 使用 C++11 可变参数模板
    print11(1, 2.5, "Hello", 'A');
}

// 

void template_specialization_demo() {
    std::cout << "template_specialization_demo_main" << std::endl;

    // 调用模板特化演示
    duan_template_specialization_demo();

    // 调用 SFINAE 演示
    sfinae_demo();
    // 调用打印演示
    print_demo();
}
/*
介绍一下SIFNAE（Substitution Failure Is Not An Error）：
SFINAE 是 C++ 模板编程中的一个重要概念，它允许在模板参数替换失败时不导致编译错误，而是选择其他重载或特化。
SFINAE 的主要目的是在模板编程中提供更大的灵活性和可扩展性。
通过 SFINAE，我们可以根据类型特性选择不同的函数或类模板，从而实现更复杂的类型推断和特化。
SFINAE 的常见用法包括：
1. 检查类型是否具有特定成员函数或类型。
2. 根据类型特性选择不同的函数或类模板。
3. 实现类型萃取（Type Traits）和类型特化（Type Specialization）。
SFINAE 的实现通常使用 `decltype`、`std::enable_if` 和模板特化等技术。

auto 函数名(...) -> decltype(expression) {
    // 函数体
    return expression;
}
这种写法叫做：尾置返回类型（trailing return type），是 C++11 引入的函数声明方式
它常常用于只有在函数参数作用域内才能确定返回类型，比如模板和泛型编程
auto在前面告诉编译器 位置返回类型要在后面看

auto...->decltype(...) 必须在返回值依赖参数类型但参数还没声明之前，
用尾置返回类型让编译器“看到”参数再推导返回类型，非常适合模板编程和SFINAE
*/

}
}