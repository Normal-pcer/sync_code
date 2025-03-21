/**
 * @link https://loj.ac/p/6278
 */
#include "./lib_v5.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

namespace Solution_3584455995115247 {
    class BlocksManager {
        struct Block {
            std::vector<i64> data;
            std::vector<i64> sorted;
            i64 add_tag;

            auto sort() -> void {
                for (auto &x: data)  x += add_tag;
                add_tag = 0, sorted = data;
                ranges::sort(sorted);
            }
        };
        i32 N = 0;
        i32 block_len = 0;
        std::vector<Block> blocks;

        auto build(std::vector<i32> const &init) -> void {
            for (i32 i = 0; auto &block: blocks) {
                block.data.resize(block_len);
                for (auto &x: block.data) {
                    if (i == N)  break;
                    x = init[i++];
                }
                block.sort();
            }
        }
    public:
        BlocksManager() {}
        BlocksManager(i32 N, std::vector<i32> const &init): N(N), block_len(std::ceil(std::sqrt(N))), blocks(block_len) {
            build(init);
        }

        auto addRange(i32 begin, i32 end, i64 val) -> void {
            auto front_block = begin / block_len;
            auto back_block = end / block_len;

            if (front_block == back_block) {
                for (auto j = begin % block_len; j != end % block_len; j++) {
                    blocks[front_block].data[j] += val;
                }
                blocks[front_block].sort();
                return;
            }

            for (auto j = begin % block_len; j != block_len; j++) {
                blocks[front_block].data[j] += val;
            }
            for (auto j = front_block + 1; j != back_block; j++) {
                blocks[j].add_tag += val;
            }
            for (i32 j = 0; j != end % block_len; j++) {
                blocks[back_block].data[j] += val;
            }
            blocks[front_block].sort(), blocks[back_block].sort();
        }

        auto countRange(i32 begin, i32 end, i64 limit) -> i32 {
            auto front_block = begin / block_len;
            auto back_block = end / block_len;
            i64 res = 0;
            if (front_block == back_block) {
                for (auto j = begin % block_len; j != end % block_len; j++) {
                    res += (blocks[front_block].data[j] + blocks[front_block].add_tag <= limit);
                }
                return res;
            }

            for (auto j = begin % block_len; j != block_len; j++) {
                res += (blocks[front_block].data[j] + blocks[front_block].add_tag <= limit);
            }
            for (auto j = front_block + 1; j != back_block; j++) {
                auto it = ranges::upper_bound(blocks[j].sorted, limit - blocks[j].add_tag);
                auto greater = std::distance(it, blocks[j].sorted.end());
                res += block_len - greater;
            }
            for (i32 j = 0; j != end % block_len; j++) {
                res += (blocks[back_block].data[j] + blocks[back_block].add_tag <= limit);
            }
            return res;
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N;  std::cin >> N;
        std::vector<i32> init(N);
        for (auto &x: init)  std::cin >> x;

        BlocksManager bmgr{N, init};
        for (auto q = N; q --> 0; ) {
            i32 op, l, r, c;  std::cin >> op >> l >> r >> c;
            if (op == 0) {
                bmgr.addRange(l - 1, r, c);
            } else {
                auto ans = bmgr.countRange(l - 1, r, static_cast<i64>(c) * c - 1);
                std::cout << ans << endl;
            }
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_3584455995115247::solve();
    return 0;
}
