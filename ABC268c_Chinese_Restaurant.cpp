/**
 * @link https://www.luogu.com.cn/problem/AT_abc268_c
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

using namespace lib;

/**
 * 先记录每个人需要进行多少次操作才能“开心”
 * 具体地，一个菜的当前位置和目标位置做差加减 1
 * 
 */
namespace Solution_6503771972895951 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;  std::cin >> N;
        std::vector<int> init(N);
        for (auto &x: init)  std::cin >> x;

        // 记录每一个菜种类当前的位置
        std::vector<int> init_pos(N);
        for (auto i = 0; i < N; i++)  init_pos[init[i]] = i;
        
        // 有多少个菜希望被转 i 次
        std::vector<int> wish(N);
        for (auto i = 0; i < N; i++) {
            auto times = (i - init_pos[i] + N) % N;
            auto min = times - 1;
            auto max = times + 1;
            for (auto j = min; j <= max; j++)  wish[(j + N) % N]++;
        }
        assert(std::accumulate(wish.begin(), wish.end(), 0LL) == static_cast<ll>(N) * 3);
        auto ans = std::ranges::max(wish);
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_6503771972895951::solve();
    return 0;
}
