/**
 * @link https://www.luogu.com.cn/problem/P7153
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"
using namespace lib;

/**
 * 先考虑如何判断一个集合是否可以被框住。肯定是希望弄一个尽可能小的正方形。
 * 先用一个矩形框住它，然后向外延伸变成正方形。
 * 假设这个矩形的宽 大于 高。
 * 之后，在它的上面找一个最近的格子，下面找一个最近的格子。
 * 如果两个格子之间的竖直距离大于等于矩形的宽，那样就可行。
 * 
 * 枚举集合当然没有戏，考虑枚举正方形。
 * 枚举正方形的边长。由于希望它尽可能小，所以让它的上下两条边正好框住两个格子。
 * 接下来枚举左边即可确定一个正方形。
 * 每次让左边跑到一个格子的右面，这样似乎可以保证：这个正方形框出来的区域，要么是空集，要么是一个新的集合。
 * 不太对，这似乎只能保证在这一行中是唯一的？
 * 让枚举每一行的正方形都互不相同，可以做到吗？似乎是可行的，例如强制要求最高的格子和最低的格子必须同框。
 * 那么左端点可能的范围可以按照如下描述：
 * 从最高点和最低点第一次同框，到有一个被移出。
 * 每次右移让一个新的点移进框内或者移到框外。
 * 
 * 这样能够保证是唯一的。因为每一行一定包括最高点和最低点，并且一定不包括更外侧的点。
 * 复杂度：O(N**3) 左右，N <= 200，可以接受
 * 
 * 完蛋，写假了。正方形也可能是靠宽撑起来的。
 * 可以旋转过来再做一遍。恰好在正方形对角线上的两个点可能会被重复算，大概需要去重。
 * 去重：如果一个正方形的四条边上都有格子，那么一定会被算两次，-1 即可
 */
namespace Solution_9263732586291096 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N;  std::cin >> N;
        struct Block {
            i32 r, c;
        };
        std::vector<Block> blocks(N);  // 被涂色的格子
        for (auto &[x, y]: blocks)  std::cin >> x >> y;

        auto calc = [&](std::vector<Block> const &blocks, bool unique) -> i32 {
            // 每行上唯一的格子，每列上唯一的格子
            std::map<i32, i32> row_block, col_block;
            for (i32 i = 0; i < N; i++) {
                auto [x, y] = blocks[i];
                row_block[x] = i;
                col_block[y] = i;
            }

            i32 ans = 0;
            // 枚举最低点和最高点
            // 这里定义：最低点的 x 值比较大
            std::map<i32, i32> col_block_inner;  // 被夹在 low 与 high 中间
            for (auto [high, high_block]: row_block) {
                col_block_inner.clear();
                col_block_inner[blocks[high_block].c] = high_block;
                for (auto [low, low_block]: row_block) {
                    if (not (low > high))  continue;
                    col_block_inner[blocks[low_block].c] = low_block;

                    auto len = low - high;  // 边长
                    auto left_block = low_block, right_block = high_block;  // 更靠左、更靠右的极点
                    if (blocks[left_block].c > blocks[right_block].c)  std::swap(left_block, right_block);

                    // 初始状态：恰好把更靠右的极点囊括进来
                    auto l = blocks[right_block].c - len;  // 初始左端点
                    auto it_next_l = col_block_inner.lower_bound(l);  // 指向下一个大于 l 的列号
                    auto it_next_r = col_block_inner.upper_bound(l + len);

                    // 最终状态：刚好错过更靠左的极点
                    auto final_l = blocks[left_block].c;
                    for (; l <= final_l; ) {
                        debug  std::cout << "ans: " << low << " " << l << " " << high << " " << l + len << std::endl;
                        ans++;

                        // 额外去重
                        if (unique and col_block_inner.contains(l) and col_block_inner.contains(l + len)) {
                            // std::cout << "ans--" << std::endl;
                            ans--;
                        }

                        // 跳到下一个有效的左端点
                        // 要么右面再加一个，要么左面减一个
                        auto l1 = it_next_l != col_block_inner.end()? it_next_l->first + 1: inf;
                        auto l2 = it_next_r != col_block_inner.end()? it_next_r->first - len: inf;
                        if (l1 <= l2) {  // 左面减一个
                            ++it_next_l;
                            l = l1;
                        }
                        if (l2 <= l1) {  // 右面加一个
                            ++it_next_r;
                            l = l2;
                        }
                        // 如果 l1 == l2 恰好是期望的结果

                        
                    }
                }
            }
            return ans;
        };

        auto ans = calc(blocks, true);
        // 转过来再做一遍
        for (auto &[x, y]: blocks)  std::swap(x, y);
        debug  std::cout << "calc" << std::endl;
        ans += calc(blocks, false);

        // 算上单个块
        ans += N + 1;

        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_9263732586291096::solve();
    return 0;
}
