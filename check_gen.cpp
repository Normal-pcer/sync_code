// Do not expand include
#include <bits/stdc++.h>
namespace Generator {
    #include "lib"
    #include "libs/range.hpp"
    #include "libs/fixed_int.hpp"
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
        while (true) {
            i32 const _N = 4;
            i32 N = _N;
            std::vector<i32> r(N), c(N);
            ranges::iota(r, 0);
            ranges::iota(c, 0);

            std::mt19937 rng{std::random_device{}()};
            ranges::shuffle(r, rng);
            ranges::shuffle(c, rng);
            std::vector<std::pair<i32, i32>> p(N);
            for (i32 i = 0; i < N; i++) {
                p[r[i]].first = i + 1;
                p[c[i]].second = i + 1;
            }
            out << N << endl;
            for (auto [x, y]: p)  out << x << " " << y << endl;
            break;
        }
    }
}

int main() {
    Generator::generate(std::cout);
    return 0;
}