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
        const int _N = 5, _V = 8;
        int N = _N;
        std::vector<int> vec(N, _V);
        ranges::transform(vec, vec.begin(), rand_max);
        ranges::sort(vec);
        int W = randint(ranges::max(vec), ranges::max(vec) << 1);
        out << W << ' ' << N << endl;
        for (auto i: vec)  out << i << endl;
    }
}

int main() {
    Generator::generate(std::cout);
    return 0;
}