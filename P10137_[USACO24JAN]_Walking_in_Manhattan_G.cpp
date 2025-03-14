/**
 * @link https://www.luogu.com.cn/problem/P10137
 */
#include <algorithm>
#include <bits/ranges_algo.h>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <iterator>
#include <numeric>
#include <utility>
#include <vector>
#include <format>
bool DEBUG_MODE = false;
#define debug if (DEBUG_MODE)
template <typename T> inline auto chkMax(T &base, const T &cmp) -> T & { return (base = std::max(base, cmp)); }
template <typename T> inline auto chkMin(T &base, const T &cmp) -> T & { return (base = std::min(base, cmp)); }
#define never if constexpr(0)
const int inf = 0x3f3f3f3f;  const long long infLL = 0x3f3f3f3f3f3f3f3fLL; using ll = long long; using ull = unsigned long long;
const char endl = '\n';

#define __lambda_1(expr) [&]() { return expr; }
#define __lambda_2(a, expr) [&](auto a) { return expr; }
#define __lambda_3(a, b, expr) [&](auto a, auto b) { return expr; }
#define __lambda_4(a, b, c, expr) [&](auto a, auto b, auto c) { return expr; }
#define __lambda_overload(a, b, c, d, e, ...) __lambda_##e
#define lambda(...) __lambda_overload(__VA_ARGS__, 4, 3, 2, 1)(__VA_ARGS__)
#define lam lambda
#include "./libs/fixed_int.hpp"

#include "./libs/debug_log.hpp"


/*
从 x 值相同的地方开始向右走。
(x0, y0) 以及 (x0, y1) 进行第一次转弯时的 x 值一定一致。
快速查询一个方向上的终点可以使用倍增法。

一定是类似 R -> U -> R -> U 的转弯方式。
R[i][j] 表示从 x = i 的点开始向右走，第 2**j 次转弯时所处的 x 坐标。
U[i][j] 表示从 y = i 的点开始向上走，第 2**j 次转弯时所处的 y 坐标。
那么我们似乎就可以快速求出从一个点（十字路口）向右第 i 次转弯时的位置了。

*/
namespace Solution_7715973200731060 {
    namespace ranges = std::ranges;

    template <typename T, typename U, typename Func, typename Comp = std::less<>>
    auto lowerBoundValue(T begin, T end, U x, Func &&f, Comp &&cmp = {}) -> T {
        while (end - begin >= 1) {
            auto mid = std::midpoint(begin, end);
            if (cmp(f(mid), x))  begin = mid + 1;
            else  end = mid;
        }
        return begin;
    }

    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N, Q;  std::cin >> N >> Q;
        struct Road {
            enum Direction {
                Horizontal, Vertical
            } dir = Horizontal;
            i32 value = 0;
        };

        std::vector<i64> all_x, all_y;
        std::vector<Road> roads;
        for (i32 i = 0; i < N; i++) {
            char ch;  i32 x;
            std::cin >> ch >> x;
            if (ch == 'V')  roads.push_back({Road::Vertical, x}), all_x.push_back(x);
            else  roads.push_back({Road::Horizontal, x}), all_y.push_back(x);
        }
        all_x.push_back(infLL), all_y.push_back(infLL);

        auto sort_unique = [](auto &x) {
            ranges::sort(x);
            auto tmp = ranges::unique(x);
            x.erase(tmp.begin(), tmp.end());
        };
        sort_unique(all_x);
        sort_unique(all_y);

        auto logN = std::__lg(N) + 1;
        // 为了访存连续调换 i, j
        i32 r_cnt = all_x.size() - 1, u_cnt = all_y.size() - 1;
        std::vector R(logN, std::vector<i64>(r_cnt + 1, r_cnt));
        std::vector U(logN, std::vector<i64>(u_cnt + 1, u_cnt));

        // 首先需要处理出一个点的下一次转弯在哪里
        // 如果一段的长度为奇数，自然需要进行一次转弯；否则和下一个点一起转弯。
        for (i32 i = r_cnt - 1; i --> 0; ) {
            auto dis = all_x[i + 1] - all_x[i];
            if (dis % 2 == 0) {
                R[0][i] = R[0][i + 1];
            } else {
                R[0][i] = i + 1;
            }
        }
        for (i32 i = u_cnt - 1; i --> 0; ) {
            auto dis = all_y[i + 1] - all_y[i];
            if (dis % 2 == 0) {
                U[0][i] = U[0][i + 1];
            } else {
                U[0][i] = i + 1;
            }
        }

        for (i32 j = 1; j < logN; j++) {
            R[j][r_cnt] = r_cnt;
            for (i32 i = 0; i < r_cnt; i++) {
                if (i + (1 << j) >= r_cnt)  break;
                R[j][i] = R[j - 1][R[j - 1][i]];
            }
        }
        for (i32 j = 1; j < logN; j++) {
            U[j][u_cnt] = u_cnt;
            for (i32 i = 0; i < u_cnt; i++) {
                if (i + (1 << j) >= u_cnt)  break;
                U[j][i] = U[j - 1][U[j - 1][i]];
            }
        }

        // for (i32 j = 0; j < logN; j++) {
        //     for (i32 i = 0; i <= r_cnt; i++) {
        //         printValues(j, i, R[j][i]);
        //     }
        //     for (i32 i = 0; i <= u_cnt; i++) {
        //         printValues(j, i, U[j][i]);
        //     }
        // }

        // 从点 (x0, y0) 开始向上走，返回转弯第 cnt 次的点。
        auto walk_from = [&](i64 x0, i64 y0, i32 cnt) -> std::pair<i64, i64> {
            auto cnt_r = cnt / 2;
            auto cnt_u = cnt - cnt / 2;

            auto x = x0;
            for (i32 i = 0; i < logN; i++) {
                if (cnt_r & (1 << i))  x = R[i][x];
            }
            auto y = y0;
            for (i32 i = 0; i < logN; i++) {
                if (cnt_u & (1 << i))  y = U[i][y];
            }
            debug  std::cout << std::format("walk_from {} {} {} -> {} {}", x0, y0, cnt, all_x[x], all_y[y]) << std::endl;
            return {all_x[x], all_y[y]};
        };

        // 点 (x0, y0) 开始向上走，返回路程恰好为 dis 的点
        auto walk_from_dis = [&](i64 x0, i64 y0, i64 dis) -> std::pair<i64, i64> {
            auto cnt_to_dis = [&](i32 cnt) -> i64 {
                auto [x1, y1] = walk_from(x0, y0, cnt);
                // if (x1 == r_cnt or y1 == u_cnt)  return infLL;
                return x1 - all_x[x0] + y1 - all_y[y0];
            };

            auto cnt = lowerBoundValue<i64>(0, static_cast<i64>(N) * 6 / 5, dis, cnt_to_dis) - 1;
            auto [x1, y1] = walk_from(x0, y0, cnt);
            auto dis_pass = cnt_to_dis(cnt);
            auto dis_remains = dis - dis_pass;
            if (dis_pass % 2 == 0) {
                // 向北继续走
                y1 += dis_remains;
            } else {
                x1 += dis_remains;
            }
            return {x1, y1};
        };
        for (auto q = Q; q --> 0; ) {
            i64 x, y, val;  std::cin >> x >> y >> val;

            auto x1 = x, y1 = y;
            auto need_turn = false;  // 还需要再转一次
            // 走到一个交叉口
            if (ranges::binary_search(all_x, x)) {
                // 位于一条街上
                auto up_to_iter = ranges::lower_bound(all_y, y);
                if (std::next(up_to_iter) == all_y.end()) {
                    y1 += val;
                    val = 0;
                } else {
                    auto up_to = *up_to_iter;
                    auto dis = up_to - y;
                    if (val <= dis) {
                        std::cout << x1 << " " << y1 + val << endl;
                        continue;
                    }
                    y1 = up_to;
                    if (dis % 2 == 1)  need_turn = true;
                    val -= dis;
                }
            } else if (ranges::binary_search(all_y, y)) {
                // 在一条路上
                auto right_to_iter = ranges::lower_bound(all_x, x);
                if (std::next(right_to_iter) == all_x.end()) {
                    x1 += val;
                    val = 0;
                } else {
                    auto right_to = *right_to_iter;
                    auto dis = right_to - x;
                    if (val <= dis) {
                        std::cout << x1 + val << " " << y1 << endl;
                        continue;
                    }
                    x1 = right_to;
                    if (dis % 2 == 1)  need_turn = true;
                    val -= dis;
                }
            } else {
                assert(false), __builtin_unreachable();
            }

            auto getIndex = [&](i32 x, auto const &in) -> i32 {
                auto it = ranges::lower_bound(in, x);
                assert(*it == x);
                return std::distance(in.begin(), it);
            };

            if (val == 0) {
                std::cout << x1 << " " << y1 << endl;
                continue;
            }
            x1 = getIndex(x1, all_x);
            y1 = getIndex(y1, all_y);

            if (need_turn) {
                auto nx = R[0][x1];
                if (all_x[nx] - all_x[x1] >= val) {
                    std::cout << all_x[x1] + val << " " << all_y[y1] << endl;
                    continue;
                }
                val -= all_x[nx] - all_x[x1];
                x1 = nx;
            }

            auto [ans_x, ans_y] = walk_from_dis(x1, y1, val);
            std::cout << ans_x << " " << ans_y << endl;
        }

    }
}

// 但他们好可爱 qwq
int main(int dyy, char const *cyy[]) {
    DEBUG_MODE = (dyy-1) and not std::strcmp("-d", cyy[1]);
    Solution_7715973200731060::solve();
    return 0;
}
