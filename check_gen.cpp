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
        long long randll(ll x, ll y) { return random64() % (y - x + 1) + x; }
    }
    using namespace Random;

    void generate(std::ostream &out) {
        const int _N = 100, _V = 1000000000;
        int N = _N;

        std::vector<int> a(N);
        for (auto &i: a)  i = randint(1, _V);
        out << N << endl;
        for (auto i: a)  out << i << ' ';
        out << endl;
    }
}

int main() {
    Generator::generate(std::cout);
    return 0;
}