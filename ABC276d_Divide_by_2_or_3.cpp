/**
 * @link https://www.luogu.com.cn/problem/AT_abc276_d
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"

using namespace lib;

// 怎么感觉直接写就行
namespace Solution_2466847609570700 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;  std::cin >> N;
        std::vector<i32> vec(N);
        for (auto &x: vec)  std::cin >> x;

        i32 gcd = std::accumulate(vec.begin() + 1, vec.end(), vec.front(), lam(x, y, std::gcd(x, y)));
        i64 ans = 0;
        for (auto x: vec) {
            auto div = x / gcd;
            auto c2 = 0, c3 = 0;
            while (div % 2 == 0)  c2++, div /= 2;
            while (div % 3 == 0)  c3++, div /= 3;
            if (div != 1) {
                std::cout << -1 << endl;
                return;
            }
            ans += c2 + c3;
        }
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_2466847609570700::solve();
    return 0;
}
