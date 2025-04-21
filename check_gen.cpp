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
            i32 constexpr maxN = 5;
            auto n = maxN;
            out << 1 << ' ' << i64(1e18) << ' ' << n << endl;

            for (auto _: range(n)) {
                char ch = static_cast<char>(randint('a', 'd'));
                out << ch << ' ';
                auto len = randint(1, 3);
                for (auto _: range(len)) {
                    out << static_cast<char>(randint('a', 'd'));
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