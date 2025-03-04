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

        int randint(int x, int y) { return std::uniform_int_distribution<int>{x, y}(random); }
        long long randll(ll x, ll y) { return std::uniform_int_distribution<long long>{x, y}(random64); }
    }
    using namespace Random;

    void generate(std::ostream &out) {
        i32 constexpr _T = 1;
        i32 T = _T;
        out << 0 << " " << T << endl;
        while (true) {
            i32 constexpr _N = 5, _V = 15;

            i32 N = _N;
            std::vector<i32> ab(N * 2);
            std::set<i32> s;
            for (auto &x: ab) {
                while (true) {
                    i32 tmp = randint(1, _V);
                    if (s.contains(tmp))  continue;
                    s.insert(tmp);
                    x = tmp;
                    break;
                }
            }

            ranges::sort(ab);
            for (i32 i = 0; i < N - 1; i++) {
                if (randint(1, 2) == 1)  std::swap(ab[i * 2 + 1], ab[i * 2 + 2]);
            }

            out << N << endl;
            auto maxTime = static_cast<i64>(N) * _V * 3 / 5;
            for (i32 i = 0; i < N; i++) {
                out << ab[i * 2] << " " << ab[i * 2 + 1] << " " << randint(1, maxTime) << endl;
            }
            break;
        }
    }
}

int main() {
    Generator::generate(std::cout);
    return 0;
}