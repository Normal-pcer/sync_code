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
        int T = 1;
        out << 1 << ' ' << T << endl;
        while (T --> 0) {
            const int _N = 100, _M = 5, _V = 3, _D = 1;
            // const int _N = 100, _M = 100, _V = 1'000'000'000, _D = 100'000'000;
            int N = _N, M = _M;
            int D = randint(1, _D), K = randint(1, N);
            
            using Task = std::tuple<int, int, int>;
            std::vector<Task> tasks;
            while ((int)tasks.size() < M) {
                int end = randint(1, N);
                int cnt = randint(1, end);
                int val = randint(1, _V);
                tasks.emplace_back(end, cnt, val);
            }
            out << N << ' ' << M << ' ' << K << ' ' << D << endl;
            for (auto [x, y, val]: tasks)  out << x << ' ' << y << ' ' << val << endl;
        }
    }
}

int main() {
    Generator::generate(std::cout);
    return 0;
}