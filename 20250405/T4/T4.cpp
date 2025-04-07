/**
 * @link
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

#define FILENAME "T4"

namespace Solution_1003255212752722 {
    auto solve1(i32 const n, i32 const k) -> void {
        assert(k == 1);
        std::vector<i32> a(n);
        for (auto &x: a) {
            std::cout << x << endl;
        }
        std::sort(a.begin(), a.end());
        auto it = std::unique(a.begin(), a.end());
        std::cout << it - a.begin() << endl;
    }
    auto solve2(i32 const n, i32 const k) -> void {  // 子任务 2
        assert(k == 2);
        std::vector<i32> a(n);
        for (auto &x: a) {
            std::cin >> x;
        }

        std::vector<i32> countPre(n);  // 前缀有几种物品
        {
            std::vector<char> exists(n + 1);
            i32 cnt = 0;
            for (i32 i = 0; i < n; i++) {
                cnt += not exists[a[i]];
                countPre[i] = cnt;
                exists[a[i]] = true;
            }
        }

        std::vector<i32> countSuf(n);
        {
            std::vector<char> exists(n + 1);
            i32 cnt = 0;
            for (i32 i = n; i --> 0; ) {
                cnt += not exists[a[i]];
                countSuf[i] = cnt;
                exists[a[i]] = true;
            }
        }

        i32 ans = 0;
        for (i32 i = 1; i < n; i++) {
            // [0, i), [i, n)
            i32 curAns = countPre[i - 1] + countSuf[i];
            chkMax(ans, curAns);
        }
        std::cout << ans << endl;
    }
    auto solve34(i32 const n, i32 const k) -> void {
        assert(k == 3);
        std::vector<i32> a(n);
        for (auto &x: a) {
            std::cin >> x;
        }

        auto countUnique = [&](auto begin, auto end, auto outBegin) -> auto {
            std::vector<char> exists(n + 1);
            auto itIn = begin;
            auto itOut = outBegin;
            i32 count = 0;
            for (; itIn != end; itIn++) {
                auto x = *itIn;
                count += not exists[x];
                *itOut++ = count;
                exists[x] = true;
            }
            return itOut;
        };
        i32 ans = 0;
        for (i32 i = 1; i < n - 1; i++) {
            std::vector<i32> cnt1(i);
            std::vector<i32> cnt2Pre(n);
            std::vector<i32> cnt2Suf(n);

            // 处理第一个人拿到的
            countUnique(a.begin(), a.begin() + i, cnt1.begin());

            // 第二个人和第三个人
            countUnique(a.begin() + i, a.end(), cnt2Pre.begin() + i);
            countUnique(a.rbegin(), a.rbegin() + (n - i), cnt2Suf.rbegin());
            for (i32 j = i + 1; j < n; j++) {
                auto curAns = 0;
                curAns += cnt1[i - 1];
                curAns += cnt2Pre[j - 1];
                curAns += cnt2Suf[j];
                chkMax(ans, curAns);
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

    i32 n, k;
    std::cin >> n >> k;
    if (k == 1) {
        Solution_1003255212752722::solve1(n, k);
    } else if (k == 2) {
        Solution_1003255212752722::solve2(n, k);
    } else {
        Solution_1003255212752722::solve34(n, k);
    }
    return 0;
}
