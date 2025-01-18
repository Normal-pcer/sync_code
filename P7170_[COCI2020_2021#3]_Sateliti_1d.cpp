#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

/**
 * 先考虑一维的情况：
 * 比较两个序列在何处开始不一样
 * 
 * F[i][j] 表示 [i, i + 2**j) 子串的哈希值
 */
namespace Solution_1751876445283900 {
    constexpr const int base = 131;
    auto constexpr qpow(ull a, ull b) -> ull {
        auto res = 1ULL;
        for (; b; b >>= 1, a = a * a)  if (b & 1) res = res * a;
        return res;
    }
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;  std::cin >> N;
        std::string str;  std::cin >> str;
        str = str + str;

        auto lg = std::__lg(N + N);
        std::vector F(N + N, std::vector<ull>(lg));
        for (auto i: range(N + N))  F[i][0] = str[i];
        for (auto j: range(1, lg)) {
            for (auto i: range(N + N)) {
                if (i + (1 << j) > N + N)  break;
                auto x = qpow(base, 1 << (j - 1));
                F[i][j] = F[i][j-1] + x * F[i + (1 << (j - 1))][j-1];
            }
        }

        debug for (auto const &line: F) {
            for (auto const &item: line) {
                std::cout << item << " ";
            }
            std::cout << std::endl;
        }

        auto cmp = [&](int x, int y) -> bool {
            for (auto j = lg - 1; j --> 0; ) {
                auto hash0 = F[x][j], hash1 = F[y][j];
                if (hash0 == hash1)  x += 1 << j, y += 1 << j;
            }
            return str[x] < str[y];
        };
        auto ans = ranges::min(range(N), cmp);
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1751876445283900::solve();
    return 0;
}
