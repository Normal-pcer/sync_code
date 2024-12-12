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
        const int _N = 2e5, _Q = 2e5, _V = 1e9;
        int N = _N, Q = _Q;
        std::vector<std::pair<int, int>> segs;
        while ((int)segs.size() != (int)N) {
            auto x = randint(1, _V), y = randint(1, _V);
            if (x > y)  std::swap(x, y);
            if (x == y)  continue;
            segs.emplace_back(x, y);
        }
        std::vector<std::pair<int, int>> queries;
        while ((int)queries.size() != Q) {
            auto x = randint(1, _V), y = randint(1, _V);
            if (x > y)  std::swap(x, y);
            if (x == y)  continue;
            queries.emplace_back(x, y);
        }
        out << N << endl;
        for (auto [x, y]: segs)  out << x << ' ' << y << endl;
        out << Q << endl;
        for (auto [x, y]: queries)  out << x << ' ' << y << endl;
    }
}