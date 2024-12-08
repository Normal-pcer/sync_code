/**
 * @link https://www.luogu.com.cn/problem/P1471
 */

#include <bits/stdc++.h>
bool DEBUG_MODE=false;
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define from(i,b,e) for(auto i=(b);i<(e);i++)
#define rev(i,e,b) for(auto i=(e);i>(b);i--)
template <typename T> inline auto chkMax(T& base, const T& cmp) { return (base = std::max(base, cmp)); }
template <typename T> inline auto chkMin(T& base, const T& cmp) { return (base = std::min(base, cmp)); }
#define never if constexpr(0)
#define always if constexpr(1)
const int inf = 0x3f3f3f3f;  const long long infLL = 0x3f3f3f3f3f3f3f3fLL; using ll = long long; using ull = unsigned long long;
const char endl = '\n';

#define __lambda_1(expr) [&](){return expr;}
#define __lambda_2(a, expr) [&](auto a){return expr;}
#define __lambda_3(a, b, expr) [&](auto a, auto b){return expr;}
#define __lambda_4(a, b, c, expr) [&](auto a, auto b, auto c){return expr;}
#define __lambda_overload(a, b, c, d, e, args...) __lambda_##e
#define lambda(...) __lambda_overload(__VA_ARGS__, 4, 3, 2, 1)(__VA_ARGS__)
#define lam lambda
namespace lib{}
#if __cplusplus > 201703LL
namespace ranges { using namespace std::ranges; }
namespace views { using namespace std::ranges::views; }
#endif

using namespace lib;

namespace Solution_8988634283922841 {
    class SegTree {
    public:
        struct Node {
            int l, r, len;
            double sum = 0, square_sum = 0;
            double tag = 0;
        };
        std::vector<Node> tr;
        std::vector<double> const &initializer;

        static double square(double x) noexcept { return x * x; }

#define ls (p<<1)
#define rs (p<<1 | 1)
        void push_up(int p) {
            tr[p].sum = tr[ls].sum + tr[rs].sum;
            tr[p].square_sum = tr[ls].square_sum + tr[rs].square_sum;
        }
        void push_down(int p) {
            if (tr[p].tag) {
                tr[ls].square_sum += square(tr[p].tag) * tr[ls].len + 2 * tr[p].tag * tr[ls].sum;
                tr[rs].square_sum += square(tr[p].tag) * tr[rs].len + 2 * tr[p].tag * tr[rs].sum;
                tr[ls].sum += tr[p].tag * tr[ls].len;
                tr[rs].sum += tr[p].tag * tr[rs].len;
                tr[ls].tag += tr[p].tag, tr[rs].tag += tr[p].tag;
                tr[p].tag = 0;
            }
        }
        void build(int l, int r, int p = 1) {
            tr[p].l = l, tr[p].r = r, tr[p].len = r - l + 1;
            if (l == r) {
                tr[p].sum = initializer.at(l-1), tr[p].square_sum = square(initializer.at(l-1));
                return;
            }
            auto mid = (l + r) >> 1;
            build(l, mid, ls);
            build(mid+1, r, rs);
            push_up(p);
        }
    public:
        SegTree(std::vector<double> const &init): tr(init.size() << 2), initializer(init) {
            build(1, init.size());
        }
        double sum(int l, int r, int p = 1) {
            if (tr[p].l >= l and tr[p].r <= r) {
                return tr[p].sum;
            }
            push_down(p);
            auto res = 0.0;
            if (tr[ls].r >= l)  res += sum(l, r, ls);
            if (tr[rs].l <= r)  res += sum(l, r, rs);
            return res;
        }
        double square_sum(int l, int r, int p = 1) {
            if (tr[p].l >= l and tr[p].r <= r) {
                return tr[p].square_sum;
            }
            push_down(p);
            auto res = 0.0;
            if (tr[ls].r >= l)  res += square_sum(l, r, ls);
            if (tr[rs].l <= r)  res += square_sum(l, r, rs);
            return res;
        }
        void add(int l, int r, double k, int p = 1) {
            if (tr[p].l >= l and tr[p].r <= r) {
                tr[p].square_sum += square(k) * tr[p].len + 2 * k * tr[p].sum;
                tr[p].sum += k * tr[p].len;
                tr[p].tag += k;
                return;
            }
            push_down(p);
            if (tr[ls].r >= l)  add(l, r, k, ls);
            if (tr[rs].l <= r)  add(l, r, k, rs);
            push_up(p);
        }
        bool check(int p = 1) {
            if (tr[p].l == tr[p].r)  return tr[p].square_sum == square(tr[p].sum);
            push_down(p), push_up(p);
            auto res = 0.0;
            for (auto i = tr[p].l; i <= tr[p].r; i++)  res += square_sum(i, i);
            if (res != tr[p].square_sum)  std::cout << std::format("Not equals at {}", p) << std::endl;
            return res == tr[p].square_sum and check(ls) and check(rs);
        }
        double sum_force(int l, int r) {
            auto ans = 0.0;
            for (auto i = l; i <= r; i++)  ans += sum(i, i);
            assert(abs(sum(l, r)-ans) < 1e-4);
            return ans;
        }
        double sqr_sum_force(int l, int r) {
            auto ans = 0.0;
            for (auto i = l; i <= r; i++)  ans += square(sum(i, i));
            assert(abs(square_sum(l, r)-ans) < 1e-4);
            return ans;
        }
#undef ls
#undef rs
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);
        int N, M;  std::cin >> N >> M;
        std::vector<double> init(N);
        for (auto &i: init)  std::cin >> i;

        SegTree seg(init);
        std::cout << std::fixed << std::setprecision(4);
        for (auto _ = 0; _ != M; _++) {
            int op, x, y;  std::cin >> op >> x >> y;
            if (op == 1) {
                double z;  std::cin >> z;
                seg.add(x, y, z);
            } else if (op == 2) {
                std::cout << seg.sum(x, y) / (y - x + 1) << endl;
            } else {
                auto sqr_sum = seg.square_sum(x, y);
                auto sum = seg.sum(x, y);
                auto len = y - x + 1;
                auto avg = sum / len;
                auto variance = sqr_sum / len - avg * avg;
                std::cout << variance << endl;
            }
            never {
                // for (auto i = 1; i <= N; i++) {
                //     std::cout << seg.sum(i, i) << ':' << seg.square_sum(i, i) << ' ';
                // }
                for (auto i: seg.tr | std::views::drop(1)) {
                    std::cout << std::format("[{}][{}, {}] {:.1f}({:.1f}), {:.1f}({:.1f})   ({})", i.len, i.l, i.r, i.sum, seg.sum_force(i.l, i.r), i.square_sum, seg.sqr_sum_force(i.l, i.r), i.tag) << std::endl;
                }
                // assert(seg.check());
                std::cout << std::endl;
            }
        }
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    Solution_8988634283922841::solve();
    return 0;
}
