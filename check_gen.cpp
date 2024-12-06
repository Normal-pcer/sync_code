#include <bits/stdc++.h>
namespace Generator {
    namespace Random {
        std::mt19937 random(std::random_device{}());

        unsigned randint(int x, int y) { return random() % (y - x + 1) + x; }
    }
    using namespace Random;

    void generate(std::fstream &out) {
        const int _N = 10, _V = 5;
        int N = randint(1, _N);
        std::vector<std::pair<int, int>> pairs(N);
        for (auto &[x, y]: pairs)  x = randint(1, _V), y = randint(1, _V);

        out << N << std::endl;
        for (auto [x, y]: pairs)  out << x << ' ' << y << std::endl;
    }
}