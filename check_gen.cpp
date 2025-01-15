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
        const int _N = 3, _Q = 3, _V = 5, _C = 2;
        int N = _N;
        out << N << " ";
        int Q = _Q;  out << Q << endl;
        std::vector<std::vector<int>> G(N + 1);
        for (auto _: range(N)) {
            out << randint(1, _V) << " " << randint(1, _C) << endl;
        }
        for (auto i: range(2, N + 1)) {
            auto prev = randint(1, i - 1);
            out << prev << " " << i << endl;
            G[i].push_back(prev), G[prev].push_back(i);
        }
        int i = 0;
        while (i != Q) {
            int op = randint(1, 4);
            int x = randint(1, N), y = randint(1, N);
            if (x == y)  continue;
            if (op == 1) {
                out << "CC" << " " << x << " " << randint(1, _C) << endl;
            } else if (op == 2) {
                out << "CW" << " " << x << " " << randint(1, _V) << endl;
            } else if (op == 3) {
                out << "QS" << " " << x << " " << y << endl;
            } else {
                out << "QM" << " " << x << " " << y << endl;
            }
            i++;
        }
    }
}

int main() {
    Generator::generate(std::cout);
    return 0;
}