// Do not expand include
#include <bits/stdc++.h>
namespace Generator {
    #include "lib"
    #include "libs/range.hpp"
    using namespace lib;
    namespace Random {
        std::mt19937 random(std::random_device{}());
        std::mt19937_64 random64(std::random_device{}());

        int randint(int x, int y) { return random() % (y - x + 1) + x; }
        long long randll(ll x, ll y) { return random64() % (y - x + 1) + x; }
    }
    using namespace Random;

    void generate(std::fstream &out) {
        const int _N = 5, _K = 10, _V = 5;
        int N = _N, M = _N, K = _K;
        struct Point {
            int x, y, cnt;
        };

        std::vector<Point> luo;
        std::set<std::pair<int, int>> set;
        while ((int)luo.size() < K) {
            auto x = randint(1, N), y = randint(1, M), cnt = randint(1, _V);
            if (x == N and y == M)  continue;
            if (set.contains({x, y}))  continue;
            luo.push_back({x, y, cnt}), set.insert({x, y});
        }
        
        out << N << ' ' << M << ' ' << K << endl;
        for (auto [x, y, cnt]: luo)  out << x << ' ' << y << ' ' << cnt << endl;
    }
}