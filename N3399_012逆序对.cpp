/**
 * @link https://neooj.com:8082/oldoj/problem.php?id=3399
 */
#include "lib"
#include "libs/fixed_int.hpp"
using namespace lib;

namespace Solution_3593834255407005 {
    i32 constexpr maxVal = 3;
    i32 constexpr maxRevPairIndex = 3;
    class SegTree {
        struct Node {
            i32 begin = 0, end = 0;
            std::array<i32, maxVal> cnt, replace_tag;  // tag 以 -1 为无效值
            std::array<std::array<i64, maxVal>, maxVal> pair_cnt;

            Node() {
                cnt.fill(0), replace_tag.fill(-1);

                for (auto &line: pair_cnt) {
                    for (auto &item: line) {
                        item = 0;
                    }
                }
            }

            auto replace(i32 from, i32 to) -> void {
                auto new_pair_cnt = pair_cnt;  // 重新计数各种对的数量
                for (i32 other = 0; other < maxVal; other++) {
                    new_pair_cnt[other][to] += new_pair_cnt[other][from];
                    new_pair_cnt[other][from] = 0;
                    new_pair_cnt[to][other] += new_pair_cnt[from][other];
                    new_pair_cnt[from][other] = 0;
                }
                pair_cnt = new_pair_cnt;

                cnt[to] += cnt[from];
                cnt[from] = 0;
                replace_tag[from] = to;
            }
        };
        std::vector<Node> tree;

        auto static constexpr lson(i32 p) -> i32 { return p << 1; }
        auto static constexpr rson(i32 p) -> i32 { return p << 1 | 1; }

        auto pushDown(i32 p) {
            for (i32 i = 0; i < maxVal; i++) {
                if (auto to = tree[p].replace_tag[i]; to != -1) {
                    tree[lson(p)].replace(i, to);
                    tree[rson(p)].replace(i, to);
                    tree[p].replace_tag[i] = -1;
                }
            }
        }
        
    };
    auto solve() -> void {

    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    Solution_3593834255407005::solve();
    return 0;
}
