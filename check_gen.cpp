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
            i32 constexpr maxN = 5, maxQ = 4, maxPos = 1e9, maxLen = 1e9;
            auto N = maxN, Q = maxQ;

            out << N << " " << Q << endl;
            std::vector<i32> all_x, all_y;
            auto all_x_size = randint(1, N - 1);
            auto all_y_size = N - all_x_size;
            all_x = sample(1, maxPos, all_x_size);
            all_y = sample(1, maxPos, all_y_size);
            for (auto x: all_x)  out << "V " << x << endl;
            for (auto y: all_y)  out << "H " << y << endl;

            for (auto _: range(Q)) {
                auto on_x = !!randint(0, 1);
                i32 x, y;
                if (on_x)  x = all_x[randrange(0, all_x.size())], y = randint(0, maxPos);
                else  y = all_y[randrange(0, all_y.size())], x = randint(0, maxPos);
                auto len = randint(1, maxLen);
                out << x << " " << y << " " << len << endl;
            }
            break;
        }
    }
}

int main() {
    Generator::generate(std::cout);
    return 0;
}