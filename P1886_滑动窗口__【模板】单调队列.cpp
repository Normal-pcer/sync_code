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


namespace Solution_7972717572875620 {

    int N, K;  const int _N = 1e6+5;
    int a[_N], res[_N];

    void findMin() {
        static int q[_N];
        int l=1, r=0;
        from(i, 1, K) {
            while (l<=r and a[q[r]]>=a[i])  r--;
            q[++r] = i;
        }
        for (auto i=1; i+K-1<=N; i++) {
            while (l<=r and i > q[l])  l++;
            while (l<=r and a[q[r]]>=a[i+K-1])  r--;
            q[++r] = i+K-1;
            auto front = q[l];
            res[i] = a[front];
        }
    }

    void solve() {
        io >> N >> K;
        from(i, 1, N)  io >> a[i];

        findMin();
        from(i, 1, N-K+1)  io << res[i] << ' ';
        io << endl;
        std::transform(a+1, a+1+N, a+1, lam(x, -x));
        findMin();
        std::transform(res+1, res+1+N, res+1, lam(x, -x));
        from(i, 1, N-K+1)  io << res[i] << ' ';
        io << endl;
    }
}


int main() {
    initDebug;
    Solution_7972717572875620::solve();
    return 0;
}