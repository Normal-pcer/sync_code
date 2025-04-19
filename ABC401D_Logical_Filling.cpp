/**
 * @link https://atcoder.jp/contests/abc401/tasks/abc401_d
 */
#include "./lib_v7.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

namespace Solution_3597503876216660 {
    auto solve() -> void {
        i32 n, k;
        std::cin >> n >> k;

        std::string s;
        std::cin >> s;

        

        for (i32 i = 0; i < n; i++) {
            auto flag = false;
            if (i != 0 and s[i - 1] == 'o') flag = true;
            if (i != n - 1 and s[i + 1] == 'o') flag = true;
            if (flag) s[i] = '.';
        }
        
        auto cnt = static_cast<i32>(ranges::count(s, 'o'));
        if (cnt == k) {
            for (auto &ch: s) {
                if (ch == '?') ch = '.';
            }
        }
        // 考虑每个连续 '?' 的区间
        // 如果类似 o?????o 且需要 floor(n / 2) 个 o，那么可以确定
        // 同理，.?????. - ceil(n / 2)
        // o????. - n / 2
        // .????o - n / 2
        // 每个区间希望的 'o' 数量
        struct Segment {
            std::string::iterator first, last;
            i32 expect = 0;
        };
        std::vector<Segment> segs;
        i32 totalExpected = 0;
        for (auto it = s.begin(); it != s.end(); ) {
            if (*it != '?') {
                ++it;
                continue;
            }
            auto last = ranges::find_if(it, s.end(), lam(x, x != '?'));
            // [it, last) 均为问号
            auto len = static_cast<i32>(std::distance(it, last));
            i32 dotCount = 0, circleCount = 0;
            if (it != s.begin() and *std::prev(it) == 'o') circleCount++;
            else dotCount++;
            if (last != s.end() and *last == 'o') circleCount++;
            else dotCount++;

            if (len % 2 == 0) {
                if (circleCount + dotCount == 1) {
                    segs.push_back({it, last, len / 2});
                }
                totalExpected += len / 2;
            } else {
                if (circleCount == 2) {
                    segs.push_back({it, last, len / 2});
                    totalExpected += len / 2;
                } else if (dotCount == 2) {
                    segs.push_back({it, last, len / 2 + 1});
                    totalExpected += len / 2 + 1;
                } else {
                    totalExpected += len / 2;
                }
            }

            it = last;
        }

        if (totalExpected + cnt == k) {
            for (auto [it, last, expect]: segs) {
                if (cnt + expect > k) continue;
                auto constexpr charset = "o.";
                auto replace = [&](i32 first) -> void {
                    auto x = first;
                    for (; it != last; x ^= 1, ++it) {
                        *it = charset[x];
                    }
                };
    
    
                if (it != s.begin() and *std::prev(it) == 'o') {
                    replace(1);
                } else {
                    replace(0);
                }
                cnt += expect;
            }
        }

        std::cout << s << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_3597503876216660::solve();
    return 0;
}
