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
typedef unsigned int ui;

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

    std::mt19937 rnd(745184);

    const int _N = 5e5+5;
    ui rand[_N];

    int N;  // 点数
    int M;  // 边数
    int Q;  // 询问次数

    ull c[_N];  // 对于一条从 i 出发的边，认为边权为 rand[i]。所有终点为 i 的边的边权和为 c[i]
    ull sum;  // c[i] 的求和
    ull target;  // 期望的求和
    ull origin[_N];  // 记录每个点的原始状态

    void init() {
        io >> N >> M;
        from(i, 0, N)  rand[i] = rnd();
        from(_, 1, M) {
            int x, y;
            io >> x >> y;  // x->y 连边
            c[y] += rand[x];
        }
        from(i, 1, N)  origin[i] = c[i];
        sum = std::accumulate(c+1, c+1+N, 0ULL);
        from(i, 1, N)  target += rand[i];
        io >> Q;
    }

    void solve() {
        init();
        
        from(_, 1, Q) {
            int op, u;
            io >> op >> u;
            if (op == 1) {  // 摧毁一条连接
                auto v = io.get();
                c[v] -= rand[u];
                sum -= rand[u];
            } else if (op == 2) {  // 摧毁一个据点
                sum -= c[u];
                c[u] = 0;
            } else if (op == 3) {  // 重建一条连接
                auto v = io.get();
                sum += rand[u];
                c[v] += rand[u];
            } else {  // 修复一个据点
                sum += (origin[u] - c[u]);
                c[u] = origin[u];
            }
            if (sum == target) {
                io << "YES" << endl;
            } else {
                io << "NO" << endl;
            }
        }
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    Solution::solve();
    return 0;
}