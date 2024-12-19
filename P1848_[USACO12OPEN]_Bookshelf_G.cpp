/**
 * @link https://www.luogu.com.cn/problem/P1848
 */

#include "./lib"


#include "./libs/range.hpp"


using namespace lib;

namespace Solution_5926768708892818 {
    class SegTree {
        struct Node {
            int begin, end;
            ll max_h, min_ans = infLL, add_tag;
        };
        std::vector<Node> tr;
#define ls (p<<1)
#define rs (p<<1 | 1)
        void pushUp(int p) {
            tr[p].max_h = std::max(tr[ls].max_h, tr[rs].max_h);
            tr[p].min_ans = std::min(tr[ls].min_ans, tr[rs].min_ans);
        }
        void addNode(int p, ll val) {
            tr[p].max_h += val;
            tr[p].min_ans += val;
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
            if (begin + 1 == end)  return;
            auto mid = std::midpoint(begin, end);
            build(begin, mid, ls), build(mid, end, rs);
        }
    public:
        SegTree(int begin, int end): tr((end - begin) << 2) {
            build(begin, end, 1);
        }
        ll maxH(int begin, int end, int p = 1) {
            if (begin >= end)  return 0LL;
            if (tr[p].begin >= begin and tr[p].end <= end) {
                return tr[p].max_h;
            }
            auto res = 0LL;
            pushDown(p);
            if (tr[ls].end > begin)  chkMax(res, maxH(begin, end, ls));
            if (tr[rs].begin < end)  chkMax(res, maxH(begin, end, rs));
            return res;
        }
        ll minAns(int begin, int end, int p = 1) {
            if (begin >= end)  return 0LL;
            if (tr[p].begin >= begin and tr[p].end <= end) {
                return tr[p].min_ans;
            }
            auto res = infLL;
            pushDown(p);
            if (tr[ls].end > begin)  chkMin(res, minAns(begin, end, ls));
            if (tr[rs].begin < end)  chkMin(res, minAns(begin, end, rs));
            return res;
        }
        void add(int begin, int end, ll val, int p = 1) {
            if (begin >= end)  return;
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
            if (tr[p].begin == pos and pos + 1 == tr[p].end) {
                tr[p].max_h = 0;
                tr[p].min_ans = val;
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

        int N, L;  std::cin >> N >> L;
        struct Book { int height, width; };
        std::vector<Book> books(N+1);
        for (auto &[h, w]: books | views::drop(1))  std::cin >> h >> w;
        std::vector<ll> w_ps(books.size());
        ranges::transform(books, w_ps.begin(), lam(x, x.width)), std::partial_sum(w_ps.begin(), w_ps.end(), w_ps.begin());

        // std::vector<ll> F(N+1, infLL);
        // F.at(0) = 0;
        // for (auto i: range(1, N+1)) {
        //     for (auto j: range(0, i)) {
        //         if (w_ps.at(i) - w_ps.at(j) <= L) {
        //             auto max = 0;
        //             for (auto k: range(j+1, i+1)) {
        //                 chkMax(max, books.at(k).height);
        //             }
        //             chkMin(F.at(i), F.at(j) + max);
        //         }
        //     }
        // }
        std::deque<int> st {0};  // st.back() 是最近的比 i 高的元素，单调递减栈
        books.front().height = inf;
        SegTree sgt(0, N + 1);
        std::vector<ll> F(N+1, infLL);
        sgt.assign(0, 0);
        for (auto i: range(1, N+1)) {
            debug  std::cout << std::format("While i = {}", i) << std::endl;
            debug {
                std::cout << "st: " << std::endl;
                for (auto i: st)  std::cout << std::format("({}){}", books.at(i).height, i) << ' ';
                std::cout << std::endl;
            }
            // 更新 H 的最大值
            // 现在的 H 中可以包含 a[i]
            // 维护 st
            auto prev = i;
            while (not st.empty() and books.at(st.back()).height <= books.at(i).height) {
                // (back, prev] 区间的 maxH 相同，均小于 books[i].h
                debug  std::cout << std::format("{}: sgt.maxH({}, {}) -> {}", __LINE__, st.back(), prev, sgt.maxH(st.back(), prev)) << std::endl;
                auto origin_max_h = sgt.maxH(st.back(), prev);
                // auto origin_max_h = books.at(i).height;
                auto delta_h = books.at(i).height - origin_max_h;
                debug  std::cout << std::format("{}: sgt.add({}, {}, {})", __LINE__, st.back(), prev, delta_h) << std::endl;
                sgt.add(st.back(), prev, delta_h);
                prev = st.back();
                st.pop_back();
            }
            debug  std::cout << std::format("{}: sgt.maxH({}, {}) -> {}", __LINE__, st.back(), prev, sgt.maxH(st.back(), prev)) << std::endl;
            auto origin_max_h = sgt.maxH(st.back(), prev);
            // auto origin_max_h = books.at(i).height;
            auto delta_h = books.at(i).height - origin_max_h;
            debug  std::cout << std::format("{}: sgt.add({}, {}, {})", __LINE__, st.back(), prev, delta_h) << std::endl;
            sgt.add(st.back(), prev, delta_h);

            st.push_back(i);
            debug {
                auto max = 0;
                for (auto j: range(1, i+1) | views::reverse) {
                    chkMax(max, books.at(j).height);
                    assert(max == sgt.maxH(j-1, i));
                }
            }
            debug {
                std::cout << "new st: " << std::endl;
                for (auto i: st)  std::cout << std::format("({}){}", books.at(i).height, i) << ' ';
                std::cout << std::endl;
            }

            auto query_begin = std::distance(w_ps.begin(), ranges::lower_bound(w_ps, w_ps.at(i) - L));
            auto query_end = i + 1;
            debug  std::cout << std::format("{}: sgt.minAns({}, {}) -> {}", __LINE__, query_begin, query_end, sgt.minAns(query_begin, query_end)) << std::endl;
            auto cur = sgt.minAns(query_begin, query_end);  // 区间内可以转移到答案

            F.at(i) = cur;
            debug  std::cout << std::format("{}: sgt.assign({}, {})", __LINE__, i, cur) << std::endl;
            sgt.assign(i, cur);
        }
        std::cout << F.at(N) << std::endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_5926768708892818::solve();
    return 0;
}
