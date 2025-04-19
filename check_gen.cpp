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
            i32 constexpr maxN = 10;
            auto n1 = maxN;
            out << n1 << endl;
            for (i32 i = 2; i <= n1; i++) {
                auto prev = randint(1, i - 1);
                out << prev << " " << i << endl;
            }

            auto n2 = maxN;
            out << n2 << endl;
            for (i32 i = 2; i <= n2; i++) {
                auto prev = randint(1, i - 1);
                out << prev << " " << i << endl;
            }
            break;
        }
    }
}

int main() {
    Generator::generate(std::cout);
    return 0;
}