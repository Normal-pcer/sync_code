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
        while (true) {
            i32 constexpr maxN = 3, maxV = 5;
            auto N = maxN;
            out << N << endl;
            for (auto _: range(N))  out << randint(1, maxV) << " ";
            out << endl;
            break;
        }
    }
}

int main() {
    Generator::generate(std::cout);
    return 0;
}