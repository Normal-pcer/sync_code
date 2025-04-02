#include <bits/stdc++.h>
bool DEBUG_MODE = false;
#define debug if (DEBUG_MODE)
#define never if constexpr (false)
template <typename T> inline auto chkMax(T &base, const T &cmp) -> T & { return (base = std::max(base, cmp)); }
template <typename T> inline auto chkMin(T &base, const T &cmp) -> T & { return (base = std::min(base, cmp)); }
#define __lambda_1(expr) [&]() { return expr; }
#define __lambda_2(a, expr) [&](auto a) { return expr; }
#define __lambda_3(a, b, expr) [&](auto a, auto b) { return expr; }
#define __lambda_4(a, b, c, expr) [&](auto a, auto b, auto c) { return expr; }
#define __lambda_overload(a, b, c, d, e, ...) __lambda_##e
#define lambda(...) __lambda_overload(__VA_ARGS__, 4, 3, 2, 1)(__VA_ARGS__)
#define lam lambda
char constexpr endl = '\n';
#include "./libs/fixed_int.hpp"

#define FILENAME "joioji"

/*

*/
namespace Solution_6065476225470371 {
    auto solve() -> void {
        i32 N; std::cin >> N;
        std::string s; std::cin >> s;

        auto countPre = [&](char x, auto out_it) -> void {
            i32 cnt = 0;
            for (auto ch: s) {
                if (ch == x) cnt++;
                *out_it++ = cnt;
            }
        };

        std::vector<std::vector<i32>> cnt_ps(3, std::vector<i32>(N));
        countPre('J', cnt_ps[0].begin());
        countPre('O', cnt_ps[1].begin());
        countPre('I', cnt_ps[2].begin());

        // 用 cnt_ps[0][i] - cnt_ps[1][i] - cnt_ps[2][i] 作为特征
        std::map<std::pair<i32, i32>, std::vector<i32>> map;
        for (i32 i = 0; i < N; i++) {
            auto iden = std::make_pair(cnt_ps[0][i] - cnt_ps[1][i], cnt_ps[1][i] - cnt_ps[2][i]);
            map[iden].push_back(i);
        }
        map[{0, 0}].push_back(-1);

        for (auto &item: map) {
            auto &line = item.second;
            std::sort(line.begin(), line.end());
        }

        i32 ans = 0;
        for (i32 i = -1; i < N; i++) {
            auto iden = i == -1? std::make_pair(0, 0): std::make_pair(cnt_ps[0][i] - cnt_ps[1][i], cnt_ps[1][i] - cnt_ps[2][i]);
            auto it = map.find(iden);
            if (it != map.end()) {
                auto const &nums = it->second;
                if (not nums.empty()) {
                    chkMax(ans, nums.back() - i);
                }
            }
        }
        std::cout << ans << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::freopen(FILENAME ".in", "r", stdin);
    std::freopen(FILENAME ".out", "w", stdout);

    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
    Solution_6065476225470371::solve();
    return 0;
}
