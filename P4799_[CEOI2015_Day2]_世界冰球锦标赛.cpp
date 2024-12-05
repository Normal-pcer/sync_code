/**
 * @link https://www.luogu.com.cn/problem/P4799
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

using namespace lib;

namespace Solution_1414291300285599 {

    int N; ll M;  const int _N = 45;
    ll cost[_N];
    ll total = 0;

    std::vector<ll> records;  // 下标表示计数，值表示求和
    void dfs1(
        int p,      // 搜索起点
        ll val     // 先前的值
    ) {
        if (p > (N>>1))  return records.push_back(val);
        dfs1(p+1, val+cost[p]);
        dfs1(p+1, val);
    }

    void dfs2(int p, ll val) {
        if (p <= (N>>1)) {
            // x + val <= M 即可
            // 找最大的 x <= M - val
            ll index = std::upper_bound(records.begin(), records.end(), M-val) - 1 - records.begin();
            debug std::cout << val << ' ' << index << std::endl;
            total += std::max(0LL, index);
            return;
        }
        dfs2(p-1, val+cost[p]);
        dfs2(p-1, val);
    }

    void solve() {
        std::ios::sync_with_stdio(false), std::cin.tie(nullptr), std::cout.tie(nullptr);
        std::cin >> N >> M;
        from(i, 1, N)  std::cin >> cost[i];

        records.push_back(0);
        dfs1(1, 0);
        std::sort(records.begin(), records.end());
        dfs2(N, 0);

        std::cout << total << std::endl;
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    Solution_1414291300285599::solve();
    return 0;
}