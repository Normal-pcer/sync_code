/**
 * 
 */

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


namespace Solution_1779691446829941 {


    void solve() {
        std::ios::sync_with_stdio(false), std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;
        std::cin >> N;

        auto lg = std::__lg(N);
        std::vector<std::pair<int, int>> pairs;
        std::vector<std::vector<size_t>> index(N+5, std::vector<size_t>(lg+2));
        from(j, 0, lg) {
            from(l, 1, N) {
                auto len = 1 << j;
                auto r = l + len - 1;
                if (r > N)  break;

                pairs.push_back({l, r});
                index.at(l).at(j) = pairs.size();
            }
        }

        std::cout << pairs.size() << std::endl;
        for (auto [l, r]: pairs) {
            std::cout << l << ' ' << r << '\n';
        }
        std::cout << std::flush;

        int Q;
        std::cin >> Q;
        from(_, 1, Q) {
            int L, R;
            std::cin >> L >> R;

            auto len = R - L + 1;
            auto lg = std::__lg(len);
            
            auto seg1_len = 1 << (lg);
            auto seg1_left = L;
            auto seg1_index = index.at(seg1_left).at(lg);

            auto seg2_left = R - seg1_len + 1;
            auto seg2_index = index.at(seg2_left).at(lg);

            std::cout << seg1_index << ' ' << seg2_index << std::endl;
        }
    }
}


int main() {
    initDebug;
    Solution_1779691446829941::solve();
    return 0;
}