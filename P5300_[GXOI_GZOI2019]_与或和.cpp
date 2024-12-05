/**
 * @link https://www.luogu.com.cn/problem/P5300
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

namespace Solution {

    const int mod = 1e9+7;
    const int _N = 1005;
    int N;
    unsigned super[_N][_N];
    
    void init() {
        io >> N;
        from(i, 1, N)  from(j, 1, N) {
            io >> super[i][j];
        }
    }

    int sub(int x, int l, int m, int r) {
        auto a = (m-l+1) * (r-m+1) % mod;
        return (ll)a * x % mod;
    }

    template <class arr>
    int count(arr&& src) {
        ll tot = 0;
        static int cnt[_N][_N];  // 从该点向上有几个连续的 1
        from(digit, 0, 31) {
            const unsigned mask = 1U<<digit;  // 掩码
            ll ans = 0;  // 记录这一位的答案
            from(i, 1, N)  from(j, 1, N) {
                if (src[i][j] & mask)  cnt[i][j] = cnt[i-1][j] + 1;
                else  cnt[i][j] = 0;
            }

            from(bottom, 1, N) {  // 枚举底边
                auto cur = cnt[bottom];  // 当前行
                static int st[_N];  // 存储下标，对应值单调不减
                // static int st2[_N];  // 存储下标，对应值单调递增
                int p = 0;
                int ans_line = 0;  // 本行产生的贡献

#if false  // 错误的实现思路
                from(i, 1, N+1) {  // 从左到右遍历，右边界视为 0
                    auto last = i;  // 最后一个被弹出的元素
                    bool flag = false;  // 是否有元素在本次流程中被弹出
                    while (p and cur[st[p]] > cur[i]) {
                        // 一个元素将要被弹出
                        // 它的上端可以直接被结算
                        auto height = cur[st[p]] - cur[i];  // 比当前元素高的部分
                        auto width = i - st[p];  // 宽度
                        // 上端的一个长方形区域，计算这部分的子矩阵数量
                        auto cnt1 = sub(height, width);
                        // 减去只在右边的部分
                        auto passed = i - last;  // 需要减去的宽度；高度依然是 height
                        auto cnt2 = sub(height, passed);
                        // 减去只在上边的部分
                        // 故这次结算出来的数量为 cnt1 - cnt2
                        assert(cnt1-cnt2>=0), ans_line = (ans_line - cnt2 + cnt1) % mod;
                        last = st[p--], flag = true;  // 记录并弹出当前元素
                    }

                    if (flag)  cur[st[++p]] = cur[i];  // 截断
                    else  st[++p] = i;
                }

                debug io << std::format("ans of line {}: {}", bottom, ans_line) << endl;
#endif
                // 对于每一列，寻找分别寻找它左侧第一个低于它的，和右侧第一个不高于它的列
                std::vector<int> left(N+5);  // 记录左侧
                from(i, 1, N+1) {
                    while (p and cur[st[p]] >= cur[i]) {
                        // 栈顶元素的右侧添加了一个更高的
                        auto height = cur[st[p]];
                        auto l = left[st[p]] + 1;
                        auto r = i - 1;
                        ll ans_this = sub(height, l, st[p], r);
                        ans_line += ans_this;  // 记录必须包含这一列的方法总数
                        ans_line %= mod;
                        p--;
                    }

                    // 此时 st2[p] 栈顶的对应值严格小于 i
                    // 符合条件，直接记录
                    left[i] = st[p];

                    st[++p] = i;  // 将 i 压入两个栈
                }

                ans = (ans + ans_line) % mod;
                never io << std::format("({})ans of line {}: {} => {}", digit, bottom, ans_line, ans) << endl;
            }
            tot += (ll)ans * (ll)mask % mod;
            never io << std::format("TOT is {} now.", tot) << endl;
            tot %= mod;
        }
        return tot;
    }

    void solve() {
        init();

        auto ans1 = count(super);
        from(i, 1, N)  from(j, 1, N)  super[i][j] = ~super[i][j];
        auto tot = 294967267LL * (N*(N+1)/2%mod) % mod * (N*(N+1)/2%mod) % mod;
        auto ans2 = (tot - count(super) + mod) % mod;
        io.writeln(ans1, ans2);
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    Solution::solve();
    return 0;
}