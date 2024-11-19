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
using namespace lib;


namespace Solution_1551233729792707 {

    int N, P, Q;  const int _N = 1e5+5;
    int W[_N];

    void solve() {
        io >> N >> P >> Q;
        from(i, 1, N)  io >> W[i];
        W[N+1] = -inf;

        static ll ps[_N];
        std::partial_sum(W, W+N+1, ps);

        static int q[_N], l=1, r=0;
        // q[++r] = 0;

        auto ans = -infLL;
        from(i, 1, N) {
            // 清除不合法
            while (r>=l and i-q[l] > Q)  l++;

            // 将合法的压入队列
            // x = i-P 在这一轮变得合法
            auto x = i - P;
            if (x >= 0) {
                // 更劣解出队
                while (r>=l and ps[q[r]] >= ps[x])  r--;
                q[++r] = x;
            }

            if (r>=l) {
                auto front = q[l];
                auto val = ps[i] - ps[front];
                chkMax(ans, val);
            }

            debug {
                from(i, l, r) {
                    io << q[i] << ' ';
                }
                io << endl;
            }
        }

        io << ans << endl;
    }
}


int main() {
    initDebug;
    Solution_1551233729792707::solve();
    return 0;
}