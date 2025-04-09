#include <chrono>
#include <iostream>
#include <functional>

volatile int sink; // 防止结果被优化
constexpr int RUN_TIMES = 50; // 增加运行次数
constexpr int FIB_N = 37;     // 增加计算深度

// 显式传递自身参数
void test_lambda_self() {
    auto fib = [](int n, auto&& self) -> int {
        if (n <= 1) return n;
        return self(n-1, self) + self(n-2, self);
    };

    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < RUN_TIMES; ++i) {
        sink = fib(FIB_N, fib);
    }
    auto end = std::chrono::steady_clock::now();
    std::cout << "Lambda (self): \t" 
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() 
              << " ms\n";
}

// std::function
void test_std_function() {
    std::function<int(int)> fib;
    fib = [&](int n) {
        if (n <= 1) return n;
        return fib(n-1) + fib(n-2);
    };

    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < RUN_TIMES; ++i) {
        sink = fib(FIB_N);
    }
    auto end = std::chrono::steady_clock::now();
    std::cout << "std::function: \t\t" 
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() 
              << " ms\n";
}

// 普通函数
int fib_normal(int n) {
    if (n <= 1) return n;
    return fib_normal(n-1) + fib_normal(n-2);
}
void test_normal_function() {
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < RUN_TIMES; ++i) {
        sink = fib_normal(FIB_N);
    }
    auto end = std::chrono::steady_clock::now();
    std::cout << "Normal function: \t" 
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() 
              << " ms\n";
}

// 仿函数
struct FibFunctor {
    int operator()(int n) const {
        if (n <= 1) return n;
        return (*this)(n-1) + (*this)(n-2);
    }
};
void test_functor() {
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < RUN_TIMES; ++i) {
        sink = FibFunctor{}(FIB_N);
    }
    auto end = std::chrono::steady_clock::now();
    std::cout << "Functor: \t\t" 
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() 
              << " ms\n";
}

int main() {
    test_lambda_self();
    test_std_function();
    test_normal_function();
    test_functor();
}