#include <bits/stdc++.h>
namespace Generator {
    namespace Random {
        std::mt19937 random(std::random_device{}());

        unsigned randint(int x, int y) { return random() % (y - x + 1) + x; }
    }
    using namespace Random;

    void generate(std::fstream &out) {
        const int _N = 20, _M = 10;
        auto N = randint(_N, _N), M = randint(_M, _M);
        std::vector<int> init(N);
        for (auto &i: init)  i = randint(1, N);
        std::vector<std::pair<int, int>> modify(M);

        for (auto &[x, y]: modify)  x = randint(1, N), y = randint(1, N);
        std::sort(modify.begin(), modify.end());
        modify.erase(std::unique(modify.begin(), modify.end()), modify.end());
        std::shuffle(modify.begin(), modify.end(), random);

        M = modify.size();

        out << N << ' ' << M << std::endl;
        for (auto i: init)  out << i << ' ';
        out << std::endl;
        for (auto [x, y]: modify)  out << x << ' ' << y << std::endl;
    }
}