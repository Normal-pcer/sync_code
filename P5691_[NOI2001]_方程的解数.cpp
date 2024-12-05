/**
 * @link https://www.luogu.com.cn/problem/P5691
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

namespace Solution_6627068540932095 {

    int N, M;  const int _N = 7;
    int p[_N], k[_N];

    std::map<ll, int> solutions;  // 能得到该值的方案数
    ll res = 0;

    ll pow(ll a, ll b) {
        auto res = 1LL;
        for (; b; b >>= 1, a = a * a)  if (b&1) res = res * a;
        return res;
    }

    void dfs1(int i, int limit, int val=0) {
        if (i > limit)  return void(solutions[val]++);
        from(x, 1, M) {
            auto cur = (ll)k[i] * pow(x, p[i]);
            dfs1(i+1, limit, val+cur);
        }
    }

    void dfs2(int i, int limit, int val=0) {
        if (i <= limit)  return void(res += solutions[-val]);
        from(x, 1, M) {
            auto cur = (ll)k[i] * pow(x, p[i]);
            dfs2(i-1, limit, val+cur);
        }
    }

    void solve() {
        io >> N >> M;
        from(i, 1, N)  io >> k[i] >> p[i];

        auto mid = N>>1;
        dfs1(1, mid);
        dfs2(N, mid);

        io << res << endl;
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    Solution_6627068540932095::solve();
    return 0;
}