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
        while (true) {
            i32 const _N = 1e5, _M = 1e5, _V = 1e9;
            auto yes = [&]() -> void {
                i32 N = _N, M = _M;
                std::vector<i32> vec(N);
                for (auto &x: vec)  x = randint(1, _V);

                ranges::sort(vec);

                std::vector<std::pair<i32, i32>> ops;
                auto make_useless = [&]() -> void {
                    // debug  out << "Make Useless" << std::endl;
                    while (true) {
                        i32 x = randint(1, N), y = randint(1, N);
                        if (x == y)  continue;
                        ops.push_back({x, y}), ops.push_back({x, y});
                        break;
                    }
                };
                uz prev = 0;
                // 随机交换 M 次
                for (; static_cast<i32>(ops.size()) < M; ) {
                    i32 x = randint(1, N), y = randint(1, N);
                    if (x == y)  continue;
                    std::swap(vec[x - 1], vec[y - 1]);
                    ops.push_back({x, y});
                    if (randint(1, 100) <= (40 + (ops.size() - prev >= 3? 35: 0))) {
                        if (prev == ops.size())  continue;
                        std::vector tmp(ops.begin() + prev, ops.end());

                        ranges::reverse(tmp);
                        for (auto x: tmp)  ops.push_back(x), std::swap(vec[x.first - 1], vec[x.second - 1]);
                        prev = ops.size();
                        if (randint(1, 100) <= 30)  make_useless();
                    }
                }

                for (; static_cast<i32>(ops.size()) > M; ) {
                    auto [x, y] = ops.back();  ops.pop_back();
                    std::swap(vec[x - 1], vec[y - 1]);
                }

                out << N << " " << M << endl;
                for (auto x: vec)  out << x << " ";
                out << endl;

                for (auto [x, y]: ops | views::reverse)  out << x << " " << y << endl;
            };
            yes();
            break;
        }
    }
}

int main() {
    Generator::generate(std::cout);
    return 0;
}