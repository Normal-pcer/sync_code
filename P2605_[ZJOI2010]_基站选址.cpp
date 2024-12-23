/**
 * @link https://www.luogu.com.cn/problem/P2605
 */
// #ifndef ONLINE_JUDGE
// #define GNU_DEBUG
// #define _GLIBCXX_DEBUG 1
// #define _GLIBCXX_DEBUG_PEDANTIC 1
// #define _GLIBCXX_SANITIZE_VECTOR 1
// #endif
#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_6006738937570677 {
#if true
    class SegTree {
        struct Node {
            int begin, end;
            int min = inf, add_tag = 0;
        };
        std::vector<Node> tr;
#define ls (p << 1)
#define rs (p << 1 | 1)
        void pushUp(int p) {
            tr[p].min = std::min(tr[ls].min, tr[rs].min);
        }
        void addNode(int p, int val) {
            tr[p].min += val;
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
        void build(int begin, int end, int p) {
            tr[p].begin = begin, tr[p].end = end;
            if (begin + 1 == end)  return;
            auto mid = std::midpoint(begin, end);
            build(begin, mid, ls), build(mid, end, rs);
        }
    public:
        SegTree(int begin, int end): tr((end - begin) << 2) {
            build(begin, end, 1);
        }
        int min(int begin, int end, int p = 1) {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                return tr[p].min;
            }
            auto res = inf;
            pushDown(p);
            if (tr[ls].end > begin)  chkMin(res, min(begin, end, ls));
            if (tr[rs].begin < end)  chkMin(res, min(begin, end, rs));
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
            if (tr[p].begin == pos and tr[p].end == pos + 1) {
                tr[p].min = val;
                return;
            }
            pushDown(p);
            if (tr[ls].end > pos)  assign(pos, val, ls);
            else  assign(pos, val, rs);
            pushUp(p);
        }
        void clear() {
            auto begin = tr[1].begin, end = tr[1].end;
            ranges::fill(tr, Node{});
            build(begin, end, 1);
        }
#undef ls
#undef rs
    };
#else
    class SegTree {
    public:
        std::vector<int> data;
        SegTree(int, int end): data(end, inf) {}
        int min(int begin, int end) const {
            return *ranges::min_element(data.begin() + begin, data.begin() + end);
        }
        void add(int begin, int end, int val) {
            for (auto i: range(begin, end))  data[i] += val;
        }
        void assign(int pos, int val) {
            data[pos] = val;
        }
    };
}

using Solution_6006738937570677::SegTree;

template <typename C>
struct std::formatter<SegTree, C>: public std::formatter<int, C> {
    using base_type = std::formatter<int, C>;
    template <typename ContextType>
    ContextType::iterator format(const SegTree &other, ContextType context) const {
        std::stringstream stream;
        for (auto i: other.data)  stream << i << ' ';
        return ranges::copy(std::move(stream).str(), context.out()).out;
    }
};

namespace Solution_6006738937570677 {
#endif
    // #define debug_print(args...) debug  std::cout << __LINE__ << ": " << std::format(args) << std::endl;
    #define debug_print(...)
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, K;  std::cin >> N >> K, K++;
        struct Village {
            int pos, cost, range, indemnity;
        };
        std::vector<Village> villages(N+2);  // 从 1 开始编号
        for (auto i: range(2, N+1))  std::cin >> villages[i].pos;
        for (auto i: range(1, N+1))  std::cin >> villages[i].cost;
        for (auto i: range(1, N+1))  std::cin >> villages[i].range;
        for (auto i: range(1, N+1))  std::cin >> villages[i].indemnity;
        villages.front().pos = -inf, villages.back().pos = +inf;

        struct VillagesStatistics {
            int sum;
            std::vector<int> items;
        };
        std::vector<VillagesStatistics> ends(villages.size()+2);  // 接受范围向右在 i 结束
        std::vector<int> beginIndex(villages.size()+2);
        for (auto i: range(1, N+2)) {
            auto [pos, c, r, indemnity] = villages.at(i);
            auto max_pos = pos + r;
            auto it_end = ranges::upper_bound(villages, max_pos, std::less{}, lam(x, x.pos));
            auto index_end = std::distance(villages.begin(), it_end);
            ends.at(index_end).items.push_back(i);
            ends.at(index_end).sum += indemnity;
            auto min_pos = pos - r;
            auto it_begin = ranges::lower_bound(villages, min_pos, std::less{}, lam(x, x.pos));
            auto index_begin = std::distance(villages.begin(), it_begin);
            beginIndex.at(i) = index_begin;
        }

        std::vector F(2, std::vector<int>(N+2));
        std::vector sgts(2, SegTree(0, N+2));
        auto ans = inf;
        sgts.at(0).assign(0, 0);
        for (auto j: range(1, K+1)) {
            for (auto i: range(j, N+2)) {
                // F[i][j] <- F[k][j-1] + p[k][i]
                // 此时的 sgt[j-1] 里包括：F[k][j-1] + p[k][i-1]
                // 希望变为 F[k][j-1] + p[k][i]

                // 考虑 p[k][i] 增量
                // 对于每一个 end 被挂载到 i 上的点 k，[0, beginIndex[k]) 的点上的基站都无法满足要求
                // 则必须在这些基站上进行赔偿
                // 循环内部总共调用了 N 次，所以复杂度是正确的
                for (auto k: ends.at(i).items) {
                    auto begin = beginIndex[k];
                    sgts[(j-1)&1].add(0, begin, villages[k].indemnity);
                }

                // 转移至当前的 F[j][i]
                auto cur = sgts[(j-1)&1].min(0, i);
                cur += villages[i].cost;  // 加上 c[i] 为当前的 F 值
                F[j&1][i] = cur;
                debug_print("sgts[{}].assign({}, {})", j, i, cur);
                sgts.at(j&1).assign(i, cur);  // 写入到新的线段树中
                // for (auto j: range(0, K+1))  debug_print("{}", sgts[j]);
            }
            sgts.at((j-1)&1).clear();
            chkMin(ans, F.at(j&1).at(N+1));
        }
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_6006738937570677::solve();
    return 0;
}