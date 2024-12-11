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
        // const ull _N = 3, _M = 3, _X = 10, _V = 1e15;
        const ull _N = 1e5, _M = 1e5, _X = 1e9, _V = 1e15;
        int N = _N, M = _M;
        std::vector<int> init(N);
        std::transform(init.begin(), init.end(), init.begin(), lam(,randint(1, _X)));

        out << N << ' ' << M << endl;
        for (auto i: init)  out << i << ' ';
        out << endl;
        for (auto i = 0; i < M; ) {
            auto op = randint(1, 2);
            out << op << ' ';
            if (op == 1) {
                int p = randint(1, N), x = randint(1, _X);
                out << p << ' ' << x << endl;
            } else {
                ull v = randll(1, _V);
                out << v << endl;
            }
            i++;
        }
    }
}