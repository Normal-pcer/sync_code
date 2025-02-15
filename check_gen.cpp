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
        const std::vector<int> primes{7, 11, 13, 17, 19};

        out << 5 << endl;
        for (auto _: range(5)) {
            auto p = primes.at(randint(0, primes.size() - 1));
            out << p << " " << randint(1, 7) << " " << randint(1, 7) << " " << randint(1, 7) << " " << randint(1, 7) << endl;
        }
    }
}

int main() {
    Generator::generate(std::cout);
    return 0;
}