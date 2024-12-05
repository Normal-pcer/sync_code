/**
 * https://www.luogu.com.cn/problem/AT_abc271_f
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

namespace Solution_1260535371943753 {

    int N;  const int _N = 21;
    int map[_N][_N];
    ll ans = 0;

    std::map<std::pair<std::pair<int, int>, int/*异或和*/>, ll/*方案数*/> cnt;  

    void dfs1(int x, int y, int res=0) {  // 左上到右下
        if (x >= N or y >= N)  return;  // 出界
        auto cur = res ^ map[x][y];
        never io.writeln(x, y, cur);
        if (x+y == N-1) {  // 抵达对角线
            cnt[{{x, y}, res}] += 1;
            return;
        }
        dfs1(x+1, y, cur);
        dfs1(x, y+1, cur);
    }

    void dfs2(int x, int y, int res=0) {  // 右下到左上
        if (x < 0 or y < 0)  return;  // 出界
        auto cur = res ^ map[x][y];
        never io.writeln(x, y, cur);
        if (x+y == N-1) {  // 抵达对角线
            ans += cnt[{{x, y}, cur}];
            return;
        }
        dfs2(x-1, y, cur);
        dfs2(x, y-1, cur);
    }

    void solve() {
        io >> N;
        from(i, 0, N-1) {
            from(j, 0, N-1) {
                io >> map[i][j];
            }
        }

        dfs1(0, 0, 0);
        dfs2(N-1, N-1, 0);

        io << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    Solution_1260535371943753::solve();
    return 0;
}