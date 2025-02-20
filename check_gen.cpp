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
        int const _N = 5, _M = 5;
        auto N = _N, M = _M;

        out << N << " " << M << endl;
        for (auto _: range(N))  out << randint(-100, 100) << " ";
        out << endl;
        for (auto _: range(M)) {
            auto op = randint(0, 3);
            auto x = randint(1, N);
            auto y = randint(1, N);
            auto z = randint(-100, 100);

            if (op == 0)  out << op << " " << x << " " << y << endl;
            else  out << op << " " << x << " " << y << " " << z << endl;
        }
    }
}

int main() {
    Generator::generate(std::cout);
    return 0;
}