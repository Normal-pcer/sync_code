/**
 * @link https://www.luogu.com.cn/problem/AT_abc249_f
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

/**
 * 可能让最终结果变大：
 * - 删掉一次赋值操作，赋值前的值大于 x
 * - 删掉一次加负数
 * 一次赋值操作之前的所有操作没有意义
 * 只需要从一次赋值之后加若干个数
 * 把绝对值最大的几个负数删去
 * 
 * 从后往前遍历所有操作，考虑维护最小的若干个负数
 * 记录当前能够保留的最小值，以及需要删除的两种操作数
 * 如果经过一个赋值，可能需要少删一个数。
 * 如果经过一个数，这个数可能会取代其他数。
 * 通过 priority_queue 维护到现在所有数的大小关系即可
 */
namespace Solution_1612129350107929 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, K;  std::cin >> N >> K;
        std::vector<std::pair<int, int>> ops(N);
        for (auto &[x, y]: ops)  std::cin >> x >> y;
        
        auto assign_cnt = 0;  // 丢弃的赋值操作数量
        auto add_cnt = 0;  // 丢弃的加操作数量
        auto ans = -infLL;  // 答案
        auto sum = 0LL;  // 加法的和
        std::priority_queue<int> pq;  // 所有被丢弃的数
        for (auto [x, y]: ops | views::reverse) {
            if (x == 1) {
                // 当前是一次赋值
                chkMax(ans, sum + y);

                // 丢弃这次赋值
                assign_cnt++;
                if (assign_cnt + add_cnt > K) {
                    if (add_cnt == 0)  break;
                    // 需要额外加上一个数
                    // 从 pq 中选择一个最大值
                    auto num = pq.top();  pq.pop();
                    assert(num < 0);
                    sum += num;
                    add_cnt--, assert(add_cnt == static_cast<int>(pq.size()));
                }
            } else if (x == 2) {
                // 一个数
                if (y < 0 and assign_cnt + add_cnt < K) {
                    pq.push(y);
                    add_cnt++;
                } else if (not pq.empty() and y < pq.top()) {
                    assert(y < 0);
                    sum += pq.top();
                    pq.pop(), pq.push(y);
                } else {
                    sum += y;
                }
            }
        }

        if (add_cnt + assign_cnt <= K)  chkMax(ans, sum);
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1612129350107929::solve();
    return 0;
}
