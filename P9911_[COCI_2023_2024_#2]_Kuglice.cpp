/**
 * @link https://www.luogu.com.cn/problem/P9911
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

/**
 * 双端队列，两个人从任意一端取数
 * 第一次拿到一个颜色即可得分
 * 1 1 2 1 3
 * 
 * 取若干个之后一定剩下一个连续区间 [l, r]
 * F[l][r] 表示一个玩家面对上述区间做出最佳抉择后，比对手多的分数
 * F[l][r] <= -F[l+1][r] + ?
 * 此时取走的数为 a[l]，可以判断一下这个数是否为第一次出现（即 [0, l) Union [r, N) 区间中不存在这个数）
 * 如果是第一次出现，玩家可以获得 1 分
 * 同样地，也可以从 F[l][r+1] 转移
 * 
 * 考虑上述的 check 怎么写。
 * 如果使用记忆化搜索，每层递归都会多一个出现过的数，使用一个 multiset 维护即可。
 */
namespace Solution_7015738533239413 {
    class Game {
        int N;
        std::vector<int> deque;
        std::vector<std::vector<int>> F;

        auto getF(int l, int r, std::vector<int> &counter) -> int {
            if (l > r)  return 0;  // 空区间
            auto check = [&](int x) -> bool { return counter[x] == 0; };

            if (F[l][r] <= -inf) {
                auto take_left = static_cast<int>(check(deque[l]));
                counter.at(deque[l])++;
                take_left -= getF(l+1, r, counter);
                counter.at(deque[l])--;

                auto take_right = static_cast<int>(check(deque[r]));
                counter.at(deque[r])++;
                take_right -= getF(l, r-1, counter);
                counter.at(deque[r])--;

                F[l][r] = std::max(take_left, take_right);
            }

            return F[l][r];
        }
    public:
        Game(int N, std::vector<int> const &deque): 
                N(N), deque(deque), F(N, std::vector(N, -inf)) {}
        auto getAns() -> std::pair<int, int> {
            std::vector<int> counter(N+1);
            auto diff = getF(0, N-1, counter);

            std::vector<int> unique(deque);
            std::sort(unique.begin(), unique.end());
            auto it = std::unique(unique.begin(), unique.end());
            auto K = static_cast<int>(std::distance(unique.begin(), it));
            // a - b = diff, a + b = K（去重后的数量）
            return {(diff + K) >> 1, (K - diff) >> 1};
        };
    };
    void solve() {
        int N;  std::cin >> N;
        std::vector<int> deque(N);
        for (auto &x: deque)  std::cin >> x;

        Game game{N, deque};
        auto [a, b] = game.getAns();
        std::cout << a << ":" << b << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_7015738533239413::solve();
    return 0;
}
