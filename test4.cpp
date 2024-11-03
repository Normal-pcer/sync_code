/**
 * 
 */

#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=(n);i++)
#define from(i,b,e) for(int i=(b);i<=(e);i++)
#define rev(i,e,b) for(int i=(e);i>=(b);i--)
#define main() main(int argc, char const *argv[])
#define optimizeIO std::ios::sync_with_stdio(false); std::cin.tie(0); std::cout.tie(0);
#define chkMax(base,cmp...) (base=std::max({(base),##cmp}))
#define chkMin(base,cmp...) (base=std::min({(base),##cmp}))
#define ensure(v, con, otw) (((v) con)? (v): (otw))
#define never if constexpr(0)
#define always if constexpr(1)
#define bitOr(x,y) (((x)&(y))^(((x)^(y))|(~(x)&(y))))
#define Infinity 2147483647
#define compare(x,y,g,e,l) (((x)>(y))?(g):(((x)<(y))?(l):(e)))
bool DEBUG_MODE=false;
typedef long long ll; typedef unsigned long long ull;
inline void batchOutput(int *begin, int n, const char *format){upto(i, n)printf(format, begin[i]);printf("\n");} inline void batchOutput(int*begin, int n) {batchOutput(begin,n,"%3d ");}
#define batchOutput2d(b, r, c, fmt) upto(i,r){upto(j,c)printf(fmt,b[i][j]);printf("\n");}
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


namespace Solution {

    int P, N = 30002;  const int _N = 30005;

    struct Node { int to, val; } F[_N];
    // int size[_N];

    Node find(int x) {
        if (F[x].to == x)  return {x, F[x].val};
        else {
            auto tmp = find(F[x].to);
            return (F[x] = {tmp.to, tmp.val + F[x].val});
        }
    }

    void connect(int x, int y, int val) {
        Node a = find(x), b = find(y);
        if (a.to == b.to)  return;
        F[a.to] = {b.to, val - b.val + a.val};
    }

    void init() {
        std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
        std::cin >> P;
        from(i, 1, N)  F[i] = {i, 0};
    }

    void solve() {
        init();
        char ch; int x, y;
#if 1
        from(i, 1, P) {
            std::cin >> ch >> x;
            if (ch == 'M') {
                std::cin >> y;
                connect(x, y, 1);
                // log("!!%d\n", find(x).val);
                // from(i, 1, 6) log("[%d]%d %d\n", i, F[i].to, F[i].val);
            } else {
                std::cout << find(x).val << std::endl;
            }
        }
#endif
        // connect(1, 6, 1);
        // connect(2, 4, 1);
        // connect(4, 1, 1);
        // printf("%d %d %d %d %d %d\n", find(1).to, find(1).val, find(4).to, find(4).val, find(2).to, find(2).val);
    }
}


int main() {
    initDebug;
    Solution::solve();
    return 0;
}