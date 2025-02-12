/**
 * @link https://www.luogu.com.cn/problem/AT_abc269_f
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

using namespace lib;

/**
 * 这个矩阵 A 即为从上到下、从左到右连续排布的正整数序列去掉了若干个奇偶性合适的数（变为 0）
 * 如果没有删掉元素？
 * 每行分别减去 3*M, 4*M, 5*M 这样的即可变得相同
 * 有删去元素，按照奇偶列分别考虑即可。
 * 比如，把奇数列拿出来，每一行均减去 2*M 的若干倍，能把每行变成一样
 */
namespace Solution_6122186285858567 {
    constexpr const int mod = 998244353;
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, M;  std::cin >> N >> M;
        int Q;  std::cin >> Q;
        while (Q --> 0) {
            ll x1, y1, x2, y2;  std::cin >> x1 >> x2 >> y1 >> y2;
            // 行列均改为从 0 开始编号。忍不了。
            x1--, y1--, x2--, y2--;

            auto query = [&](ll x1, ll y1, ll x2, ll y2, auto &&getGreater, auto &&getLess) -> std::pair<int, int> {
                auto odd_col_min = getGreater(y1);
                auto odd_col_max = getLess(y2);
                auto odd_col_count = (odd_col_max - odd_col_min) / 2 + 1;
                // 奇列编号和
                auto odd_col_index_sum = ((odd_col_min + odd_col_max) * odd_col_count / 2) % mod;
                // 奇行
                auto odd_row_min = getGreater(x1);
                auto odd_row_max = getLess(x2);
                auto odd_row_count = (odd_row_max - odd_row_min) / 2 + 1;
                auto odd_row_index_sum = ((odd_row_min + odd_row_max) * odd_row_count / 2) % mod;
                // 编号为 i 行上的所有数应该加上 i * M
                // 奇数行上的奇数列加和
                // 每一行上的元素数量为 odd_col_count
                auto odd_delta_per_row = odd_col_count * M % mod;
                auto odd_extra_sum_of_row = odd_delta_per_row * odd_row_index_sum % mod;  // 由行号提供的贡献
                auto odd_sum_per_row = odd_col_index_sum + odd_col_count;
                auto odd_sum = (odd_sum_per_row * odd_row_count % mod + odd_extra_sum_of_row) % mod;
                auto odd_count = odd_row_count * odd_col_count % mod;
                // std::cout << odd_sum << " " << odd_count << std::endl;
                return {odd_sum, odd_count};
            };

            auto [odd_sum, odd_count] = query(x1, y1, x2, y2, lam(x, x & 1? x: x + 1), lam(x, x & 1? x: x - 1));
            // 偶数
            auto [even_sum, even_count] = query(x1, y1, x2, y2, lam(x, x & 1? x + 1: x), lam(x, x & 1? x - 1: x));
            debug  std::cout << "odd: " << odd_sum << std::endl;
            debug  std::cout << "even: " << even_sum << std::endl;
            std::cout << (odd_sum + even_sum) % mod << endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_6122186285858567::solve();
    return 0;
}
