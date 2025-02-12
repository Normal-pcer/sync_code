/**
 * @link https://www.luogu.com.cn/problem/P6278
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

/**
 * 查询 j：两个点 A, B 之间构成逆序对
 * 要求：index[A] < index[B]，min(j, val[A]) > min(j, val[B])
 * 当两个点中较小的那一个小于 j 之后，这一对不再会被计算
 * F[i] 表示：以 i 为较小值的逆序对数量
 * 查询时减掉一个前缀和
 */
namespace Solution_1490717655565874 {
    class BIT {
        std::vector<ll> c;
        static constexpr auto lowbit(int x) -> int { return x & -x; }
    public:
        BIT(int N): c(N+1) {}

        auto prefixSum(int x) const -> ll {
            x++;  auto res = 0LL;
            while (x != 0) {
                res += c[x];
                x -= lowbit(x);
            }
            return res;
        }
        auto prefixAdd(int x, ll val) -> void {
            x++;
            while (x < static_cast<int>(c.size())) {
                c[x] += val;
                x += lowbit(x);
            }
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;  std::cin >> N;
        std::vector<int> val(N);
        for (auto &x: val)  std::cin >> x;

        std::vector<int> F(N+1);
        BIT bit{N+1};
        auto cnt = 0;
        for (auto x: val) {
            auto less_or_eq = bit.prefixSum(x);
            auto greater = cnt - less_or_eq;
            F[x] += greater;
            cnt++;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1490717655565874::solve();
    return 0;
}
