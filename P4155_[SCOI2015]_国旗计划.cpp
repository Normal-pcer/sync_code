/**
 * @link https://www.luogu.com.cn/problem/P4155
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

/*
每一次查询：给定 p，p 号战士必须参加活动的前提下，至少需要几个人才能完成任务。
一个贪心策略：
    初始令“占领区间”为 p 战士的区间。
    接下来，每次迭代，选定一个区间 R，使得 R 的左端点已被占领的前提下，右端点尽可能靠右。
    迭代直到占领区间的长度大于 m。

直接按照上述策略模拟的复杂度为 O(n^2)。
考虑优化。这个过程一共需要跳 O(n) 次。
跳的过程可以变为倍增。

记倍增数组 f[t][i] 表示：从 i 号战士占据的区间开始，进行 pow(2, t) 次迭代，最优情况下会跳转到哪个区间。（记录的是区间编号）
于是有如下转移：
    f[t][i] <- f[t - 1][ f[t - 1][i] ]

从 i 号区间开始迭代 s 次，只需要倍增地跳 O(log(s)) 次。
接下来，二分寻找最小且合法的 s 即可。每次询问复杂度为 O(log^2 (s))。

处理 f[0][i]，按照如下方式：
    假设存在一个数组 a[i]，对于每一个区间，用其左端点作为下标，右端点作为值。
    使用 ST 表来维护这个数组的区间最大值。
    接下来，对于每一个区间，在 a 数组上进行一下区间查询最大值即可。
*/
namespace Solution_5523585123640439 {
    auto log2(i32 x) -> i32 {
        return std::__lg(x);
    }
    template <typename T>
    class SparseTable {
        i32 n, logN;
        std::vector<std::vector<T>> f;
    public:
        SparseTable(i32 n, std::vector<T> const &a): n(n), logN(log2(n)), 
                f(logN + 1, std::vector<T>(n)) {
            ranges::copy(a, f[0].begin());
            for (i32 j = 1; j <= logN; j++) {
                for (i32 i = 0; i < n; i++) {
                    if (i + (1 << j) > n) break;
                    f[j][i] = std::max(f[j - 1][i], f[j - 1][i + (1 << (j - 1))]);
                }
            }
        }
        auto maxRange(i32 begin, i32 end) -> T {
            auto len = end - begin;
            return std::max(f[log2(len)][begin], f[log2(len)][end - (1 << log2(len))]);
        }
    };
    template <typename T, typename U, typename Func, typename Comp = std::less<void>>
    auto lowerBoundValue(T begin, T end, U x, Func &&f, Comp &&cmp = {}) -> T {
        while (end - begin >= 1) {
            auto mid = std::midpoint(begin, end);
            if (cmp(f(mid), x)) begin = mid + 1;
            else end = mid;
        }
        return begin;
    }
    auto solve() -> void {
        i32 n, m;
        std::cin >> n >> m;

        using Pos = i32;
        struct Segment {
            Pos left, right;
        };
        std::vector<Segment> segs(n);
        for (auto &[l, r]: segs) {
            std::cin >> l >> r;
            l--, r--;  // 转为 1-index
            if (r < l) r += m;
        }

        segs.resize(n + n);
        ranges::transform(segs.begin(), segs.begin() + n, segs.begin() + n, [&](auto seg) {
            return Segment{seg.left + m, seg.right + m};
        });

        // f[t][i]
        // 表示从 i 所在区间迭代 pow(2, j) 次的最优解
        auto logN = log2(n + n);
        std::vector f(logN + 1, std::vector<i32>(n + n));
        {
            {  // 处理 f[0]
                // 离散化
                std::vector<i32> values;
                for (auto [l, r]: segs) {
                    values.push_back(l);
                    values.push_back(r);
                }
                ranges::sort(values);
                auto tmp = ranges::unique(values);
                values.erase(tmp.begin(), tmp.end());

                auto getIndex = [&](i32 x) -> i32 {
                    auto it = ranges::lower_bound(values, x);
                    assert(*it == x);
                    return std::distance(values.begin(), it);
                };

                std::vector<Segment> mappedSegs(segs);
                for (auto &[l, r]: mappedSegs) {
                    l = getIndex(l);
                    r = getIndex(r);
                }
                
                // 使得每个区间相当于在 a 数组上进行区间查询
                // pair 存储右端点值和对应下标
                std::vector<std::pair<i32, i32>> a(values.size(), {-1, -1});
                for (i32 i = 0; i < n + n; i++) {  // 修改包含倍增出来的，查询不包含
                    auto [l, r] = mappedSegs[i];
                    chkMax(a[l], {r, i});
                }

                SparseTable st(values.size(), a);
                for (i32 i = 0; i < n; i++) {
                    auto [l, r] = mappedSegs[i];
                    auto ans = st.maxRange(l, r + 1);
                    assert(ans.first >= 0);
                    f[0][i] = ans.second;
                }
                for (i32 i = n; i < n + n; i++) {
                    f[0][i] = f[0][i - n] + n;
                }
            }
                
            for (i32 t = 1; t <= logN; t++) {
                for (i32 i = 0; i < n; i++) {
                    f[t][i] = f[t - 1][f[t - 1][i]];
                }
            }
        }

        // 从 s 所在区间开始进行 n 次迭代，返回结果区间的编号。
        auto walkN = [&](i32 s, i32 n) -> i32 {
            i32 cur = s;
            for (i32 t = 0, maxT = log2(n); t <= maxT; t++) {
                if (n & (1 << t)) {
                    cur = f[t][cur];
                }
            }
            debug std::cout << std::format("walkK({}, {}) -> {}", s, n, cur) << std::endl;
            return cur;
        };

        for (i32 p = 0; p < n; p++) {
            // 二分，对于每一个 s 判断起点和终点的距离是否达到 m
            auto ans = lowerBoundValue(0, n, m, [&](i32 s) -> i32 {
                auto lastSeg = walkN(p, s);
                auto endPoint = segs[lastSeg].right;
                return endPoint - segs[p].left;
            });

            std::cout << ans + 1 << " ";
        }
        std::cout << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_5523585123640439::solve();
    return 0;
}
