/**
 * @link https://www.luogu.com.cn/problem/P3805
 */
#include "lib"
#include "libs/fixed_int.hpp"
using namespace lib;

/*
manacher 算法模板。

在字符串 s 中，令 d1[i] 表示：以 s[i] 为中间字符、长度为奇数的最长回文串半径（例如 abcdcba 的半径为 3）。
考虑如何快速维护 s。

假设在当前已经发现的所有回文字符串中，右端点最靠右的一个，右端点为 R。对应地，这个字符串的左端点为 L。

  L           R
  |-----------|
abcdedcqwqcdedccf
   |-|     |-|
    i'      i

假设处理过程中，希望获取以 i 为中心的 d0。在这个回文区间中，寻找它的对称点 i'。显然 d0[i] 和 d0[i'] 的关联很大（事实上很多情况是相等）。
以下是一个不等的反例：

   L         R
   |---------|
abcdedcfcabdedbaf
  |---|  |-----|
  l i'r  l  i  r

事实上，i 对应的这段回文区间，可以安全复用的范围是从 L 到 i'r。接下来，可以继续向外暴力扩展，获取最大的回文范围。

尽管需要多次暴力扩展，但是注意到整个过程中，R 是单调递增的，所以整体复杂度实际上是 O(n)。
*/
namespace Solution_4339116967791283 {
namespace {
    auto solve() -> void {
        std::string s; std::cin >> s;

        auto n = static_cast<i32>(s.size());
        std::vector<i32> d0(n, 0);

        i32 l = 0, r = -1;  // [l, r] 区间是已经找到的一个回文串。初始为空区间
        for (i32 i = 0; i < n; ++i) {
            // 如果 i 已经不在这个大的区间之内，直接暴力寻找
            if (i > r) {
                for (; ; ++d0[i]) {
                    auto p = i - d0[i] - 1, q = i + d0[i] + 1;
                    if (p < 0 || q >= n) break;
                    if (s[p] != s[q]) break;
                }
            } else {
                // 否则，可以找到一个 i 在该回文区间中的对称点
                auto other = l + r - i;
                auto p = other - d0[other], q = other + d0[other];
                assert(p >= 0 && q < n);

                // 保证 [p, q] 在 [l, r] 限定的范围内
                if (p < l) {
                    d0[i] = other - l;
                    for (; ; ++d0[i]) {
                        auto p = i - d0[i] - 1, q = i + d0[i] + 1;
                        if (p < 0 || q >= n) break;
                        if (s[p] != s[q]) break;
                    }
                } else {
                    d0[i] = d0[other];
                }
            }
        }
    }
}
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_4339116967791283::solve();
    return 0;
}
