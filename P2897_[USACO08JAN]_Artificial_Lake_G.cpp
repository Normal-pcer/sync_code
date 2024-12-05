/**
 * @link https://www.luogu.com.cn/problem/P2897
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

namespace Solution_6584310475963280 {
    int N;  const int _N = 1e5+5;
    struct level { int width; ll height; };
    level lvl[_N];

    #define hh(i) lvl[i].height
    #define ww(i) lvl[i].width

    void init() {
        io >> N;
        from(i, 1, N)  io >> lvl[i].width >> lvl[i].height;
        lvl[N+1].height = lvl[0].height = Infinity;
    }

    void solve() {
        init();
        auto min = std::min_element(lvl+1, lvl+1+N, lam(a, b, a.height<b.height)) - lvl;
        auto l = min-1, r = min+1;
        ll time = 0;

        static int st[_N], p = 0;  // 单调递减栈，被水流过的阶梯的下标
        st[++p] = min;
        std::vector<ll> res(N+1);  
        from(_, 1, N) {
            auto ex_w = 0;  // 合并被灌水的阶梯
            if (lvl[l].height < lvl[r].height) {  // 走左侧
                // 维护单调递减栈，即将压入 l
                while (p and hh(st[p]) < hh(l)) {  // 一个较矮的
                    ww(st[p]) += ex_w;
                    res[st[p]] = time + ww(st[p]) * 1;  
                    ex_w = ww(st[p]);
                    auto filled = st[p--];
                    time += ww(filled) * (std::min(hh(st[p]), hh(l)) - hh(filled)); 
                }
                ww(l) += ex_w;
                st[++p] = l, l--;  // 压入这级阶梯
            } else {
                // 维护单调递减栈，即将压入 r
                while (p and hh(st[p]) < hh(r)) {  // 一个较矮的
                    ww(st[p]) += ex_w;
                    res[st[p]] = time + ww(st[p]) * 1;  
                    ex_w = ww(st[p]);
                    auto filled = st[p--];
                    time += ww(filled) * (std::min(hh(st[p]), hh(r)) - hh(filled)); 
                }
                ww(r) += ex_w;
                st[++p] = r, r++;  // 压入这级阶梯
            }
        }

        from(i, 1, N)  io << res[i] << endl;
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    Solution_6584310475963280::solve();
    return 0;
}