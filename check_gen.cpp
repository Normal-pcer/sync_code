// Do not expand include
#ifndef INCLUDING_GENERATOR
#include <bits/stdc++.h>
#endif
namespace Generator {
    char constexpr endl = '\n';
    namespace ranges = std::ranges;
    namespace views = std::ranges::views;
#include "libs/range.hpp"
#include "libs/fixed_int.hpp"
    using namespace lib;
#include "randlib.hpp"
    using namespace Random;

#define cout  // 避免误用 std::cout 输出
    void generate(std::ostream &out) {
        while (true) {
            i32 constexpr maxN = 1000, maxV = 100;

            auto min = randint(1, maxV), max = randint(1, maxV);
            if (min > max) std::swap(min, max);
            if (randint(1, 10) == 1) min = max;

            auto n = randint(1, maxN);
            out << n << ' ' << max << ' ' << min << endl;

            std::vector<i32> a(n);
            for (auto &x: a) x = randint(1, maxV);

            for (auto x: a) out << x << ' ';
            out << endl;

            break;
        }
    }
#undef cout
}

int main() {
    Generator::generate(std::cout);
    return 0;
}