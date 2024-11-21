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

    int N, M, P; int const _N = 11;
    std::vector<int> required;
    struct node { int to, val; 
        bool operator< (const node& b) const {
            return val > b.val;
        }
    };
    std::vector<node> graph[_N];

    int st[_N];
    ll cnt[_N];
    void dfs1(int p) {
        if (p>N) {
            // 计算最短路
            ll ans = 0;
            for (auto i: required) {
                static bool vis[_N];
                std::priority_queue<node> q;
                std::memset(vis, 0, sizeof(vis));
                q.push({i, 0});
                ll cur = infLL;
                while (not q.empty()) {
                    auto x = q.top();  q.pop();
                    if (vis[x.to])  continue;
                    if (st[x.to]) {
                        cur = x.val;
                        goto egg;
                    }
                    vis[x.to] = true;
                    for (auto &j: graph[x.to]) {
                        if (not vis[j.to])  q.push({j.to, std::max(x.val, j.val)});
                    }
                }
            egg:
                ans += cur;
            }
            chkMin(cnt[std::count(st+1, st+N+1, 1)], ans);
            return;
        }
        st[p] = 0;
        dfs1(p+1);
        st[p] = 1;
        dfs1(p+1);
    }

    void solve() {
        io >> N >> M >> P;
        // if (N>10)  throw "喵";
        from(i, 1, P)  required.push_back(io.get());
        from(i, 1, M) {
            int u, v, w;
            io >> u >> v >> w;
            graph[u].push_back({v, w}), graph[v].push_back({u, w});
        }
        std::memset(cnt, 0x3f, sizeof(cnt));
        dfs1(1);
        from(i, 1, N) {
            io << cnt[i] << ' ';
        }
        io << endl;
    }
}

int main() {
    initDebug;
#ifndef ONLINE_JUDGE
    freopen("internet.in", "r", stdin);
    freopen("internet.out", "w", stdout);
#endif
    Solution::solve();
    return 0;
}