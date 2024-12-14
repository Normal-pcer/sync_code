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
        const int _N = 4, _M = 4, _V = 5;
        int N = _N, M = _M;
        std::vector<int> pos(N);
        std::generate(pos.begin(), pos.end(), lam(randint(1, _V)));
        std::vector<std::tuple<int, int, int>> segs;
        std::set<std::pair<int, int>> unique;
        for (; segs.size() < (size_t)M;) {
            auto l = randint(1, N), r = randint(1, N), val = randint(1, _V);
            if (unique.find({l, r}) != unique.end())  continue;
            if (l >= r)  std::swap(l, r);
            unique.insert({l, r});
            segs.push_back({l, r, val});
        }
        out << N << ' ' << M << std::endl;
        for (auto i: pos)  out << i << ' ';
        out << std::endl;
        for (auto [l, r, val]: segs) {
            out << l << ' ' << r << ' ' << val << std::endl;
        }
    }
}