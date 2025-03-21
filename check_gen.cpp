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
            i32 constexpr maxN = 3000, maxK = 50;
            auto N = maxN, K = maxK;
            out << N << " " << K << endl;
            for (auto _: range(N)) out << randint(1, K) << " ";
            out << endl;
            
            for (auto _: range(K)) {
                for (auto _: range(K)) {
                    out << randint(0, 1);
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