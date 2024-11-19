#include <bits/stdc++.h>
#pragma GCC optimize(3)
#pragma GCC optimize("inline")

using ll = long long;

namespace Solution {

    constexpr inline auto pow(ll a, ll b, int p) {
        auto res = 1LL;
        for (; b; b >>= 1, a = a * a % p) if (b&1) res = res * a % p;
        return res;
    }

    void solve() {
        std::ios::sync_with_stdio(false), std::cin.tie(nullptr), std::cout.tie(nullptr);
#ifndef ONLINE_JUDGE
        std::freopen("T1.in", "r", stdin);
        std::freopen("T1.out", "w", stdout);
#endif
        int M, Q, P;
        std::cin >> M >> Q >> P;
        std::vector<std::pair<int, int>> f;  // 多项式 f；每一项为系数和指数

        for (auto i = 1; i <= M; i++) {
            int x, y;
            std::cin >> x >> y;
            f.push_back({x, y});
        }

        auto calc = [&](int x) -> int {
            auto res = 0;
            for (auto [a, b]: f) {
                res += (long long)a * pow(x, b, P) % P;
                res %= P;
            }
            return res;
        };

        auto recursion = [&](int x, int times) -> int {
            auto res = x;
            std::map<int, int> map;
            std::vector<int> vec;
            map.insert({x, 0});
            vec.push_back(0);
            for (auto _ = 1; _ <= times; _++) {
                res = calc(res);
                vec.push_back(res);
                if (map.find(x) != map.end()) {
                    map.insert({x, 0});
                } else {
                    auto prev = map.at(x);
                    auto len = x - prev;  // 循环节
                    return vec.at(prev + (x - prev) % len);  // 返回记录中的值
                }
            }
            return res;
        };

        for (auto _ = 1; _ <= Q; _++) {
            int x, y;
            std::cin >> x >> y;
            std::cout << recursion(x, y) << std::endl;
        }
    }
}

int main() {
    Solution::solve();
    return 0;
}