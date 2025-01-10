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
        int rand_max(int y) { return randint(1, y); }
        long long randll(ll x, ll y) { return random64() % (y - x + 1) + x; }
    }
    using namespace Random;

    void generate(std::ostream &out) {
        const int _N = 4;
        const int _M = 2;
        const int _V = 5;
        int N = _N, M = _M;
        out << N << ' ' << M << endl;
        for (auto i: range(2, N+1)) {
            auto x =  randint(1, i-1);
            out << x << ' ' << i << endl;
        }
        std::vector<int> w(N);
        for (auto &x: w)  x = randint(1, _V);
        for (auto x: w)  out << x << ' ';
        out << endl;

        out << randint(1, N) << endl;
        for (; M != 0; ) {
            int op = randint(1, 3);
            if (op == 1) {
                out << 1 << ' ' << randint(1, N) << endl;
            } else if (op == 2) {
                auto x = randint(1, N);
                auto y = randint(1, N);
                if (x == y)  continue;
                out << 2 << ' ' << x << ' ' << y << ' ' << randint(1, _V) << endl;
            } else {
                out << 3 << ' ' << randint(1, N) << endl;
            }
            M--;
        }
    }
}

int main() {
    Generator::generate(std::cout);
    return 0;
}