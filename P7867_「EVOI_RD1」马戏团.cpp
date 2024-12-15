/**
 * @link https://www.luogu.com.cn/problem/P7867
 */

#include <bits/stdc++.h>
bool DEBUG_MODE=false;
#define debug if(DEBUG_MODE)
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

namespace lib {   
    template <typename T> auto range(const T to) {  return std::views::iota(T(), to);  }
    template <typename T> auto range(const T begin, const T end) {  return std::views::iota(begin, end);  }
    template <typename T, typename U> struct StepViewIterator {
        T _cur;  const U _step;
        StepViewIterator(T cur, const U step): _cur(cur), _step(step) {}
        auto operator++() { _cur+=_step; }  auto operator++(int) { _cur+=_step; }
        auto operator!=(const StepViewIterator& other) { return _step>=0? _cur<other._cur: _cur>other._cur; }
        auto operator*() { return _cur; }
    };
    template <typename T, typename U> struct StepView: std::ranges::view_interface<StepView<T, U>> {
        const T _begin, _end;  const U _step;
        StepView(const T begin, const T end, const U step): _begin(begin), _end(end), _step(step) {}
        auto begin() const { return StepViewIterator<T, U>(_begin, _step); }
        auto end() const { return StepViewIterator<T, U>(_end, _step); }
    };
    template <typename T, typename U> auto range(T begin, T end, U step) { return StepView<T, U>(begin, end, step); }
}

using namespace lib;

namespace Solution_7735545344698334 {
    struct Segment {
        int l, r, val;
    };

    class SegTree {
        struct Node {
            int begin, end;
            ll max = 0, add_tag = 0;  // 查询：最大值；修改：累加
        };
        std::vector<Node> tr;
#define ls (p<<1)
#define rs (p<<1|1)
        void pushUp(int p) {
            tr[p].max = std::max(tr[ls].max, tr[rs].max);
        }
        void pushDown(int p) {
            if (tr[p].add_tag) {
                tr[ls].max += tr[p].add_tag, tr[rs].max += tr[p].add_tag;
                tr[ls].add_tag += tr[p].add_tag, tr[rs].add_tag += tr[p].add_tag;
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
        SegTree(int N): tr((N+1)<<2) {
            build(0, N+1);
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
        void add(int begin, int end, int val, int p = 1) {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                tr[p].max += val, tr[p].add_tag += val;
                return;
            }
            pushDown(p);
            if (tr[ls].end > begin)  add(begin, end, val, ls);
            if (tr[rs].begin < end)  add(begin, end, val, rs);
            pushUp(p);
        }
        void assign(int pos, ll val, int p = 1) {
            if (tr[p].begin == pos and pos + 1 == tr[p].end) {
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

        int N, M;  std::cin >> N >> M;
        std::vector<int> cost(N+2);
        for (auto &c: cost | views::drop(1) | views::take(N))  std::cin >> c;

        std::vector<Segment> segs(M);
        std::vector<std::vector<Segment>> right(N+1);
        for (auto &[l, r, val]: segs) {
            std::cin >> l >> r >> val;
        }
        std::sort(segs.begin(), segs.end(), lam(x, y, x.r==y.r? x.l>y.l: x.r>y.r));
        for (auto &[l, r, val]: segs) {
            right.at(r).push_back({l, r, val});
        }

        SegTree sgt(N+1);
        // auto ans = 0LL;
        std::vector<ll> F(N+1);
        for (auto i: range(1, N+1)) {
            for (auto [l, r, val]: right.at(i)) {
                assert(r == i);
                sgt.add(0, l, +val);
            }
            sgt.add(0, i, -cost.at(i));
            F.at(i) = std::max(F.at(i-1), sgt.max(0, i));
            debug  std::cout << F.at(i) << std::endl;
            sgt.assign(i, F.at(i));
        }
        std::cout << F.at(N) << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_7735545344698334::solve();
    return 0;
}
