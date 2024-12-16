// Do not expand include
#include <bits/stdc++.h>
namespace Generator {
    #include "lib"
    #include "libs/range.hpp"
    using namespace lib;
    namespace Random {
        std::mt19937 random(std::random_device{}());
        std::mt19937_64 random64(std::random_device{}());

        unsigned randint(int x, int y) { return random() % (y - x + 1) + x; }
        ull randll(ll x, ll y) { return random64() % (y - x + 1) + x; }
    }
    using namespace Random;

    void generate(std::fstream &out) {
        const int _N = 5, _V = 10;
        int N = _N;
        std::vector<std::tuple<int, int, int, int>> rect;
        for (; static_cast<int>(rect.size()) != N; ) {
            int x1 = randint(1, _V), y1 = randint(1, _V), x2 = randint(1, _V), y2 = randint(1, _V);
            if (x1 == x2)  continue;
            if (y1 == y2)  continue;
            if (x1 > x2)  std::swap(x1, x2);
            if (y1 > y2)  std::swap(y1, y2);
            rect.push_back({x1, y1, x2, y2});
        }

        out << N << endl;
        for (auto [a, b, c, d]: rect) {
            out << a << ' ' << b << ' ' << c << ' ' << d << endl;
        }
    }
}