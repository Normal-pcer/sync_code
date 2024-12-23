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
        const int _N = 4, _M = 3, _V = 5;
        while (true) {
            struct Edge {
                int x, y, val;
            };
            int N = _N;
            int M = randint(1, _M);
            int K = randint(1, N);
            std::vector<Edge> edges;
            for (auto i: range(2, N+1)) {
                auto to = randint(1, std::max(1, i - 2));
                edges.push_back({i, to, randint(1, _V)});
            }
            out << N << ' ' << M << ' ' << K << endl;
            for (auto [x, y, val]: edges) {
                out << x << ' ' << y << ' ' << val << endl;
            }
            break;
        }
    }
}

int main() {
    Generator::generate(std::cout);
    return 0;
}