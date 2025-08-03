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
            i32 constexpr maxN = 100, maxV = 100, maxT = 1e5;

            auto n = randint(1, maxN), t = randint(1, maxT);
            out << n << ' ' << t << endl;
            for (auto _: range(n)) {
                out << randint(1, maxV) << ' ';
            }
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