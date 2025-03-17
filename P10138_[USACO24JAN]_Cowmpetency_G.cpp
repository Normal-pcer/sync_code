/**
 * @link https://www.luogu.com.cn/problem/P10138
 */
#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"
using namespace lib;

/**
 * 一个限制 (a, h) 可以要求 [h] > [1], [2], ..., [h - 1]
 * 同时 [a + 1], [a + 2], ..., [h - 1] 一定比较小。
 * 
 * 考虑以下问题：
 * 共有 N 个数，指定部分数一定是前缀最大值，有多少种取法。
 * F[i][j] 表示前 i 个数，最大值为 j 的取法。
 * 转移：F[i][j] -> F[i + 1][k]（k > j）(a)
 * F[i][j] * j -> F[i + 1][j] (b)
 * 
 * 另外，本题的一个限制还要求一些牛不可以是前缀最大值。
 * 如果只能是前缀最大值，只能进行转移 a。
 * 如果不能是前缀最大值，只能进行转移 b。
 * 否则，正常进行转移。
 * 
 * 似乎这样可以保证正确性，但是复杂度比较劣，可以荣获 50 分。
 * 
 * 发现很容易出现特别多个连续的 NoneType 和 NotMax。
 * 如果进行很多次转移 b，显然直接乘就行。
 * 
 * K * F[i - t][k] -> F[i][j] ?
 * 如果接下来 t 个数中出现了前缀最大值 j。
 * 一共有多少种取法？首先 t 个里面必须有一个选定了 j，其他的都必须不大于 j
 * 用总共的减去都小于 j 的即可。
 * K = j**t - (j-1)**t
 * 
 * a, b 同时转移，
 * sum K * F[i - t][k](k < j) + j**t * F[i - t][j] -> F[i][j]
 * 
 * 这里可以记录一下前缀和。
 * 
 * 似乎比较乱。把连续段压缩到一起。
 * G[i][j] 表示取到第 i 个连续段，当前的前缀最大值为 j 的方案数。
 * IsMax 的段就别压了。
 * 
 * 转移：
 * NotMax:
 * G[i][j] <- G[i - 1][j] * j**len
 * 
 * IsMax:
 * G[i][j] <- sum G[i - 1][k] (k < j)
 * 
 * NoneType:
 * let K = (t - 1) * (j - 1)
 * G[i][j] <- K * sum G[i - 1][k] (k < j) + j**len * G[i - 1][j]
 */
namespace Solution_9091860887338826 {
    i32 constexpr mod = 1e9 + 7;
    auto constexpr qpow(i64 a, i64 b, i32 const mod) -> i32 {
        i64 res = 1;
        for (; b != 0; b >>= 1, a = a * a % mod) {
            if (b & 1)  res = res * a % mod;
        }
        return res;
    }
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N, Q, C;
        std::cin >> N >> Q >> C;

        std::vector<std::pair<i32, i32>> limits(Q);

        enum Type {
            NoneType, IsMax, NotMax
        };
        struct Segment {
            i32 begin, end;
        };
        struct Point {
            i32 pos;
        };
        std::vector<Point> is_max_points; 
        std::vector<Segment> not_max_segs;
        for (auto &[a, h]: limits) {
            std::cin >> a >> h;
            is_max_points.push_back({h});
            not_max_segs.push_back({a + 1, h});
        }

        debug {
            std::cout << "Not Max Segs: " << std::endl;
            for (auto x: not_max_segs)  std::cout << "[" << x.begin << ", " << x.end << ")" << std::endl;
        }

        // 合并 not_max 的区间
        not_max_segs = [&]() {
            std::vector<Segment> res;
            i32 prev_begin = 0;
            i32 prev_end = 0;

            ranges::sort(not_max_segs, lam(x, y, x.end == y.end? x.begin < y.begin: x.end < y.end));
            for (auto [begin, end]: not_max_segs) {
                if (prev_end <= begin) {
                    if (prev_begin != prev_end)  res.push_back({prev_begin, prev_end});
                    prev_begin = begin, prev_end = end;
                } else {
                    prev_end = end;
                }
            }
            if (prev_begin != prev_end)  res.push_back({prev_begin, prev_end});
            return res; 
        }();
        ranges::sort(is_max_points, std::less{}, lam(x, x.pos));
        {
            auto tmp = ranges::unique(is_max_points, std::equal_to{}, lam(x, x.pos));
            is_max_points.erase(tmp.begin(), tmp.end());
        }

        struct SegmentWithType {
            Segment seg;
            Type type;
        };
        std::vector<SegmentWithType> all_segs;

        auto it1 = is_max_points.begin();
        auto it2 = not_max_segs.begin();
        for (i32 pos = 1; pos != N + 1; ) {
            if (it1 != is_max_points.end() and pos == it1->pos) {
                all_segs.push_back({.seg = {pos, pos + 1}, .type = IsMax});
                pos++, it1++;
            } else if (it2 != not_max_segs.end() and pos == it2->begin) {
                all_segs.push_back({.seg = {it2->begin, it2->end}, .type = NotMax});
                pos = it2->end, it2++;
            } else {
                auto last = std::min(it1 == is_max_points.end()? N + 1: it1->pos, it2 == not_max_segs.end()? N + 1: it2->begin);
                assert(last > pos);
                all_segs.push_back({.seg = {pos, last}, .type = NoneType});
                pos = last;
            }
        }

        debug {
            std::cout << "All Segs" << std::endl;
            for (auto x: all_segs) {
                std::cout << "seg = [" << x.seg.begin << ", " << x.seg.end << "), type = " << static_cast<i32>(x.type) << std::endl;
            }
        }

        i32 segs_cnt = all_segs.size();
        all_segs.emplace(all_segs.begin());
        std::vector G(segs_cnt + 1, std::vector<i32>(C + 1));

        G[0][0] = 1;
        for (i32 i = 1; i <= segs_cnt; i++) {
            std::vector ps(G[i - 1]);  // 前缀和
            std::partial_sum(ps.begin(), ps.end(), ps.begin(), lam(x, y, (x + y) % mod));
            auto this_seg = all_segs[i];
            auto len = this_seg.seg.end - this_seg.seg.begin;

            for (i32 j = 1; j <= C; j++) {
                if (this_seg.type == IsMax) {
                    G[i][j] = ps[j - 1];
                } else if (this_seg.type == NotMax) {
                    G[i][j] = static_cast<i64>(G[i - 1][j]) * qpow(j, len, mod) % mod;
                } else {
                    auto K = (qpow(j, len, mod) - qpow(j - 1, len, mod) + mod) % mod;
                    G[i][j] = (
                        static_cast<i64>(K) * ps[j - 1] % mod +
                        static_cast<i64>(G[i - 1][j]) * qpow(j, len, mod) % mod
                    ) % mod;
                }
            }
        }

        debug for (i32 i = 0; i <= segs_cnt; i++) {
            for (i32 j = 1; j <= C; j++) {
                std::cout << "G[" << i << "][" << j << "] = " << G[i][j] << std::endl;
            }
        }

        i32 ans = 0;
        for (i32 i = 1; i <= C; i++)  ans = (ans + G[segs_cnt][i]) % mod;
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_9091860887338826::solve();
    return 0;
}
