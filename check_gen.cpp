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
        const int _N = 2, _L = 3;
        auto gen_word = [&](int len) -> std::string {
            std::string res;
            for (auto i = 0; i < len; i++) {
                res.push_back(static_cast<char>(randint('a', 'z')));
            }
            return res;
        };

        int N = _N, M = _N;
        out << N << endl;
        for (auto _: range(N))  out << gen_word(_L) << endl;
        out << M << endl;
        for (auto _: range(N))  out << gen_word(_L) << endl;
    }
}

int main() {
    Generator::generate(std::cout);
    return 0;
}