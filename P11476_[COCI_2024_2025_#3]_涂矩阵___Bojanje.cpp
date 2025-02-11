/**
 * @link https://www.luogu.com.cn/problem/P11476
 */
#ifndef ONLINE_JUDGE
#define GNU_DEBUG
#define _GLIBCXX_DEBUG 1
#define _GLIBCXX_DEBUG_PEDANTIC 1
#define _GLIBCXX_SANITIZE_VECTOR 1
#endif
#include <bits/stdc++.h>
bool DEBUG_MODE=false;
#define debug if(DEBUG_MODE)
template <typename T> inline auto chkMax(T& base, const T& cmp) { return (base = std::max(base, cmp)); }
template <typename T> inline auto chkMin(T& base, const T& cmp) { return (base = std::min(base, cmp)); }
#define never if constexpr(0)
const int inf = 0x3f3f3f3f;  const long long infLL = 0x3f3f3f3f3f3f3f3fLL; using ll = long long; using ull = unsigned long long;
const char endl = '\n';

#define __lambda_1(expr) [&](){return expr;}
#define __lambda_2(a, expr) [&](auto a){return expr;}
#define __lambda_3(a, b, expr) [&](auto a, auto b){return expr;}
#define __lambda_4(a, b, c, expr) [&](auto a, auto b, auto c){return expr;}
#define __lambda_overload(a, b, c, d, e, args...) __lambda_##e
#define lambda(...) __lambda_overload(__VA_ARGS__, 4, 3, 2, 1)(__VA_ARGS__)
#define lam lambda
namespace lib{
#if __cplusplus > 201703LL
namespace ranges = std::ranges;
namespace views = std::views;
#endif
}
using namespace lib;

/**
 * 构造方案，每次填充一行或一列 1 或 2，将全 0 矩阵变为目标矩阵
 * 对于一个全 1 或者全 2 的行列，可以最后填充
 * 然后把这一行/列去掉，作为一个新矩阵继续填充即可
 * N <= 2e3
 * 记录每一行和每一列上 0, 1, 2 的数量，如果：
 * 没有 0，并且 1 或 2 的数量等于当前的长度
 * 直接填充
 */
namespace Solution_4555327211797367 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;  std::cin >> N;
        using Matrix = std::vector<std::vector<int>>;
        Matrix mat(N, std::vector<int>(N));
        for (auto &row: mat) {
            for (auto &x: row)  std::cin >> x;
        }

        auto height = N, width = N;
        std::vector<std::array<int, 3>> row_cnt(N), col_cnt(N);
        for (auto i = 0; i < N; i++) {
            for (auto j = 0; j < N; j++) {
                row_cnt[i][mat[i][j]]++;
                col_cnt[j][mat[i][j]]++;
            }
        }

        struct Operation {
            enum Type {
                Row, Column
            } type;
            int index;
            int val;
        };
        std::vector<Operation> ops;
        while (height != 0 and width != 0) {
            bool filled = false;
            auto fill_row = [&](int r, int val) -> void {
                ops.push_back({Operation::Row, r, val});
                for (auto j = 0; j < N; j++) {
                    auto &old = mat[r][j];
                    if (old != -1) {
                        assert(col_cnt[j][old] != 0);
                        col_cnt[j][old]--;
                        old = -1;
                    }
                }
                height--, filled = true;
                ranges::fill(row_cnt[r], 0);
            };
            auto fill_col = [&](int c, int val) -> void {
                ops.push_back({Operation::Column, c, val});
                for (auto i = 0; i < N; i++) {
                    auto &old = mat[i][c];
                    assert(old != 0);
                    if (old != -1) {
                        assert(row_cnt[i][old] != 0);
                        row_cnt[i][old]--;
                        old = -1;
                    }
                }
                width--, filled = true;
                ranges::fill(col_cnt[c], 0);
            };
            for (auto i = 0; i < N; i++) {
                // 尝试填充第 i 行
                if (row_cnt[i][0] == 0) {  // 要求一定不可以有 0
                    if (row_cnt[i][1] == width)  fill_row(i, 1);
                    else if (row_cnt[i][2] == width)  fill_row(i, 2);
                }
                if (filled)  goto egg;
            }
            for (auto j = 0; j < N; j++) {
                if (col_cnt[j][0] == 0) {
                    if (col_cnt[j][1] == height)  fill_col(j, 1);
                    else if (col_cnt[j][2] == height)  fill_col(j, 2);
                }
                if (filled)  goto egg;
            }

        egg:
            debug {
                std::cout << "------" << endl;
                std::cout << std::format("H: {}, W: {}", height, width) << endl;
                auto [type, index, val] = ops.back();
                auto type_number = static_cast<int>(type) + 1;
                std::cout << type_number << " " << (index + 1) << " " << val << endl;
                for (auto i = 0; i < N; i++) {
                    auto const &row = mat[i];
                    for (auto x: row)  std::cout << std::format("{:2}", x) << " ";
                    std::cout << ": " << row_cnt[i][0] << " " << row_cnt[i][1] << " " << row_cnt[i][2];
                    std::cout << endl;
                }
                for (auto j = 0; j < N; j++) { std::cout << std::format("{:2}", col_cnt[j][0]) << " "; }  std::cout << endl;
                for (auto j = 0; j < N; j++) { std::cout << std::format("{:2}", col_cnt[j][1]) << " "; }  std::cout << endl;
                for (auto j = 0; j < N; j++) { std::cout << std::format("{:2}", col_cnt[j][2]) << " "; }  std::cout << endl;
            }

            if (not filled) {
                // 检查整个矩阵全为 0 或 -1
                auto blank = true;
                for (auto const &row: mat) {
                    for (auto const x: row)  blank &= (x == -1 or x == 0);
                }
                if (blank)  break;
                else {
                    std::cout << "-1" << std::endl;
                    return;
                }
            }
        }

        std::cout << ops.size() << endl;
        for (auto [type, index, val]: ops | views::reverse) {
            auto type_number = static_cast<int>(type) + 1;
            std::cout << type_number << " " << (index + 1) << " " << val << endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_4555327211797367::solve();
    return 0;
}
