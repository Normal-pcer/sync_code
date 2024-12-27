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
        const int _N = 100000, _M = 100000, _V = 600;
        int N = _N, M = _M;
        std::set<std::tuple<int, int, int>> points;
        while ((int)points.size() < N) {
            points.insert({randll(-_V, 0), randll(-_V, 0), randll(-_V, 0)});
        }
        std::vector vec(points.begin(), points.end());
        std::shuffle(vec.begin(), vec.end(), std::mt19937{std::random_device{}()});
        out << N << ' ' << M << endl;
        for (auto &[x, y, p]: vec)  out << x << ' ' << y << ' ' << p << endl;
        for (auto _: range(M)) {
            int x1 = randll(-_V, 0), x2 = randll(-_V, 0);
            int y1 = randll(-_V, 0), y2 = randll(-_V, 0);
            std::tie(x1, x2) = std::minmax({x1, x2});
            std::tie(y1, y2) = std::minmax({y1, y2});
            out << x1 << ' ' << y1 << ' ' << x2 << ' ' << y2 << endl;
        }
        out << std::endl;
    }
}

int main() {
    Generator::generate(std::cout);
    return 0;
}