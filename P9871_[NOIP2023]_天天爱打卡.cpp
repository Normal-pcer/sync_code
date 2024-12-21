/**
 * @link https://www.luogu.com.cn/problem/P9871
 */

#include "./libs/debug_macros.hpp"

#include "./lib"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_9512731660030115 {
#if true
    class SegTree {
        struct Node {
            int begin, end;
            ll max = -infLL, add_tag = 0;
        };
        std::vector<Node> tr;

#define ls (p << 1)
#define rs (p << 1 | 1)
        void pushUp(int p) {
            tr[p].max = std::max(tr[ls].max, tr[rs].max);
        }
        void addNode(int p, ll val) {
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
            if (tr[p].begin + 1 == tr[p].end)  return;
            auto mid = std::midpoint(begin, end);
            build(begin, mid, ls), build(mid, end, rs);
        }
    public:
        SegTree(int begin, int end): tr((end - begin) << 2) {
            build(begin, end, 1);
        }
        ll max(int begin, int end, int p = 1) {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                return tr[p].max;
            }
            pushDown(p);
            auto res = -infLL;
            if (tr[ls].end > begin)  chkMax(res, max(begin, end, ls));
            if (tr[rs].begin < end)  chkMax(res, max(begin, end, rs));
            return res;
        }
        void add(int begin, int end, ll val, int p = 1) {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                addNode(p, val);
                return;
            }
            pushDown(p);
            if (tr[ls].end > begin)  add(begin, end, val, ls);
            if (tr[rs].begin < end)  add(begin, end, val, rs);
            pushUp(p);
        }
        void assign(int pos, ll val, int p = 1) {
            if (tr[p].begin + 1 == tr[p].end) {
                tr[p].add_tag = 0;
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
#else
    class SegTree {
        std::vector<ll> data;
    public:
        SegTree(int, int end): data(end, -infLL) {}
        void assign(int pos, int val) { data.at(pos) = val; }
        void add(int begin, int end, int val) {
            for (auto i = begin; i != end; i++) {
                data.at(i) += val;
            }
        }
        ll max(int begin, int end) const {
            auto res = -infLL;
            for (auto i = begin; i != end; i++) {
                chkMax(res, data.at(i));
            }
            return res;
        }
    };
#endif
    void solve() {
        int N, M, K, D;  std::cin >> N >> M >> K >> D;

        // 天数编号从 1 开始
        struct Task {
            int end, cnt;  // [end-cnt, end) 连续跑步
            int val;  // 奖励
        };
        std::vector<Task> tasks(M);
        for (auto &[x, y, val]: tasks)  std::cin >> x >> y >> val, x++;

#if false  // 暴力 DP
        std::vector<ll> F(N+2, -inf);  // 只考虑 [1, i) 天
        F[0] = F[1] = 0;
        auto w = [&](int begin, int end) -> ll {
            auto res = 0LL;
            for (auto &[x, y, val]: tasks) {
                auto cur_begin = x - y, cur_end = x;
                if (begin <= cur_begin and cur_end <= end)  res += val;
            }
            return res;
        };
        for (auto i: range(2, N+2)) {
            F[i] = F[i-1];
            // 枚举 j：[j, i) 范围内连续跑
            auto j_begin = std::clamp(i - K, 1, i), j_end = i;
            for (auto j: range(j_begin, j_end)) {
                chkMax(F[i], F[j-1] + w(j, i) - (i-j)*D);
            }
        }
        debug {
            for (auto i: F)  std::cout << i << ' ';
            std::cout << std::endl;
        }
        auto ans = F[N+1];
        std::cout << ans << endl;
#else
        std::vector<int> values {0, 1, 2, N+1, N+2, N+3};
        for (auto [end, cnt, val]: tasks)  values.push_back(end), values.push_back(end - cnt);
        ranges::sort(values), values.erase(ranges::unique(values).begin(), values.end());
        auto lowerBound = [&](int x) -> int {
            auto it = ranges::lower_bound(values, x);
            return std::distance(values.begin(), it);
        };
        auto upperBound = [&](int x) -> int {
            auto it = ranges::upper_bound(values, x);
            return std::distance(values.begin(), it);
        };
        std::vector<std::vector<int>> ends(lowerBound(N+2));  // end 为第 i 天的所有任务编号
        for (auto i: range(tasks.size())) {
            auto &[end, cnt, val] = tasks[i];
            end = lowerBound(end);
            ends.at(end).push_back(i);
        }
        std::vector<ll> F(lowerBound(N+2), -infLL);  // 只考虑 [1, i) 天，最大能量值
        F[lowerBound(0)] = F[lowerBound(1)] = 0;
        SegTree sgt(lowerBound(1), lowerBound(N+3));  // j 值对应的 F[j-1] + w(j, i) - (i-j)*D，到当前枚举的 i
        sgt.assign(upperBound(0), 0), sgt.assign(upperBound(1), 0);
        for (auto i: range(lowerBound(2), lowerBound(N+2))) {
            // 由 F[i-1] 转移
            F[i] = F[i-1];
            // j：[j, i) 范围内连续跑；取值范围：
            auto j_begin = std::clamp(lowerBound(values.at(i) - K), 1, i), j_end = i;
            // 更新它们的 w(j, i)
            // end 指针移动到了 i 点
            for (auto taskIndex: ends.at(i)) {
                auto [end, cnt, val] = tasks.at(taskIndex);
                auto begin = upperBound(values.at(end) - cnt);
                // j <= begin 的点，w(j, i) 增加了 val
                if (j_begin < begin)  sgt.add(j_begin, begin, val);
            }
            // 更新 -(i-j)*D
            // 增量恒为 -D
            sgt.add(j_begin, j_end, -(ll)D * (values.at(i) - values.at(i - 1)));
            // 查询最大值
            auto cur = sgt.max(j_begin, j_end);
            chkMax(F[i], cur);
            // w(i+1, i+1) = 0
            // D*(i+1) - D*(i+1) = 0
            // F[i] 存入线段树即可
            sgt.assign(i + 1, F[i]);
        }
        auto ans = F[lowerBound(N+1)];
        std::cout << ans << endl;
#endif
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);

    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    int C, T;  std::cin >> C >> T;
    while (T --> 0) {
        Solution_9512731660030115::solve();
    }
    return 0;
}