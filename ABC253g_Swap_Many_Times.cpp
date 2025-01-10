/**
 * @link https://www.luogu.com.cn/problem/AT_abc253_g
 */

#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_1751209703359097 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;  std::cin >> N;
        ll L, R;  std::cin >> L >> R;
        std::vector<int> seq(N);
        ranges::copy(range(N), seq.begin());

        // 数对的下标从 0 开始
        // f(x)：第 i 组（以 i 开头）的开始下标
        auto f = [&](int x) -> ll { return static_cast<ll>(N + N - x - 1) * x / 2; };

        // 二分查找 L 和 R 在哪一组
        L--, R--;
        auto l_group = *(ranges::upper_bound(seq, L, lam(x, y, x < f(y))) - 1);
        auto r_group = *(ranges::upper_bound(seq, R, lam(x, y, x < f(y))) - 1);

        if (l_group == r_group) {
            auto sec_l = L - f(l_group) + l_group + 1;
            auto sec_r = R - f(r_group) + r_group + 1;
            for (auto i: range<ll>(sec_l, sec_r + 1)) {
                // debug  std::cout << std::format("{} swap [{}] [{}]", __LINE__, i, l_group) << std::endl;
                std::swap(seq[i], seq[l_group]);
            }
            goto egg;
        }
        {  // 执行 L 到下一组
            auto sec = L - f(l_group) + l_group + 1;
            for (auto i: range<ll>(sec, N)) {
                std::swap(seq[i], seq[l_group]);
            }
        }
        {  // 中间的部分一起执行
            auto x = l_group + 1, y = r_group, len = y - x;
            ranges::reverse(seq.end() - len, seq.end());
            ranges::rotate(seq.begin() + x, seq.end() - len, seq.end());
        }
        {  // 执行这一组到 R
            auto sec = R - f(r_group) + r_group + 1;
            for (auto i: range<ll>(r_group + 1, sec + 1)) {
                std::swap(seq[i], seq[r_group]);
            }
        }

    egg:
        for (auto i: seq)  std::cout << i + 1 << ' ';
        std::cout << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1751209703359097::solve();
    return 0;
}
