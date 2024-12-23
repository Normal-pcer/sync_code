/**
 * @link https://www.luogu.com.cn/problem/CF1304F2
 * @link https://codeforces.com/problemset/problem/1304/F2
 */

#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_9416077271017769 {
    class SegTree {
        struct Node {
            int begin, end;
            int max, add_tag;
        };
        std::vector<Node> tr;
        std::vector<int> const &init;
#define ls (p << 1)
#define rs (p << 1 | 1)
        void pushUp(int p) {
            tr[p].max = std::max(tr[ls].max, tr[rs].max);
        }
        void addNode(int p, int val) {
            tr[p].max += val;
            tr[p].add_tag += val;
        }
        void pushDown(int p) {
            if (tr[p].add_tag) {
                for (auto s: {ls, rs}) {
                    addNode(s, tr[p].add_tag);
                }
                tr[p].add_tag = 0;
            }
        }
        void build(int begin, int end, int p = 1) {
            tr[p].begin = begin, tr[p].end = end;
            if (begin + 1 == end) {
                tr[p].max = init.at(begin);
                return;
            }
            auto mid = std::midpoint(begin, end);
            build(begin, mid, ls), build(mid, end, rs);
            pushUp(p);
        }
    public:
        SegTree(int begin, int end, const std::vector<int> &init): tr((end - begin) << 2), init(init) {
            build(begin, end, 1);
        }
        int max(int begin, int end, int p = 1) {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                return tr[p].max;
            }
            pushDown(p);
            auto res = 0;
            if (tr[ls].end > begin)  chkMax(res, max(begin, end, ls));
            if (tr[rs].begin < end)  chkMax(res, max(begin, end, rs));
            return res;
        }
        void add(int begin, int end, int val, int p = 1) {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                addNode(p, val);
                return;
            }
            pushDown(p);
            if (tr[ls].end > begin)  add(begin, end, val, ls);
            if (tr[rs].begin < end)  add(begin, end, val, rs);
            pushUp(p);
        }
        void assign(int pos, int val, int p = 1) {
            if (tr[p].begin + 1 == tr[p].end) {
                tr[p].max = val;
                return;
            }
            pushDown(p);
            if (tr[ls].end > pos)  assign(pos, val, ls);
            else  assign(pos, val, rs);
            pushUp(p);
        }
#undef ls
#undef rs
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, M, K;  std::cin >> N >> M >> K;
        std::vector matrix(N+1, std::vector<int>(M));
        for (auto &line: matrix | views::drop(1)) {
            for (auto &i: line)  std::cin >> i;
        }

#if false
        // F[i][j] 表示第 i 天选在 [j, j+K) 区域，前 i 天贡献的最大值
        std::vector F(N+1, std::vector<int>(M));
        for (auto i: range(1, N+1)) {
            auto &a = matrix[i];
            std::vector<int> a_exc(a.size());
            std::partial_sum(a.begin(), a.end(), a_exc.begin());
            a_exc.insert(a_exc.begin(), 0);

            auto sum = [&](int begin, int end) -> int {
                return a_exc.at(end) - a_exc.at(begin);
            };
            for (auto j: range(0, M - K + 1)) {
                auto max = 0;
                if (i != 1) {
                    for (auto p: range(0, M - K + 1)) {
                        auto uniq = 0;
                        if (p <= j and p + K > j)  uniq = sum(j, p + K);
                        if (p <= j + K and p + K > j + K)  uniq = sum(p, j + K);
                        debug  std::cout << std::format("({})F[{}][{}] <= ({})F[{}][{}] + ({})sum[{}, {}) - {}\n", F[i-1][p] + sum(p, p + K) - uniq, i, j, F[i-1][p], i-1, p, sum(p, p+K), p, p+K, uniq);
                        chkMax(max, F[i-1][p] + sum(p, p + K) - uniq);
                    }
                }
                F[i][j] = max + sum(j, j + K);
            }
        }
        auto ans = ranges::max(F[N]);
        std::cout << ans << endl;
#else
        // F[i][j] 表示第 i 天选在 [j, j+K) 区域，前 i 天贡献的最大值
        std::vector F(N+1, std::vector<int>(M));
        for (auto i: range(1, N+1)) {
            auto &a = matrix[i];
            std::vector<int> a_exc(a.size());
            std::partial_sum(a.begin(), a.end(), a_exc.begin());
            a_exc.insert(a_exc.begin(), 0);
            auto sum = [&](int begin, int end) -> int {
                return a_exc.at(end) - a_exc.at(begin);
            };

            auto &last = F[i-1];
            // 线段树初值
            const auto p_end = M - K + 1; 
            for (auto p: range(0, p_end)) {
                last[p] += sum(p, p + K);
            }
            // 考虑初始的 unique
            // K 处的 unique 为 0，向左一次增加一个重复元素
            /* calc unique */ {
                auto unique = 0;
                for (auto p: range(K) | views::reverse) {
                    unique += a[p];
                    last[p] -= unique;
                }
            }
            // 线段树表示 p -> F[i-1][p] + sum(p, p + K) - unique
            SegTree sgt(0, M, last);
            
            // 直接处理 j = 0 的情况
            auto max = sgt.max(0, p_end);
            if (i == 1)  max = 0;
            F[i][0] = max + sum(0, K);

            for (auto j: range(1, p_end)) {
                // j 右移一位，考虑 unique 变化
                // [unique[j-k], unique[j]) 均被撤出了一个元素 a[j-1]
                // [unique[j], unique[j+k]) 均被放入了一个元素 a[j+k-1]
                // 取相反数
                sgt.add(j - K, j, +a[j-1]);
                sgt.add(j, j + K, -a[j+K-1]);

                // 获取答案
                auto max = sgt.max(0, p_end);
                if (i == 1)  max = 0;
                F[i][j] = max + sum(j, j + K);
            }

            debug {
                std::cout << std::format("F[{}]: ", i);
                for (auto j: range(0, p_end)) {
                    std::cout << std::format("[{}]{} ", j, F[i][j]);
                }
                std::cout << std::endl;
            }
        }
        auto ans = ranges::max(F[N]);
        std::cout << ans << endl;
#endif
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_9416077271017769::solve();
    return 0;
}
