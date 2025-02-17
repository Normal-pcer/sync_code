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
        int const _N = 3, _V = 7;
        auto N = _N;

        out << N << " " << randint(0, _V) << endl;
        for (auto _: range(N)) {
            out << randint(1, 3) << " " << randint(0, _V) << endl;
        }
    }
}

int main() {
    Generator::generate(std::cout);
    return 0;
}