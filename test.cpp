#include <iostream>
#include <cmath>
#include <type_traits>


// requires 表达式可以用来定义概念
template <typename T>
concept MyConcept = requires(T a, T b) {
    // requires 中可以“假设”定义若干个 T 类型的变量，然后检查操作合法性
    
    // 简单要求：检查某个表达式是否有效
    a + b;  // 必须支持加法
    a < b;  // 必须支持小于号

    // 类型要求：检查嵌套类型是否存在
    typename T::value_type;  // 必须存在这个类型

    // 复合要求：检查一个表达式，对它的返回值类型进行约束
    { a + b } -> std::convertible_to<T>;  // 加法返回值可以转化为 T
    // 这里的含义是，假设 a + b 的返回值为 U，概念 std::convertible_to<U, T> 必须成立
    { a.size() } -> std::integral;  // size() 返回值是整数

    // 嵌套要求：要求一个 requires 子句成立
    requires (sizeof(T) >= 16);  // 大小足够大
    requires (requires(T a, char b) { a + b; });  // 可以加上一个字符

/*
    此处的逻辑是，第一个 requires 是“子句”，括号里希望一个布尔值。
    第二个 requires 是“表达式”，正好会返回一个布尔值。
*/
};

// 例如，传入一个 std::string 就是合法的
auto f(MyConcept auto const &x) -> void {
    std::cout << x << '\n';
}


auto main() -> int {
    std::string s{"Hello"};
    f(s);
}