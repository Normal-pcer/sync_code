/**
 * @link https://www.luogu.com.cn/problem/AT_abc264_d
 * @link https://atcoder.jp/contests/abc264/tasks/abc264_d
 */

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_1136351056949233 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);
        std::string s;  std::cin >> s;
        std::string target = "atcoder";
        auto ans = 0;
        std::vector<int> vec(7);
        ranges::transform(s, vec.begin(), lam(x, std::distance(target.begin(), ranges::find(target, x))));
        for (auto _: range(7)) {
            for (auto j: range(7)) {
                if (vec[j-1] > vec[j]) {
                    std::swap(vec[j-1], vec[j]), ans -=- 1;
                    debug  for (auto x: vec)  std::cout << x << ' ';
                    debug  std::cout << endl;
                }
            }
        }
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1136351056949233::solve();
    return 0;
}
