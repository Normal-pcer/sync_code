/**
 * 
 */
#pragma GCC optimize(3)
#pragma GCC optimize("inline")
#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define from(i,b,e) for(auto i=(b);i<=(e);i++)
#define rev(i,e,b) for(auto i=(e);i>=(b);i--)
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

using namespace lib;


namespace Solution_1686091082382235 {

    constexpr const auto beyond = 20;  // 20! > 1e18

    ll _fac[beyond];  // 阶乘
    const auto _N = 28;
    int N, K;
    ll S;

    int num[_N];

    constexpr void preProcess() {
        _fac[0] = 1;
        from(i, 1, beyond-1)  _fac[i] = _fac[i-1] * i;
    }

    inline ll fac(int x) {
        if (x < 0 or x >= beyond)  return -1;
        else  return _fac[x];
    }

    std::unordered_map<ll, ll> res1[_N]; // res1[i][j]: i 次阶乘，和为 j
    // 左侧 dfs
    void dfs1(int p, ll sum, int used, int limit) {
        // debug std::cout << std::format("dfs1 {} {} {} {}", p, sum, used, limit) << std::endl;
        if (p > limit) {
            if (res1[used].find(sum) != res1[used].end())  res1[used][sum]++;
            else  res1[used].insert({sum, 1});
            return;
        }
        if (used > K)  return;
        if (sum > S)  return; 
        dfs1(p+1, sum, used, limit);  // 不取
        dfs1(p+1, sum+num[p], used, limit);  // 取
        if (0 <= fac(num[p]) and fac(num[p]) <= S) {
            dfs1(p+1, sum+fac(num[p]), used+1, limit);  // 阶乘再取
        }
    }

    auto ans = 0LL;
    void dfs2(int p, ll sum, int used, int limit) {
        // debug std::cout << std::format("dfs2 {} {} {} {}", p, sum, used, limit) << std::endl;
        if (p > limit) {
            auto target = K - used;
            auto remainingSum = S - sum;
            for (auto i = 0; i <= target; i++) {
                ans += res1[i][remainingSum];
            }
            return;
        }
        if (used > K)  return;
        if (sum > S)  return;
        dfs2(p+1, sum, used, limit);
        dfs2(p+1, sum+num[p], used, limit);
        if (0 <= fac(num[p]) and fac(num[p]) <= S) {
            dfs2(p+1, sum+fac(num[p]), used+1, limit);
        }
    }

    void solve() {
        preProcess();
        std::ios::sync_with_stdio(false), std::cin.tie(nullptr), std::cout.tie(nullptr);

        std::cin >> N >> K >> S;
        from(i, 1, N)  std::cin >> num[i];

        auto mid = N >> 1;
        dfs1(1, 0LL, 0, mid);
        never {
            from(i, 1, N) {
                std::cout << "i = " << i << std::endl;
                for (auto [key, val]: res1[i]) {
                    std::cout << std::format("[{}: {}]", key, val) << std::endl;
                }
            }
        }
        dfs2(mid+1, 0LL, 0, N);

        std::cout << ans << std::endl;
    }
}


int main() {
    initDebug;
    Solution_1686091082382235::solve();
    return 0;
}