#include <chrono>
#include <cmath>
#include <cstdint>
#include <print>
#include <string_view>
#include <random>

std::mt19937 rng{std::random_device{}()};

using i32 = std::int32_t;
using u32 = std::uint32_t;

namespace ranges = std::ranges;
namespace chrono = std::chrono;

i32 constexpr testCount = 1e6;
i32 constexpr repeat = 500;

auto benchmark(std::string_view name, auto &&f) {
    auto begin = chrono::system_clock::now();
    
    u32 res = 0;
    std::vector<i32> tests(testCount);
    std::uniform_int_distribution dist{0, i32(1e9)};

    for (auto &x: tests) {
        x = dist(rng);
    }
    
    for (i32 i = 0; i != repeat; i++) {
        for (auto x: tests) {
            res += f(x);
        }
    }

    auto end = chrono::system_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - begin);
    
    std::println("Test {}: res = {}, duration = {}.", 
        name, res, duration.count());
}

auto f1(i32 x) -> i32 {
    return static_cast<i32>(std::sqrt(x));
}

int f2(unsigned n) {
    if (n < 2) return n;
    unsigned x = 1U << ((32 - __builtin_clz(n)) / 2); // 位运算估算初始值
    while (true) {
        unsigned y = (x + n / x) / 2; // 牛顿迭代
        if (y >= x) break; // 收敛时停止
        x = y;
    }
    // 确保结果向下取整
    return static_cast<int>((x * x > n) ? x - 1 : x);
}

#include <immintrin.h>

int f3(unsigned n) {
    if (n < 2) return n;
    float f = static_cast<float>(n);
    int r = _mm_cvtss_si32(_mm_sqrt_ss(_mm_set_ss(f)));
    // 处理浮点舍入误差
    return (static_cast<unsigned>(r) * r > n) ? r - 1 : r;
}

int f4(unsigned n) {
    if (n < 2) return n;
    // 初始估计 (魔法数优化)
    unsigned r = (0x1fbd1df5 + (n >> 1)) >> 16;
    r = (r + n / r) >> 1;  // 一次牛顿迭代
    r = (r + n / r) >> 1;  // 一次牛顿迭代
    // 处理误差
    return (r * r > n) ? r - 1 : r;
}

auto main() -> int {
    benchmark("std::sqrt", f1);
    benchmark("newton", f2);
    benchmark("hw_sqrt", f3);
    benchmark("magic", f4);
}