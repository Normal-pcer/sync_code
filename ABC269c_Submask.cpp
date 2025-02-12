/**
 * @link https://www.luogu.com.cn/problem/AT_abc269_c
 */

#include "./lib_v3.hpp"

using namespace lib;

namespace Solution_4998090062044940 {
    template <class T, class Func>
    inline void subset(T s, Func&& f) {  
        for (auto t = s; t; t = (t-1) & s)  f(t);
    }
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        ll N;  std::cin >> N;
        std::vector<ll> ans;
        subset(N, [&](ll x) { ans.push_back(x); });
        ans.push_back(0);

        for (auto x: ans | views::reverse)  std::cout << x << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_4998090062044940::solve();
    return 0;
}
