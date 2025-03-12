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
            i64 constexpr maxN = 100, maxM = 3000, maxQ = 100, maxV = 4294967295U, maxT = 4294967295U;
            auto N = maxN, M = maxM, Q = maxQ;

            out << N << " " << M << " " << Q << endl;
            for (auto _: range(N))  out << randint(1, maxV) << " ";
            out << endl;

            std::set<std::pair<i32, i32>> edges;
            for (auto _: range(M)) {
                while (true) {
                    auto x = randint(1, N - 1);
                    auto y = randint(x + 1, N);
                    if (edges.contains({x, y}))  continue;
                    out << x << " " << y << endl;
                    edges.insert({x, y});
                    break;
                }
            }
            for (auto _: range(Q))  out << randint(1, maxT) << endl;
            break;
        }
    }
}

int main() {
    Generator::generate(std::cout);
    return 0;
}