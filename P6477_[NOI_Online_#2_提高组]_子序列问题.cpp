/**
 * @link https://www.luogu.com.cn/problem/P6477
 */

#include "./lib_v3.hpp"


#include "./libs/range.hpp"


#include <bits/stdc++.h>
#include "./libs/mod.hpp"


using namespace lib;
namespace Solution_7422207456822145 {
    const constexpr int mod = 1e9+7;
    template <typename T, int p>
    auto operator% (const Mod<T, p> &x, int) {
        return x;
    }
    class SegTree {
        struct Node {
            int begin, end;
            Mod<ll, mod> square_sum = 0, sum = 0, add_tag = 0;
        };
        std::vector<Node> tr;
#define ls (p<<1)
#define rs (p<<1|1)
        void pushUp(int p) {
            tr[p].sum = (tr[ls].sum + tr[rs].sum) % mod;
            tr[p].square_sum = (tr[ls].square_sum + tr[rs].square_sum) % mod;
        }
        void addNode(int p, auto d) {
            auto s = p;
            auto len = tr[s].end - tr[s].begin;
            tr[s].square_sum = (tr[s].square_sum + tr[s].sum*2*d + d*d*len) % mod;
            tr[s].sum = (tr[s].sum + d*len) % mod;
            tr[s].add_tag = (tr[s].add_tag + d) % mod;
        }
        void pushDown(int p) {
            if (auto d = tr[p].add_tag; d.data != 0) {
                for (auto s: {ls, rs}) {
                    addNode(s, d);
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
        auto squareSum(int begin, int end, int p = 1) -> Mod<ll, mod> {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                return tr[p].square_sum;
            }
            pushDown(p);
            Mod<ll, mod> res = 0LL;
            if (tr[ls].end > begin)  res = (res + squareSum(begin, end, ls)) % mod;
            if (tr[rs].begin < end)  res = (res + squareSum(begin, end, rs)) % mod;
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
#undef ls
#undef rs
    };

    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;  std::cin >> N;
        std::vector<int> a(N+1);
        for (auto &i: a | views::drop(1))  std::cin >> i;
        
        std::map<int, int> last;
        SegTree sgt(0, N+1);
        Mod<ll, mod> ans = 0LL;
        for (auto i: range(1, N+1)) {
            auto prev = last[a.at(i)];
            // debug  std::cout << std::format("add {} {} {}", prev+1, i+1, +1) << std::endl;
            sgt.add(prev+1, i+1, +1);

            // debug  std::cout << std::format("sqrSum {} {} -> {}", 1, i+1, sgt.squareSum(1, i+1)) << std::endl;
            auto cur = sgt.squareSum(1, i+1);
            ans = (ans + cur) % mod;
            // debug  std::cout << cur << std::endl;

            last[a.at(i)] = i;
        }
        std::cout << ans.data << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_7422207456822145::solve();
    return 0;
}
