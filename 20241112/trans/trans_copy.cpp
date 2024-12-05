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

namespace Solution {

    int N;  const int _N = 8;  // 站点数量
    int K;  const int _K = 8;  // 交通方式
    int T;  // 虚拟换乘时限

    int rv[_N];  // 速度的倒数
    int w[_N];  // 起步价
    int cost[_N];  // 行进 1m 的价格
    struct connection {
        std::vector<int> ways;  // 可选择的交通方式
        int dis;
    } graph[_N];

    // 从 p 到 n 的最小开支
    ll dfs(int p, int prev=-1, int prev2=-1) { 
        if (p == N-1)  return 0LL;  // 抵达终点
        ll ans = infLL;
        auto next = p+1;
        for (auto cur: graph[p].ways) {
            ll co = graph[p].dis * cost[cur];  // 这段路的价格
            debug printf("way %d -> %lld\n", cur, co);
            if (cur == prev) {  // 不换乘
                co += dfs(next, cur, prev);
            } else if (cur == prev2 and  /*上段时间*/graph[p-1].dis * rv[prev] <= T) {
                co += dfs(next, cur, prev);
            } else {
                co += dfs(next, cur, prev) + w[cur];
            }
            chkMin(ans, co);
        }
        debug printf("%d, %d, %d -> %lld\n", p, prev, prev2, ans);
        return ans;
    }

    void solve() {
        io >> N >> K >> T;
        from(i, 0, K-1)  io >> w[i];
        from(i, 0, K-1)  io >> rv[i];
        from(i, 0, K-1)  io >> cost[i];
        from(i, 1, N-1) {
            int d, t;
            io >> d >> t;
            graph[i-1].dis = d;
            from(j, 1, t)  graph[i-1].ways.push_back(io.get());
        }

        io << dfs(0) << endl;
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    Solution::solve();
    return 0;
}