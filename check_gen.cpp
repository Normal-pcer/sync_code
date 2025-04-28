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

    void generate(std::ostream &out) {
        i32 constexpr maxTTT = 2000;
        auto ttt = maxTTT;

        out << ttt << endl;
        while (ttt--)
        while (true) {
            // i32 constexpr maxN = 10;
            auto n = 8, m = 8;
            out << n << " " << m << endl;

            std::vector res(n, std::vector<i64>(m, 0));

            i64 constexpr maxT = 1e9;
            auto t = maxT;
            for (auto _ = t; _ --> 0; ) {
                auto i1 = randint(0, n - 2), j1 = randint(0, m - 2);
                auto i2 = i1, j2 = j1;
                if (randint(0, 1) == 0) i2++;
                else j2++;

                auto tmp = randint(1, static_cast<i32>(std::sqrt(t)));
                res[i1][j1] -= tmp;
                res[i2][j2] -= tmp;
                _ -= tmp;
            }

            auto min = ranges::min(res | views::transform(ranges::min));
            for (auto &line: res) {
                for (auto &item: line) {
                    item -= min, item++;
                    out << item << " ";
                }
                out << endl;
            }
            break;
        }
    }
}

int main() {
    Generator::generate(std::cout);
    return 0;
}