/**
 * @link https://www.luogu.com.cn/problem/AT_abc261_f
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"

using namespace lib;

/**
 * 显然保证稳定排序更优一些
 * 一个值和前面的逆序对数量即为交换次数。C 值相同的即为省略次数
 */
namespace Solution_1730210819167607 {
    class BIT {
        struct Node {
            i64 val = 0;
            i32 ver = 0;
        };
        std::vector<Node> c;
        i32 current = 0;

        auto static constexpr lowbit(i32 x) -> i32 { return x & -x; }
    public:
        BIT(i32 N): c(N + 1) {}

        auto addAt(i32 x, i32 val) -> void {
            x++;
            while (x < static_cast<i32>(c.size())) {
                if (c[x].ver == current)  c[x].val += val;
                else  c[x] = {val, current};
                x += lowbit(x);
            }
        }
        auto sumPre(i32 x) -> i32 {
            x++;  i32 res = 0;
            while (x != 0) {
                if (c[x].ver == current)  res += c[x].val;
                x -= lowbit(x);
            }
            return res;
        }
        auto clear() -> void {
            current++;
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N;  std::cin >> N;
        std::vector<i32> c(N), val(N);
        for (auto &x: c)  std::cin >> x;
        for (auto &x: val)  std::cin >> x;
        
        BIT bit{N+1};
        i64 ans = 0;
        for (i32 i = 0; i < N; i++) {
            auto all = bit.sumPre(N);
            auto less_eq = bit.sumPre(val[i]);
            auto greater = all - less_eq;
            
            ans += greater;
            
            bit.addAt(val[i], +1);
        }
        std::vector<std::vector<i32>> valx_that_cx_eq_i(N+1);  // c[x] 等于 i 的所有 val[x]
        for (i32 i = 0; i < N; i++) {
            valx_that_cx_eq_i[c[i]].push_back(val[i]);
        }
        for (i32 i = 1; i < N + 1; i++) {
            bit.clear();
            for (auto x: valx_that_cx_eq_i[i] | views::reverse) {
                auto less = bit.sumPre(x - 1);
                ans -= less;
                bit.addAt(x, +1);
            }
        }
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1730210819167607::solve();
    return 0;
}
