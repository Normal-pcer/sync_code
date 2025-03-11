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
            i32 constexpr maxN = 4, maxM = 4, maxK = 4, maxF = 2, maxV = 5;
            auto N = maxN, M = randint(1, maxM), K = randint(1, maxK);
            
            out << N << ' ' << M << " " << randint(1, N) << " " << randint(1, N) << " " << K << endl;
            for (auto _: range(M))  out << randint(1, N) << ' ' << randint(1, N) << " " << randint(1, maxV) << endl;
            auto f = randint(1, maxF);
            out << f << endl;
            for (auto _: range(f)) {
                auto cnt = randint(2, 4);
                auto x = sample(1, N + 1, cnt);
                out << cnt << " " << x << endl;
            }
            break;
        }
    }
}

int main() {
    Generator::generate(std::cout);
    return 0;
}