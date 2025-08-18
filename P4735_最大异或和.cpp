/**
 * @link https://www.luogu.com.cn/problem/P4735
 */
#include "lib"
#include "libs/fixed_int.hpp"
using namespace lib;

namespace Solution_2266588065936995 {
namespace {
    class Trie {
        struct Node {
            u32 count{};
            std::array<u32, 2> children;
        };
        std::deque<Node> tree;
        std::vector<u32> roots;

        // auto 
    public:
        Trie() {
            tree.emplace_back();
        }
    };
    auto solve() -> void {

    }
}
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_2266588065936995::solve();
    return 0;
}
