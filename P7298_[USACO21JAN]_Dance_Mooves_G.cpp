/**
 * @link https://www.luogu.com.cn/problem/P7298
 */
#include "./lib_v5.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

/*
首先，注意到一个数的轨迹似乎一定是有周期的。

*/
namespace Solution_3095172610102545 {
    auto solve_force() -> void {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N, K, M;  std::cin >> N >> K >> M;
        std::vector<std::pair<i32, i32>> swap_strategies(K);
        for (auto &[x, y]: swap_strategies) {
            std::cin >> x >> y;
            x--, y--;  // 0-index
        }

        std::vector<std::set<i32>> possible_pos(N);
        std::vector<i32> a(N);
        std::iota(a.begin(), a.end(), 0);
        for (i32 i = 0; i < N; i++) possible_pos[i].insert(i);
        auto it_ss = swap_strategies.begin();
        for (auto m = M; m --> 0; ) {
            auto [x, y] = *it_ss;
            std::swap(a[x], a[y]);
            possible_pos[a[x]].insert(x);
            possible_pos[a[y]].insert(y);

            for (auto x: a) std::cout << x << " ";
            std::cout << std::endl;

            it_ss++;
            if (it_ss == swap_strategies.end()) it_ss = swap_strategies.begin();
        }
        for (auto &s: possible_pos) std::cout << s.size() << endl;
    }
    void solve() {
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_3095172610102545::solve_force();
    return 0;
}
