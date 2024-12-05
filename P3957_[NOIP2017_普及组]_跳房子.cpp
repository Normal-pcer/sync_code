/**
 * 
 */

#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define from(i,b,e) for(int i=(b);i<=(e);i++)
#define rev(i,e,b) for(int i=(e);i>=(b);i--)
#define main() main(int argc, char const *argv[])
#define chkMax(base,cmp...) (base=std::max({(base),##cmp}))
#define chkMin(base,cmp...) (base=std::min({(base),##cmp}))
#define never if constexpr(0)
#define always if constexpr(1)
const int inf = 0x3f3f3f3f;  const long long infLL = 0x3f3f3f3f3f3f3f3fLL;
bool DEBUG_MODE=false;
typedef long long ll; typedef unsigned long long ull;

#define __macro_arg_counter(_1,_2,_3,_4,_5, N, ...) N
#define macro_arg_counter(...)  __macro_arg_counter(__VA_ARGS__,5,4,3,2,1,0)
#define __macro_choose_helper(M,count)  M##count
#define macro_choose_helper(M,count)   __macro_choose_helper(M,count)
#define __lambda_1(expr) [&](){return expr;}
#define __lambda_2(a, expr) [&](auto a){return expr;}
#define __lambda_3(a, b, expr) [&](auto a, auto b){return expr;}
#define __lambda_4(a, b, c, expr) [&](auto a, auto b, auto c){return expr;}
#define lambda(args...) macro_choose_helper(__lambda_, macro_arg_counter(args))(args)
#define lam lambda
namespace lib{}

#include "./libs/io.hpp"
#include "./libs/binary.hpp"
using namespace lib;

namespace Solution_1268115891474192 {

    int N, D; ll K;  const int _N = 5e5+5;
    struct Point { int pos, val; } point[_N];
    // std::mt19937 rnd(std::random_device{}());

    void solve() {
        io >> N >> D >> K;
        from(i, 1, N) {
            point[i] = {io.get(), io.get()};
        }

#if false
        auto check = [&](int g) {
            static ll F[_N];  // 最后一次跳到 i，可能的最高分
            std::memset(F, -0x3f, sizeof(F)), F[0] = 0;

            auto d_min = std::max(1, D-g), d_max = D+g;
            auto ans = -infLL;
            static int q[_N];
            from(i, 1, N) {
                auto max = -infLL;
                for (auto j = i-1; point[i].pos-point[j].pos <= d_max; j--) {
                    if (point[i].pos-point[j].pos >= d_min) {
                        chkMax(max, F[j]);
                    }
                }
                F[i] = max + point[i].val;

                chkMax(ans, F[i]);
            }
            
            debug io << g << " -> " << ans << endl;
            return ans;
        };
#else
        auto check = [&](int g) {
            static ll F[_N];  // 最后一次跳到 i，可能的最高分
            std::memset(F, -0x3f, sizeof(F)), F[0] = 0;

            auto d_min = std::max(1, D-g), d_max = D+g;
            auto ans = -infLL;
            static int q[_N];  // 单调递减
            int l=1, r=0;
            #define luo q[l]
            // q[++r] = 0;
            int ptr = 0;
            from(i, 1, N) {
                // 压入合法元素
                while (ptr<=i and point[i].pos-point[ptr].pos >= d_min) {
                    // 维护递减并压入 ptr
                    while (l<=r and F[q[r]] <= F[ptr])  r--;
                    q[++r] = ptr++;
                }
                // 删除不合法元素
                while (r>=l and point[i].pos-point[luo].pos > d_max)  l++;

                if (r>=l) {
                    auto max = F[luo];
                    F[i] = max + point[i].val;
                }

                chkMax(ans, F[i]);
            }
            
            debug io << g << " -> " << ans << endl;
            return ans;
        };
#endif
        auto min_g = binary::lower_bound_mapping(0, (int)1e9+5, K, check);  // 最小的 g
        if (min_g >= (int)1e9) {
            io << -1 << endl;
        } else {
            io << min_g << endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    Solution_1268115891474192::solve();
    return 0;
}