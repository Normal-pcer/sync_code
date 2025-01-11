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
        const int _V = 5;
        int N = _N;
        out << N << endl;
        for (auto _: range(N)) {
            int x, y, z;
            x = randint(1, _V);
            y = randint(1, _V);
            z = randint(1, _V);
            out << x << ' ' << y << ' ' << z << endl;
        }
    }
}

int main() {
    Generator::generate(std::cout);
    return 0;
}