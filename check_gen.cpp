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
        const int _N = 5, _V = 3, _M = 6;
        
        while (true) {
            auto N = _N;
            std::vector<std::pair<int, int>> c(N);
            auto sum = 0;
            for (auto &[x, y]: c)  x = randint(0, _V), y = randint(0, _V), sum += x + y;
            if (sum > _M)  continue;
            auto M = sum;
            out << N << " " << M << endl;
            for (auto [x, y]: c)  out << x << " " << y << endl;
            break;
        }
    }
}

int main() {
    Generator::generate(std::cout);
    return 0;
}