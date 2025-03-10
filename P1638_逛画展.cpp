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

namespace Solution_1163173797181140 {
    int N, M;  const int _N = 1e6+5, _M = 2e3+5;
    int artist[_N];

    void solve() {
        io >> N >> M;
        from(i, 1, N)  io >> artist[i];

        static int /*q[_N],*/ bu[_M];
        // auto l=1, r=0;
        // auto sum=0LL;
        auto p1=1, p2=1, cnt=0;  // 取 p1, p2 之间
        bu[artist[1]] = cnt = 1;
        std::pair<int, int> ans = {0, inf};

        for (p1=1; p1<=N; p1++) {
            if ((--bu[artist[p1-1]]) == 0)  cnt--;
            while (cnt < M and p2 < N) {
                p2++;
                if (0 == (bu[artist[p2]]++))  cnt++;
                debug io.writeln(p1, p2);
            }
            if (cnt >= M) {
                ans = std::min(ans, {p1, p2}, lam(a, b, a.second-a.first < b.second-b.first));
            }
        }

        auto [l, r] = ans;
        io.writeln(l, r);
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    Solution_1163173797181140::solve();
    return 0;
}