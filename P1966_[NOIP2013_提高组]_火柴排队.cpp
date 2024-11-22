/**
 * @link https://www.luogu.com.cn/problem/P1966
 */

#include <bits/stdc++.h>
bool DEBUG_MODE=false;
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define from(i,b,e) for(auto i=(b);i<(e);i++)
#define rev(i,e,b) for(auto i=(e);i>(b);i--)
#define main() main(int argc, char const *argv[])
template <typename T> inline auto chkMax(T& base, const T& cmp) { return (base = std::max(base, cmp)); }
template <typename T> inline auto chkMin(T& base, const T& cmp) { return (base = std::min(base, cmp)); }
#define never if constexpr(0)
#define always if constexpr(1)
const int inf = 0x3f3f3f3f;  const long long infLL = 0x3f3f3f3f3f3f3f3fLL; using ll = long long; using ull = unsigned long long;

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
#if __cplusplus > 201703LL
namespace rgs {  using namespace std::ranges; using namespace std::views;  }
#endif


using namespace lib;

namespace Solution_1163391625770544 {
    int N;  const int _N = 262'144;
    const int mod = 99'999'997;

    ll c[_N];
    inline int lowbit(int x) { return x & -x; }
    void update(int x, ll val) {
        while (x < _N) {
            c[x] += val;
            if (c[x] >= mod)  c[x] -= mod;
            x += lowbit(x);
        }
    }

    int query(int x) {
        auto res = 0LL;
        while (x) {
            res += c[x];
            if (res >= mod)  res -= mod;
            x -= lowbit(x);
        }
        return res;
    }

    std::vector<int> values = {-inf};
    inline int getIndex(int x) {
        return std::lower_bound(values.begin(), values.end(), x) - values.begin();
    }

    void solve() {
#if true
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);
#endif

        std::cin >> N;
        std::vector<std::pair<int, int>> pairs(N);
        for (auto &pair: pairs)  std::cin >> pair.first;
        for (auto &pair: pairs)  std::cin >> pair.second;
        
        std::vector<int> all_first(N), all_second(N);
        std::transform(pairs.begin(), pairs.end(), all_first.begin(), lam(x, x.first));
        std::transform(pairs.begin(), pairs.end(), all_second.begin(), lam(x, x.second));
        std::sort(all_first.begin(), all_first.end());
        std::sort(all_second.begin(), all_second.end());

        auto mapFirst = [&](int x) {
            return std::lower_bound(all_first.begin(), all_first.end(), x) - all_first.begin();
        };
        auto mapSecond = [&](int x) {
            return std::lower_bound(all_second.begin(), all_second.end(), x) - all_second.begin();
        };

        std::vector<int> height(N), buc(N);
        for (auto i = 0ULL; i < pairs.size(); i++) {
            auto mapped_first = mapFirst(pairs[i].first);
            buc[mapped_first] = i;
        }
        for (auto i = 0ULL; i < pairs.size(); i++) {
            auto mapped_second = mapSecond(pairs[i].second);
            height[i] = buc[mapped_second] + 1;
        }

        debug {
            for (auto i: height) {
                std::cout << i << ' ';
            }
            std::cout << std::endl;
        }

        auto cnt_reversed = 0LL;  // 逆序对数量
        for (auto i = 0; i < N; i++) {
            auto cur = height[i];
            auto cur_leq = query(cur);
            auto cur_greater = i - cur_leq;
            cnt_reversed += cur_greater;
            cnt_reversed %= mod;
            update(cur, 1);
        }

        std::cout << cnt_reversed << std::endl;
    }
}

int main() {
    initDebug;
    Solution_1163391625770544::solve();
    return 0;
}