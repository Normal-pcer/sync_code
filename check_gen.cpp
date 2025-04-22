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
            i32 constexpr maxN = 10, maxM = 6;
            auto n = maxN, m = maxM;

            for (auto _: range(n)) { out << static_cast<char>(randint('A', 'B')); } out << endl;
            for (auto _: range(m)) { out << static_cast<char>(randint('A', 'B')); } out << endl;
            break;
        }
    }
}

int main() {
    Generator::generate(std::cout);
    return 0;
}