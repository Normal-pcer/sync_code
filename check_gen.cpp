// Do not expand include
#include <bits/stdc++.h>
namespace Generator {
    #include "lib"
    #include "libs/range.hpp"
    #include "libs/fixed_int.hpp"
    using namespace lib;
    #include "randlib.hpp"
    using namespace Random;

    void generate(std::ostream &out) {
        i32 maxT = 1e5;
        auto T = maxT;

        out << T << endl;
        for (auto t = T; t --> 0; )
        while (true) {
            i64 constexpr maxN = 8, maxV = 30;

            auto N = maxN;  out << N << endl;
            for (auto _: range(N))  out << randint(1, maxV) << endl;
            break;
        }
    }
}

int main() {
    Generator::generate(std::cout);
    return 0;
}