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
        const int _N = 5, _Q = 1, _V = 10;
        int N = _N, Q = _Q;
        std::vector<int> vec(N);
        out << N << " " << Q << endl;
        for (auto &x: vec)  x = randint(1, _V), out << x << " ";
        out << endl;

        for (auto _: range(Q)) {
            auto K = randint(1, _V), M = randint(1, N);
            out << K << " " << M << endl;
        }
    }
}

int main() {
    Generator::generate(std::cout);
    return 0;
}