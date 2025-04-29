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
        while (true) {
            i32 constexpr maxN = 8, maxV = 10;
            auto n = maxN;

            out << n << endl;
            for (auto _: range(n)) {
                out << randint(1, maxV) << ' ';
            }
            out << endl;
            break;
        }
    }
}

int main() {
    Generator::generate(std::cout);
    return 0;
}