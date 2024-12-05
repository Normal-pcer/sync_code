/**
 * @link https://www.luogu.com.cn/problem/P1776
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

namespace Solution_1040875916856176 {

    int N, W;  const int _W = 4e4+5;
    struct Object { ll v, w; };
    std::vector<Object> obj;

    void solve() {
        io >> N >> W;
        from(i, 1, N) {
            ll v, w, m;  io >> v >> w >> m;

            auto j=1LL;
            for (; (j<<1)<=m; j<<=1) {
                obj.push_back({v*j, w*j});
            }
            auto sum = j - 1;
            if (sum != m)  obj.push_back({v*(m-sum), w*(m-sum)});
        }

        debug for (auto &[v, w]: obj) {
            io.writeln(v, w);
        }

        static ll F[_W], ans = -infLL;
        std::memset(F, -0x3f, sizeof(F)), F[0] = 0;
        for (auto &[v, w]: obj) {
            rev(j, W, 1) {
                if (j-w < 0)  break;
                chkMax(F[j], F[j-w] + v), chkMax(ans, F[j]);
            }
        }

        io << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    Solution_1040875916856176::solve();
    return 0;
}