/**
 * @link https://loj.ac/p/6277
 */
#include "./lib_v5.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

namespace Solution_7163302999878011 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N;  std::cin >> N;
        struct Block {
            std::vector<i32> data;
            i32 add_tag;
        };
        std::vector<Block> blocks;
        
        std::vector<i32> init(N);
        for (auto &x: init)  std::cin >> x;

        auto block_len = static_cast<i32>(std::ceil(std::sqrt(N)));
        assert(block_len != 0);
        blocks.resize(block_len);
        i32 i = 0;
        for (auto &b: blocks) {
            b.data.resize(block_len);
            for (auto &x: b.data) {
                if (i == N)  break;
                x = init[i++];
            }
        }

        auto add_range = [&](i32 begin, i32 end, i32 val) {
            auto front = begin / block_len;  // 第一个散块（包含）
            auto back = end / block_len;  // 最后一个散块（包含）

            auto j_end = (front == back? end % block_len: block_len);
            for (auto j = begin % block_len; j != j_end; j++) {
                blocks[front].data[j] += val;
            }
            if (front == back)  return;

            for (auto j = front + 1; j != back; j++) {
                blocks[j].add_tag += val;
            }
            j_end = end % block_len;
            for (auto j = 0; j != j_end; j++) {
                blocks[back].data[j] += val;
            }
        };

        for (auto q = N; q --> 0; ) {
            i32 op, l, r, c;  std::cin >> op >> l >> r >> c;
            if (op == 0) {
                add_range(l - 1, r, c);
            } else {
                static_cast<void>(l);  // 忽略
                static_cast<void>(c);
                r--;

                auto block_index = r / block_len;
                auto inner_index = r % block_len;
                auto ans = blocks[block_index].data[inner_index] + blocks[block_index].add_tag;
                std::cout << ans << endl;
            }
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_7163302999878011::solve();
    return 0;
}
