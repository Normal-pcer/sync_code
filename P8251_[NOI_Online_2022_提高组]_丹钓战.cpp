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
#define bitOr(x,y) (((x)&(y))^(((x)^(y))|(~(x)&(y))))
#define Infinity 2147483647
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

namespace Solution {

    int N, Q;  const int _N = 5e5+5;
    struct pair { int l, r; };
    // int next[_N];
    pair arr[_N];

    void init() {
        io >> N >> Q;
        from(i, 1, N)  io >> arr[i].l;
        from(i, 1, N)  io >> arr[i].r;
    }

    void solve() {
        init();

        static int st[_N];
        static int F[24][_N];
        int p = 0;

        from(i, 1, N) {
            while (p and (arr[st[p]].l == arr[i].l or arr[st[p]].r <= arr[i].r)) {
                F[0][st[p]] = i, p--;
            }
            st[++p] = i;
        }

        from(i, 1, 20) {
            from(j, 1, N) {
                F[i][j] = F[i-1][F[i-1][j]];  // 倍增
            }
        }

        while (Q --> 0) {
            int l, r;  io >> l >> r;
            int cnt = 0;
            rev(i, 20, 0) {
                if (0 < F[i][l] && F[i][l] <= r) {
                    cnt += 1<<i;
                    l = F[i][l];
                }
            }
            io << cnt + 1 << endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    Solution::solve();
    return 0;
}