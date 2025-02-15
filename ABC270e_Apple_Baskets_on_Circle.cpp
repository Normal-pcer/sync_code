/**
 * @link https://www.luogu.com.cn/problem/AT_abc270_e
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

using namespace lib;

#include "./libs/fixed_int.hpp"

/**
 * 在清空一个篮子之前，情况都是相似的
 * 所有还有东西的篮子都需要 -1
 * 维护有多少个篮子被清空，以及剩下的篮子需要减少多少。
 */
namespace Solution_1016303531592735 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N;  std::cin >> N;
        i64 K;  std::cin >> K;

        std::vector<std::pair<i64, i32>> backets(N);
        std::vector<i64> init(N);
        for (auto i = 0; i < N; i++) {
            std::cin >> backets[i].first;
            backets[i].second = i;
            init[i] = backets[i].first;
        }
        ranges::sort(backets);

        std::vector<char> cleared(N);
        std::vector<i32> flag(N);  // 最后额外加多少
        auto it = backets.begin();  // [begin, it) 被清除了
        auto remains = K;  // 剩余的苹果数
        i64 reduce = 0;
        while (remains != 0) {
            i32 not_empty = std::distance(it, backets.end());  // 非空的篮子
            auto reduce_required = remains / not_empty;  // 每个需要减少的数量
            if (reduce_required <= it->first - reduce) {
                reduce += reduce_required;
                assert(it->first - reduce >= 0);
                if (it->first - reduce == 0)  cleared[it->second] = true;
                remains = remains % not_empty;
                // 补上最后的剩余
                i32 cur_backet = 0;
                while (remains != 0) {
                    if (cur_backet == N)  cur_backet = 0;
                    if (not cleared[cur_backet])  flag[cur_backet]--, remains--;
                    cur_backet++;
                }
            } else {
                // 让当前篮子变空
                auto able_to_reduce = it->first - reduce;  // 可以减少的
                reduce += able_to_reduce;
                cleared[it->second] = true;
                remains -= able_to_reduce * not_empty;
                it++;
            }
        }

        for (auto i = 0; i < N; i++) {
            auto cur_final = [&]() -> i64 {
                if (cleared[i])  return 0;
                assert(init[i] > reduce);
                return init[i] - reduce + flag[i];
            }();

            std::cout << cur_final << " ";
        }
        std::cout << std::endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1016303531592735::solve();
    return 0;
}
