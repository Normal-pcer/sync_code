// Do not expand include
#include <bits/stdc++.h>
namespace Generator {
    #include "lib"
    #include "libs/range.hpp"
    using namespace lib;
    namespace Random {
        std::mt19937 random(std::random_device{}());

        unsigned randint(int x, int y) { return random() % (y - x + 1) + x; }
    }
    using namespace Random;

    void generate(std::fstream &out) {
        const int _T = 50;
        int T = _T;

        out << T << endl;
        for (auto _: range(T)) {
            const int _N = 10000, _M = 10000, _V = 10000;
            int N = _N;
            std::vector<int> a(N);
            std::transform(a.begin(), a.end(), a.begin(), lam(, randint(-_V, +_V)));
            int M = _M;

            out << N << std::endl;
            for (auto i: a)  out << i << ' ';
            out << std::endl << M << std::endl;
            for (auto i = 0; i < M; i++) {
                int x1 = randint(1, N), y1 = randint(1, N), x2 = randint(1, N), y2 = randint(1, N);
                if (x1 > y1)  std::swap(x1, y1);
                if (x2 > y2)  std::swap(x2, y2);
                if (not (x1 <= y1 and x1 <= x2 and y1 <= y2 and x2 <= y2)) {
                    i--;
                    continue;
                }
                out << x1 << ' ' << y1 << ' ' << x2 << ' ' << y2 << std::endl;
            }
        }
    }
}