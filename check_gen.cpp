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
        const int _N = 5;
        int N = _N;
        std::vector<int> vec(N);
        std::iota(vec.begin(), vec.end(), 0);

        std::shuffle(vec.begin(), vec.end(), std::mt19937{std::random_device{}()});
        out << N << endl;
        for (auto x: vec)  out << x << " ";
        out << endl;
    }
}

int main() {
    Generator::generate(std::cout);
    return 0;
}