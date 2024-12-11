/**
 * @link https://www.luogu.com.cn/problem/CF1741F
 * @link https://codeforces.com/problemset/problem/1741/F
 */
#pragma GCC optimize(3)
#include <bits/stdc++.h>
bool DEBUG_MODE=false;
// #define debug if(DEBUG_MODE)
template <typename T> inline auto chkMax(T& base, const T& cmp) { return (base = std::max(base, cmp)); }
template <typename T> inline auto chkMin(T& base, const T& cmp) { return (base = std::min(base, cmp)); }
#define never if constexpr(0)
const int inf = 0x3f3f3f3f;  const long long infLL = 0x3f3f3f3f3f3f3f3fLL; using ll = long long; using ull = unsigned long long;
const char endl = '\n';

#define __lambda_1(expr) [&](){return expr;}
#define __lambda_2(a, expr) [&](auto a){return expr;}
#define __lambda_3(a, b, expr) [&](auto a, auto b){return expr;}
#define __lambda_4(a, b, c, expr) [&](auto a, auto b, auto c){return expr;}
#define __lambda_overload(a, b, c, d, e, args...) __lambda_##e
#define lambda(...) __lambda_overload(__VA_ARGS__, 4, 3, 2, 1)(__VA_ARGS__)
#define lam lambda
namespace lib{
#if __cplusplus > 201703LL
namespace ranges { using namespace std::ranges; }
namespace views { using namespace std::ranges::views; }
#endif
}
#include "./libs/range.hpp"

#include "./libs/io.hpp"

using namespace lib;

#define debug never
#define at operator[]
#define unlikely(x) __builtin_expect(!!(x), 0)
#define likely(x) __builtin_expect(!!(x), 1)
namespace Solution_1260521522817291 {
    class SegTree {
        struct Node {
            int ls = 0, rs = 0;
            ll sum = 0, tag = 0;
        };
        std::vector<Node> nodes;
        
        int begin, end;

        void push_up(int p) {
            auto &ls = nodes.at(nodes.at(p).ls), &rs = nodes.at(nodes.at(p).rs);
            nodes.at(p).sum = ls.sum + rs.sum;
        }

        void push_down(int p, int n_begin, int n_end) {
            if (unlikely(nodes.at(p).tag)) {
                if (nodes[p].ls == 0)  nodes[p].ls = nodes.size(), nodes.emplace_back();
                if (nodes[p].rs == 0)  nodes[p].rs = nodes.size(), nodes.emplace_back();
                auto &ls = nodes.at(nodes[p].ls), &rs = nodes.at(nodes[p].rs);
                auto n_mid = std::midpoint(n_begin, n_end);
                ls.sum += nodes[p].tag * (n_mid - n_begin);
                rs.sum += nodes[p].tag * (n_end - n_mid);
                ls.tag += nodes[p].tag, rs.tag += nodes[p].tag;
                nodes[p].tag = 0;
            }
        }

        void add(int begin, int end, int val, int &p, int n_begin, int n_end) {
            if (p == 0)  p = nodes.size(), nodes.push_back({});
            debug  std::cout << std::format("add({}, {}, {}, {}, {}, {})", begin, end, val, p, n_begin, n_end) << std::endl;
            if (unlikely(n_begin >= begin and n_end <= end)) {
                nodes.at(p).sum += val * (n_end - n_begin);
                nodes.at(p).tag += val;
                return;
            }
            auto n_mid = std::midpoint(n_begin, n_end);
            push_down(p, n_begin, n_end);
            if (begin < n_mid)  add(begin, end, val, nodes.at(p).ls, n_begin, n_mid);
            if (end > n_mid)  add(begin, end, val, nodes.at(p).rs, n_mid, n_end);
            push_up(p);
        }

        ll sum(int begin, int end, int p, int n_begin, int n_end) {
            if (p == 0)  return 0;
            debug  std::cout << std::format("sum({}, {}, {}, {}, {})", begin, end, p, n_begin, n_end) << std::endl;
            if (unlikely(n_begin >= begin and n_end <= end)) {
                // debug  std::cout << "-> " << nodes.at(p).sum << std::endl;
                return nodes.at(p).sum;
            }
            auto n_mid = std::midpoint(n_begin, n_end);
            auto ans = 0LL;
            push_down(p, n_begin, n_end);
            if (begin < n_mid)  ans += sum(begin, end, nodes.at(p).ls, n_begin, n_mid);
            if (end > n_mid)  ans += sum(begin, end, nodes.at(p).rs, n_mid, n_end);
            // std::cout << "-> " << ans << std::endl;
            return ans;
        }

        int findFirst(int begin, int end, int &p, int n_begin, int n_end) {
            debug  std::cout << std::format("findFirst({}, {}, {}, {}, {})", begin, end, p, n_begin, n_end) << std::endl;
            // assert(n_begin <= begin and begin <= end and end <= n_end);
            if (unlikely(n_begin == n_end))  return begin;
            if (unlikely(n_begin + 1 == n_end)) {
                if (sum(n_begin, n_end) > 0)  return begin;
                else  return end;
            }
            if (p == 0)  p = nodes.size(), nodes.emplace_back();

            push_down(p, n_begin, n_end);
            auto n_mid = std::midpoint(n_begin, n_end);

            if (n_end - n_begin >= 2) {
                if (end <= n_mid)  return findFirst(begin, end, nodes.at(p).ls, n_begin, n_mid);
                if (n_mid <= begin)  return findFirst(begin, end, nodes.at(p).rs, n_mid, n_end);
            }
            if (sum(begin, n_mid) > 0) {
                return findFirst(begin, n_mid, nodes.at(p).ls, n_begin, n_mid);
            } else {
                debug  assert(sum(begin, n_mid) == 0);
                return findFirst(n_mid, end, nodes.at(p).rs, n_mid, n_end);
            }
        }

        int findLast(int rbegin, int rend, int &p, int n_begin, int n_end) {
            assert(n_begin <= rend and rend <= rbegin and rbegin <= n_end);
            if (unlikely(n_begin == n_end))  return rbegin;
            if (unlikely(n_begin + 1 == n_end)) {
                if (sum(n_begin, n_end) > 0)  return rbegin;
                else  return rend;
            }
            if (p == 0)  p = nodes.size(), nodes.emplace_back();
            debug  std::cout << std::format("findLast({}, {}, {}, {}, {})", rbegin, rend, p, n_begin, n_end) << std::endl;
            
            push_down(p, n_begin, n_end);
            auto n_mid = std::midpoint(n_begin, n_end);
            if (n_end - n_begin >= 2) {
                if (rbegin <= n_mid)  return findLast(rbegin, rend, nodes.at(p).ls, n_begin, n_mid);
                if (n_mid <= rend)  return findLast(rbegin, rend, nodes.at(p).rs, n_mid, n_end);
            }
            if (sum(n_mid, rbegin) > 0) {
                return findLast(rbegin, n_mid, nodes.at(p).rs, n_mid, n_end);
            } else {
                return findLast(n_mid, rend, nodes.at(p).ls, n_begin, n_mid);
            }
        }
    public:
        SegTree(int begin, int end): nodes(2), begin(begin), end(end) {
            nodes.reserve(8388608);
        }
        void add(int begin, int end, int val) {
            int tmp = 1;
            add(begin, end, val, tmp, this->begin, this->end);
        }
        ll sum(int begin, int end) {
            return sum(begin, end, 1, this->begin, this->end);
        }
        int findFirst(int begin, int end) {
            int tmp = 1;
            return findFirst(begin, end, tmp, this->begin, this->end);
        } 
        int findLast(int rbegin, int rend) {
            int tmp = 1;
            return findLast(rbegin, rend, tmp, this->begin, this->end);
        }
    };
    void solve() {
        const int MaxPos = 1e9+5;
        int N;  io >> N;
        struct Segment {
            int begin, end, index;
        };
        std::vector<std::vector<Segment>> buc(N+1);
        std::vector<int> ans(N, inf);
        SegTree seg(0, MaxPos);
        for (auto i: range(N)) {
            int l, r, c;  io >> l >> r >> c;
            buc.at(c).emplace_back(l, r+1, i);
            seg.add(l, r+1, 1);
        }

        for (auto c: range(buc.size())) {
            for (auto [begin, end, index]: buc[c]) {
                seg.add(begin, end, -1);
            }
            for (auto [begin, end, index]: buc[c]) {
                int cur_ans = inf;
                if (seg.sum(begin, end) == 0) {
                    auto left = seg.findLast(begin, 0), right = seg.findFirst(end, MaxPos);
                    debug std::cout << std::format("[{}]left = {}, right = {}", index, left, right) << std::endl;
                    if (left != 0)  chkMin(cur_ans, begin - left + 1);
                    if (right != MaxPos)  chkMin(cur_ans, right - end + 1);
                } else {
                    cur_ans = 0;
                }
                ans.at(index) = cur_ans;
            }
            for (auto [begin, end, index]: buc[c]) {
                seg.add(begin, end, 1);
            }

        }
        for (auto i: ans)  io << i << ' ';
        io << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    int T;  io >> T;
    while (T --> 0)  Solution_1260521522817291::solve();
    return 0;
}

/*
1
3
1 2 1
3 4 1
5 6 2
 */