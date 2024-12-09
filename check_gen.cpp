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
        const int _N = 2;
        int N = _N;
        std::vector<int> a(N), b(N), c(N), d(N);
        for (auto &i: a)  i = randint(0, 1);
        for (auto &i: b)  i = randint(0, 1);
        for (auto &i: c)  i = randint(0, 1);
        for (auto &i: d)  i = randint(0, 1);

        out << 1 << std::endl;
        out << N << std::endl;
        for (auto i: a)  out << i;
        out << std::endl;
        for (auto i: b)  out << i;
        out << std::endl;
        for (auto i: c)  out << i;
        out << std::endl;
        for (auto i: d)  out << i;
        out << std::endl;
    }
}