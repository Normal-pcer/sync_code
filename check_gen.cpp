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

#define cout  // 避免误用 cout 输出
    void generate(std::ostream &out) {
        i32 constexpr maxN = 10, maxM = 10, maxVal = 10;
        auto n = maxN, m = maxM;
        out << n << ' ' << m << endl;

        for (auto _ = n; _ --> 0; ) out << randint(1, maxVal) << ' ';
        out << endl;

        for (i32 p = 2; p <= n; ++p) {
            auto prev = randint(1, p - 1);
            out << prev << ' ' << p << endl;
        }

        auto max_lim = static_cast<i32>(std::sqrt(n));
        for (auto _ = m; _ --> 0; ) {
            auto op = randint(0, 1);

            if (op == 0) {
                auto lim = randint(1, max_lim);
                out << 0 << ' ' << randint(1, n) << ' ' << lim << endl;
            } else {
                out << 1 << ' ' << randint(1, n) << ' ' << randint(1, maxVal) << endl;
            }
        }
    }
#undef cout
}

int main() {
    Generator::generate(std::cout);
    return 0;
}