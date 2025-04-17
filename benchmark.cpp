#include <bits/stdc++.h>
#include "./libs/fixed_int.hpp"

i32 constexpr n = 1e4;
i32 constexpr len = 1e3;

using cstr = std::array<char, len>;

std::vector<cstr> a(n);

std::mt19937 rng{std::random_device{}()};

template <>
struct std::hash<cstr> {
    auto operator() (cstr const &arr) const -> std::size_t {
        std::size_t res = 0;
        std::size_t constexpr base = 31;
        for (auto ch: arr) res = res * base + ch;
        return res;
    }
};

auto init() -> void {
    std::shuffle(a.begin(), a.end(), rng);
}

auto f() -> u32 {
    return 0;
    // u32 ans = 0;
    // u32 constexpr base = 131;
    std::sort(a.begin(), a.end());

    // for (auto const &s: a) {
    //     ans = ans * base + static_cast<u32>(std::hash<cstr>{}(s));
    // }
    // return ans;
}

auto g() -> u32 {
    // u32 ans = 0;
    // u32 constexpr base = 131;
    
    static std::vector<i32> index(n);
    std::iota(index.begin(), index.end(), 0);
    std::sort(index.begin(), index.end(), [&](auto x, auto y) {
        return a[x] < a[y];
    });
    // std::vector<cstr> tmp(n);
    // for (i32 i = 0; i < n; i++) {
        // tmp[i] = a[index[i]];
    // }
    // a = tmp;

    // for (auto const &i: index) {
    //     ans = ans * base + static_cast<u32>(std::hash<cstr>{}(a[i]));
    // }
    // return ans;
    return 0;
}

template <typename Func>
auto benchmark(std::string const &name, Func &&func, i32 warmup = 5, i32 run = 100) -> void {
    for (i32 i = 0; i < warmup; i++) {
        init();
        func();
    }

    i64 nanosec = 0;
    volatile u32 ans = 0;
    for (i32 i = 0; i < run; i++) {
        init();
        auto start = std::chrono::system_clock::now();
        ans += func();
        auto end = std::chrono::system_clock::now();
        nanosec += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    }

    auto avg = static_cast<double>(nanosec) * 1e-6;
    std::cout << std::format("Benchmark `{}`: time = {:.3f}ms, ans = {}", name, avg, ans) << std::endl;
}

int main() {
    for (i32 i = 0; i < n; i++) {
        // a[i].reserve(len);
        for (i32 j = 0; j < len; j++) {
            a[i][j] = std::uniform_int_distribution<char>{'a', 'z'}(rng);
        }
    }

    benchmark("f()", f, 5, 100);
    benchmark("g()", g, 5, 100);
    return 0;
}
