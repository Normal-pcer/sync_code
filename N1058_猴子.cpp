/**
 * 
 */

#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define from(i,b,e) for(int i=(b);i<=(e);i++)
#define rev(i,e,b) for(int i=(e);i>=(b);i--)

#define chkMax(base,cmp...) (base=std::max({(base),##cmp}))
#define chkMin(base,cmp...) (base=std::min({(base),##cmp}))
#define never if constexpr(0)
#define always if constexpr(1)
#define bitOr(x,y) (((x)&(y))^(((x)^(y))|(~(x)&(y))))
#define Infinity 0x3f3f3f3f
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
using namespace lib;

namespace Solution_8186795051575562 {
    const int _N = 2005;
    int N, D, M;
    int val[_N], pos[_N];

    void solve() {
        io >> N >> D >> M;
        from(i, 1, N)  io >> val[i] >> pos[i];

        // std::vector<int> ps(N+1);
        // std::partial_sum(pos+1, pos+1+N, ps.begin()+1);

#if false
        static int F[_N][_N];  // 最后一次选中了点 i，一共跳跃 j 次的最大值
        std::memset(F, -0x3f, sizeof(F)), F[1][1] = val[1];

        int ans = 0;
        from(i, 1, N) {
            from(j, 1, M+1) {
                for (auto k=i-1; k>=0 and pos[i]-pos[k] <= D; k--) {
                    chkMax(F[i][j], F[k][j-1] + val[i]);
                }
                chkMax(ans, F[i][j]);
            }
        }
#else

        // 考虑让每次跳都尽可能远
        int ans = 0;
        static int F[_N][_N];
        static int q[_N][_N], l[_N], r[_N];
        std::fill(l, l+M+1, 1);
        std::memset(F, -0x3f, sizeof(F)), F[1][1] = ans = val[1];
        from(j, 1, M+1)  q[j][++r[j]] = 1;
        from(i, 2, N) {
            rev(j, M, 1) {
                auto &tl = l[j];
                auto &tr = r[j];
                auto &tq = q[j];
                while (tr >= tl and pos[i] - pos[tq[tl]] > D)  tl++;
                auto front = tq[tl];
                F[i][j+1] = F[front][j] + val[i], chkMax(ans, F[i][j+1]);
                while (r[j+1] >= l[j+1] and F[q[j+1][r[j+1]]][j+1] <= F[i][j+1])  r[j+1]--;
                q[j+1][++r[j+1]] = i;
            }
        }
#endif
        io << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    Solution_8186795051575562::solve();
    return 0;
}