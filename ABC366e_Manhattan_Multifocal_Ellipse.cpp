/**
 * @link https://atcoder.jp/contests/abc366/tasks/abc366_e
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
#define chkMaxEx(base,exchange,other,cmp...) {auto __b__=base;if(__b__!=chkMax(base,##cmp)){exchange;} else other;}
#define chkMinEx(base,exchange,other,cmp...) {auto __b__=base;if(__b__!=chkMin(base,##cmp)){exchange;} else other;}
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

;
namespace Solution {
    int N, D;  const int _N = 2e5+5, _V = 4e6+20;

    // std::vector<std::pair<int, int>> points;
    int x_s[_N], y_s[_N];
    ll sx[_N], sy[_N];
    
    void init() {
        scanf("%d%d", &N, &D);
        upto(i, N) {
            int x, y;  scanf("%d%d", &x, &y);
            // points.push_back({x, y});
            x_s[i] = x, y_s[i] = y;
        }
        std::sort(x_s+1, x_s+1+N);
        std::sort(y_s+1, y_s+1+N);
        upto(i, N)  sx[i] = sx[i-1] + x_s[i];
        upto(i, N)  sy[i] = sy[i-1] + y_s[i];
    }

    template <class T, class U>
    ll query(T arr, U sigma, ll x) {
        int pos = std::lower_bound(arr+1, arr+1+N, x) - arr;
        if (pos == N+1) {
            // log("%d -> %d\n", x, (x*N - sigma[N]))
            return (x*N - sigma[N]);
        }
        int len1 = pos - 1, len2 = std::max(N - pos + 1, 0);
        ll sum1 = sigma[pos-1], sum2 = sigma[N] - sigma[pos-1];
        // log("%d -> %d(%d %d %d %d %d)\n", x, (sum2 - x*len2) + (x*len1 - sum1), pos, len1, len2, sum1, sum2)
        return (sum2 - x*len2) + (x*len1 - sum1);
    }

    ll ans_y[_V];
    const int center = 2e6+5;

    void solve() {
        init();
        ll cnt = 0;

        std::memset(ans_y, 0x3f, sizeof(ans_y));
        from(y, -2000002, 2000002) {
            ans_y[y+center] = query(y_s, sy, y); 
            // int ans_x = query(x_s, sx, y);
        }
        std::sort(ans_y, ans_y+_V);
        // from(i, 0, 10)  log("%d ", ans_y[i]);
        
        from(x, -2000002, 2000002) {
            ll ans_x = query(x_s, sx, x);
            int pos = std::upper_bound(ans_y, ans_y+_V, D - ans_x) - ans_y;
            cnt += pos;
        }

        printf("%lld\n", cnt);
    }
}

int main() {;

    initDebug;
    Solution::solve();
    return 0;
}