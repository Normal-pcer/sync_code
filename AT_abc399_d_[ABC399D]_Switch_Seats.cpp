/**
 * @link https://www.luogu.com.cn/problem/AT_abc399_d
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

/*
ab 构成情侣对，似乎只有不多的方式。
首先，一定会两两相邻。
ab...ba
ab...ab

记录另一个数的出现位置，判断其临位即可。
*/
namespace Solution_9316990088347499 {
    auto solve() -> void {
        i32 N; std::cin >> N;
        std::vector<i32> a(N + N);
        for (auto &x: a) std::cin >> x;

        struct BucItem {
            i32 cnt = 0;
            std::array<i32, 2> pos{};
        };
        std::vector<BucItem> buc(N + 1);
        for (i32 i = 0; i < N + N; i++) {
            auto &item = buc[a[i]];
            item.pos[item.cnt] = i;
            item.cnt++;
        }

        std::vector<i32> other(N + N);  // 与其配对的
        for (i32 i = 1; i <= N; i++) {
            auto const &p = buc[a[i]].pos;
            other[p[0]] = p[1];
            other[p[1]] = p[0];
        }

        std::set<std::pair<i32, i32>> s;
        for (i32 i = 1; i < N + N; i++) {
            auto check = [&](std::array<i32, 4> arr) -> bool {
                std::sort(arr.begin(), arr.end());
                auto new_end = std::unique(arr.begin(), arr.end());
                if (new_end != arr.end()) return false;

                std::array<i32, 4> colors;
                ranges::transform(arr, colors.begin(), lam(x, a[x]));
                ranges::sort(colors);
                if (std::distance(colors.begin(), ranges::unique(colors).begin()) != 2) return false;

                if (arr[1] - arr[0] == 1 and arr[3] - arr[2] == 1) return true;
                return false;
            };

            auto curPair = buc[a[i]].pos;
            // 添加一个颜色的另一对
            auto checkPair = [&](i32 c) -> void {
                if (c == a[i]) return;
                if (std::abs(curPair[0] - curPair[1]) == 1) return;
                auto otherPair = buc[c].pos;
                if (std::abs(otherPair[0] - otherPair[1]) == 1) return;
                std::array<i32, 4> res;
                
                ranges::copy(otherPair, ranges::copy(curPair, res.begin()).out);
                if (check(res)) {
                    auto x = a[i], y = c;
                    assert(x != y);
                    if (x > y) std::swap(x, y);
                    s.insert({x, y});
                }
            };

            checkPair(a[i - 1]);
        }
        auto ans = s.size();
        std::cout << ans << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    i32 T; std::cin >> T;
    for (auto _ = T; _ --> 0; ) {
        Solution_9316990088347499::solve();
    }
    return 0;
}
