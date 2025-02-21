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
        int const _N = 10, _M = 3, _V = 5;
        auto N = _N, M = _M;

        out << N << " " << M << endl;
        for (auto _: range(N))  out << randint(1, _V) << " ";
        for (auto _: range(M)) {
            auto op = randint(1, 2);
            if (op == 1)  out << 1 << " " << randint(1, N) << " " << randint(1, _V) << endl;
            else {
                auto l = randint(1, N), r = randint(1, N);
                if (l > r)  std::swap(l, r);
                out << 2 << " " << l << " " << r << endl;
            }
        }
    }
}

int main() {
    Generator::generate(std::cout);
    return 0;
}