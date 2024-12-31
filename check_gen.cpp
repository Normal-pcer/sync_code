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
        const int _N = 5, _M = 7, _V = 10;
        auto N = _N, M = _M;
        std::vector<int> price(N+1);
        for (auto &i: price)  i = randint(1, _V);
        out << N << ' ' << M << endl;
        for (auto i: price | views::drop(1))  out << i << ' ';
        out << endl;
        std::set<std::pair<int, int>> set;
        std::vector<std::tuple<int, int, int>> edges;
        for (; edges.size() != (size_t)M; ) {
            auto x = randint(1, N), y = randint(1, N);
            if (x == y)  continue;
            if (x > y)  std::swap(x, y);
            if (set.contains({x, y}))  continue;
            auto flag = randint(1, 2);
            edges.push_back({x, y, flag});
            set.insert({x, y});
        }
        ranges::shuffle(edges, random);
        for (auto [x, y, flag]: edges)  out << x << ' ' << y << ' ' << flag << endl;
    }
}

int main() {
    Generator::generate(std::cout);
    return 0;
}