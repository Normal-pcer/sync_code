// Do not expand include
#include <bits/stdc++.h>
namespace Generator {
    #include "libs/range.hpp"
    using namespace lib;
    namespace Random {
        std::mt19937 random(std::random_device{}());

        unsigned randint(int x, int y) { return random() % (y - x + 1) + x; }
    }
    using namespace Random;

    void generate(std::fstream &out) {
        const int _N = 100000, _V = 1000000000, _K = 100;
        int N = randint(1, _N), K = randint(1, _K);
        out << N << ' ' << K << std::endl;
        for (auto _: range(N)) {
            auto x = randint(1, _V), y = randint(1, _V), z = randint(1, _V);
            out << x << ' ' << y << ' ' << z << std::endl;
        }
    }
}