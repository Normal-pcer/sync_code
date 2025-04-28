/**
 * @link https://www.luogu.com.cn/problem/P3551
 */
#include "./lib_v7.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

/*
样例可以写成这样的形式：
(c(c(bcb)bb)b(bbc)b)

这样可以方便地看出匹配关系

怎么感觉贪心地取就很有道理
*/
namespace Solution_3293524179377120 {
    auto solve() -> void {
        char constexpr white = 'b';

        i32 n, k;
        std::cin >> n >> k;

        std::string s;
        s.reserve(n);
        std::cin >> s;

        std::vector<std::vector<i32>> ans;
        struct StackItem {
            char ch;
            i32 index;
            i32 whiteCount;
        };
        std::vector<StackItem> stack;
        stack.emplace_back('_', -1, 0);

        for (i32 i = 0; i < n; i++) {
            auto const &ch = s[i];
            stack.emplace_back(ch, i, stack.back().whiteCount + (ch == white));
            
            if (auto size = static_cast<i32>(stack.size()); 
                    size > k + 1 and stack[size - 1].whiteCount - stack[size - k - 2].whiteCount == k) {
                auto &thisAns = ans.emplace_back();
                for (auto _ = k + 1; _ --> 0; ) {
                    thisAns.emplace_back(stack.back().index);
                    stack.pop_back();
                }
            }
        }

        for (auto const &line: ans | views::reverse) {
            for (auto const &x: line | views::reverse) {
                std::cout << x + 1 << " ";
            }
            std::cout << endl;
        }
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_3293524179377120::solve();
    return 0;
}
