/**
 * @link https://www.luogu.com.cn/problem/P2894
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
#define likely(x) (__builtin_expect(!!(x), 1))
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

namespace Solution_1424889618047306 {
    class SegTree {
        struct Node {
            int l, r, len;
            int max = 0, lmax = 0, rmax = 0;
            int tag = 0;  // 1 = 清空，2 = 占用
        };

        std::vector<Node> tr;
#define ls (p << 1)
#define rs (p << 1 | 1)
        void merge(Node &root, const Node &left, const Node &right) {
            root.max = std::max(left.max, right.max);
            root.lmax = left.lmax, root.rmax = right.rmax;

            chkMax(root.max, left.rmax + right.lmax);
            if (left.lmax == left.len)  root.lmax += right.lmax;
            if (right.rmax == right.len)  root.rmax += left.rmax;
        }

        void push_up(int p) {
            merge(tr[p], tr[ls], tr[rs]);
        }

        void push_down(int p) {
            if (likely(tr[p].tag == 0)) {
                return;
            } else if (tr[p].tag == 1) {
                tr[ls].max = tr[ls].lmax = tr[ls].rmax = tr[ls].len;
                tr[rs].max = tr[rs].lmax = tr[rs].rmax = tr[rs].len;
                tr[ls].tag = tr[rs].tag = 1, tr[p].tag = 0;
            } else if (tr[p].tag == 2) {
                tr[ls].max = tr[ls].lmax = tr[ls].rmax = 0;
                tr[rs].max = tr[rs].lmax = tr[rs].rmax = 0;
                tr[ls].tag = tr[rs].tag = 2, tr[p].tag = 0;
            } else {
                assert(false), __builtin_unreachable();
            }
        }

        void build(int l, int r, int p) {
            tr[p].l = l, tr[p].r = r, tr[p].len = r - l + 1;
            tr[p].max = tr[p].lmax = tr[p].rmax = tr[p].len;
            if (l == r) {
                return;
            }
            auto mid = (l + r) >> 1;
            build(l, mid, ls);
            build(mid+1, r, rs);
        }

    public:
        SegTree(int N): tr(N << 2) {
            build(1, N, 1);
        }

        void free(int l, int r, int p = 1) {
            if (tr[p].r < l or tr[p].l > r)  assert(false), __builtin_unreachable();
            if (tr[p].l >= l and tr[p].r <= r) {
                tr[p].max = tr[p].lmax = tr[p].rmax = tr[p].len;
                tr[p].tag = 1;
                return;
            }
            push_down(p);
            if (tr[ls].r >= l)  free(l, r, ls);
            if (tr[rs].l <= r)  free(l, r, rs);
            push_up(p);
        }

        void allocate(int l, int r, int p = 1) {
            if (tr[p].r < l or tr[p].l > r)  assert(false), __builtin_unreachable();
            if (tr[p].l >= l and tr[p].r <= r) {
                tr[p].max = tr[p].lmax = tr[p].rmax = 0;
                tr[p].tag = 2;
                return;
            }
            push_down(p);
            if (tr[ls].r >= l)  allocate(l, r, ls);
            if (tr[rs].l <= r)  allocate(l, r, rs);
            push_up(p);
        }

        int query(int limit, int p = 1) {
            if (tr[p].l == tr[p].r)  return tr[p].l;
            push_down(p);
            if (tr[ls].max >= limit)  return query(limit, ls);
            else if (tr[ls].rmax + tr[rs].lmax >= limit)  return tr[ls].r - tr[ls].rmax + 1;
            else if (tr[rs].max >= limit)  return query(limit, rs);
            else  return 0;
        }
#undef ls
#undef rs
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, M;  std::cin >> N >> M;
        SegTree seg(N);

        for (auto i = 0; i < M; i++) {
            int op, x;  std::cin >> op >> x;
            if (op == 1) {
                auto ans = seg.query(x);
                std::cout << ans << endl;
                seg.allocate(ans, ans + x - 1);
            } else {
                int y;  std::cin >> y;
                seg.free(x, x + y - 1);
            }
        }
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    Solution_1424889618047306::solve();
    return 0;
}
