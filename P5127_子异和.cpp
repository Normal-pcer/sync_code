/**
 * @link https://www.luogu.com.cn/problem/P5127
 */
#include "libs/debug_macros.hpp"
#include "lib"
#include "libs/range.hpp"
using namespace lib;

namespace Solution_1245959757901944 {
    const int mod = 1'000'000'007;
    /**
     * 考虑一个连续段上的情况：
     * 按位考虑，每一位上有若干个 1，当取了奇数个 1 的时候会对这一位产生贡献。
     * 1 的个数为奇偶的情况均分，所以为 2**(N-1) 种情况产生贡献。
     * 特别地，如果某一位都是 0，一定不会有贡献。
     * 只需所有数取或，最后乘以 2**(N-1)。
     */
    constexpr auto qpow(ll a, ll b, const int mod) -> ll {
        auto res = 1LL;
        for (; b; b >>= 1, a = a * a % mod)  if (b & 1) res = res * a % mod;
        return res;
    }
    /**
     * 考虑使用线段树维护区间或。
     * 要求支持区间异或操作。
     * 仍然按位考虑，区间异或即为若干位上反转。
     * 某一个位上可能是全 1、全 0 或者二者兼有，仅全 0 时 Or 值为 0
     * 考虑用两个整数表示，a 表示某一位上是否全是 1，b 表示全是 0。
     */
    class SegTree {
        struct Node {
            int begin, end;
            int or_sum = 0, xor_tag = 0;

            auto operator^= (int x) -> Node & {

            }
        };
        std::vector<Node> tr;

        static constexpr auto lson(int p) -> int { return p << 1; }
        static constexpr auto rson(int p) -> int { return p << 1 | 1; }

    };
    void solve() { 
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1245959757901944::solve();
    return 0;
}
