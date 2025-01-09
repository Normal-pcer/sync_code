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
        const int _N = 3e4, _V = 3e4, _Q = 2e5;
        int N = _N;
        out << N << endl;
        std::vector<std::pair<int, int>> edges;
        for (auto i: range(2, N+1)) {
            auto x = randint(1, i-1);
            edges.push_back({i, x});
        }
        ranges::shuffle(edges, random);
        for (auto [x, y]: edges)  out << x << ' ' << y << endl;
        std::vector<int> val(N);
        for (auto &x: val)  x = randint(-_V, +_V);
        for (auto x: val)  out << x << ' ';
        out << endl;

        int Q = _Q;
        out << Q << endl;
        for (auto _: range(Q)) {
            auto op = randint(1, 3);
            if (op == 1) {
                out << "QMAX" << " ";
                auto x = randint(1, N), y = randint(1, N);
                out << x << ' ' << y << endl;
            } else if (op == 2) {
                out << "QSUM" << " ";
                auto x = randint(1, N), y = randint(1, N);
                out << x << ' ' << y << endl;
            } else {
                out << "CHANGE" << " ";
                auto x = randint(1, N), y = randint(-_V, +_V);
                out << x << " " << y << endl;
            }
        }
    }
}

int main() {
    Generator::generate(std::cout);
    return 0;
}