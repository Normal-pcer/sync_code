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
        const int _N = 3e4, _Q = 3e4, _V = 1023;
        int N = _N, Q = _Q;
        out << N << " " << Q << endl;
        std::vector<std::vector<int>> G(N + 1);
        for (auto i: range(2, N + 1)) {
            auto prev = randint(1, i - 1);
            out << prev << " " << i << " " << randint(1, _V) << endl;
            G[i].push_back(prev), G[prev].push_back(i);
        }
        int i = 0;
        while (i != Q) {
            int op = randint(1, 2);
            if (op == 1) {
                int x = randint(1, N), y = randint(1, N);
                if (x == y)  continue;
                out << op << " " << x << " " << y << endl;
            } else {
                int x = randint(1, N);
                int y = G[x].at(randint(0, G[x].size() - 1));
                int z = randint(1, _V);
                out << op << " " << x << " " << y << " " << z << endl;
            }
            i++;
        }
    }
}

int main() {
    Generator::generate(std::cout);
    return 0;
}