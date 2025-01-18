/**
 * @link https://www.luogu.com.cn/problem/P7170
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

/**
 * 先考虑一维的情况：
 * 比较两个序列在何处开始不一样
 * 
 * F[i][j] 表示 [i, i + 2**j) 子串的哈希值
 * 
 * 再考虑换成二维：
 * 先确定哪一行不同，再判断具体哪一个元素不同
 * F[i][j][k] 表示第 i 行中 [j, j + 2**k) 子串的哈希值
 * G[p][i][j] 表示 [i, i + 2**j) 行中每行的整行哈希值组成序列的哈希值，将 p 视为一行的开始
 */
namespace Solution_6710300631688193 {
    constexpr const int base = 131;
    constexpr const int base_rows = 13331;  // 行间的进制
    auto constexpr qpow(ull a, ull b) -> ull {
        auto ans = 1ULL;
        for (; b; b >>= 1, a = a * a)  if (b & 1) ans = ans * a;
        return ans;
    }
    auto constexpr lg_ceil(int x) -> int {
        auto lg = std::__lg(x);
        return lg + ((1 << lg) != x);
    }
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, M;  std::cin >> N >> M;
        std::vector<std::string> matrix(N + N);
        for (auto i: range(N)) {
            std::cin >> matrix[i];
            matrix[i] = matrix[i] + matrix[i];
            matrix[i + N] = matrix[i];
        }
        N += N, M += M;
        auto lgN = std::__lg(N) + 1, lgM = std::__lg(M) + 1;

        std::vector F(N, std::vector(M, std::vector<ull>(lgM)));
        std::vector G(M, std::vector(N, std::vector<ull>(lgN)));
        for (auto i: range(N)) {
            for (auto j: range(M)) {
                F[i][j][0] = matrix[i][j];
            }
            for (auto k: range(1, lgM)) {
                for (auto j: range(M)) {
                    if (j + (1 << k) > M)  break;
                    auto x = qpow(base, 1 << (k - 1));
                    F[i][j][k] = F[i][j][k-1] + x * F[i][j + (1 << (k-1))][k-1];
                }
            }
        }
        for (auto p: range(M >> 1)) {
            std::vector<ull> row_hash(N);
            for (auto i: range(N)) {
                auto res = 0ULL;
                for (auto j: range(p, p + (M >> 1)))  res = res * base + matrix[i][j];
                row_hash[i] = res;
            }
            for (auto i: range(N)) {
                G[p][i][0] = row_hash[i];
            }
            for (auto j: range(1, lgN)) {
                for (auto i: range(N)) {
                    if (i + (1 << j) > N)  break;
                    auto x = qpow(base_rows, 1 << (j - 1));
                    G[p][i][j] = G[p][i][j-1] + x * G[p][i + (1 << (j-1))][j-1];
                }
            }
        }

        using Point = std::pair<int, int>;
        auto cmp = [&](Point a, Point b) -> bool {
            // 先找到第一个存在差异的行
            auto [r0, c0] = a;
            auto [r1, c1] = b;
            for (auto j = lg_ceil(N >> 1); j --> 0; ) {
                if (r0 + (1 << j) >= N)  continue;
                auto hash0 = G[c0][r0][j], hash1 = G[c1][r1][j];
                if (hash0 == hash1)  r0 += 1 << j, r1 += 1 << j;
            }

            // 在这两行中进行比较
            for (auto j = lg_ceil(M >> 1); j --> 0; ) {
                if (c0 + (1 << j) >= M)  continue;
                auto hash0 = F[r0][c0][j], hash1 = F[r1][c1][j];
                if (hash0 == hash1)  c0 += 1 << j, c1 += 1 << j;
            }

            debug  std::cout << std::format("({}, {}) < ({}, {}) -> {} (diff {}, {} / {}, {})", a.first, a.second, b.first, b.second, matrix[r0][c0] < matrix[r1][c1], r0, c0, r1, c1) << std::endl;
            return matrix[r0][c0] < matrix[r1][c1];
        };

        auto ans = Point{0, 0};
        for (auto i: range(N >> 1)) {
            for (auto j: range(M >> 1)) {
                if (matrix[i][j] == '.')  continue;
                ans = std::min(ans, Point{i, j}, cmp);
            }
        }
        debug  std::cout << ans.first << " " << ans.second << endl;
        for (auto i: range(ans.first, ans.first + (N >> 1))) {
            for (auto j: range(ans.second, ans.second + (M >> 1))) {
                std::cout << matrix[i][j];
            }
            std::cout << endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_6710300631688193::solve();
    return 0;
}
