/**
 * @link https://www.luogu.com.cn/problem/P1357
 */
#include "./libs/debug_macros.hpp"

#include <bits/stdc++.h>
bool DEBUG_MODE = false;
#define debug if(DEBUG_MODE)
template <typename T> inline auto chkMax(T &base, const T &cmp) -> T & { return (base = std::max(base, cmp)); }
template <typename T> inline auto chkMin(T &base, const T &cmp) -> T & { return (base = std::min(base, cmp)); }
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
#include "./libs/fixed_int.hpp"
using namespace lib;

/**
 * 「我该在哪里停留，我问我自己。」
 * 没什么，突然想起来这句话了。
 * 
 * M <= 5，自然可以状压记录最后的 M 个。
 * F[i][j]（j 为二进制状态）表示前 i 个花圃中，最后 M 个花圃状态为 j 的方案数。
 * 转移：F[i][j] <- F[i-1][k]，j 的高几位和 k 的第几位匹配，且 popcount(j) 存在上限。
 * 朴素的 O(N) 转移显然无法通过。
 * 
 * 如果把所有可能的 j 编成一个矩阵（只有一行）
 * 每一次的转移方式都是比较规律的，似乎可以用一次矩阵乘法来描述。
 * 例如一个状态 11010 只能从 01101 或 11101 这样的转移过来。
 * 所以 N 次迭代变成了 N 次矩阵乘法，于是可以用矩阵快速幂优化。
 * 
 * 答案矩阵应该是表示“可以从一个状态推到另一个状态”的方案数。自己推到自己即为环。
 */
namespace Solution_8620626636490178 {
    i32 constexpr mod = 1e9 + 7;
    template <typename T>  using ilist = std::initializer_list<T>;
    auto mul(i32 a, i32 b) -> i32 {
        return static_cast<i64>(a) * b % mod;
    }
    class Matrix {
    public:
        i32 height = 0, width = 0;
        std::vector<std::vector<i32>> data;

        Matrix(i32 height, i32 width): height(height), width(width), data(height, std::vector<i32>(width)) {}
        Matrix(ilist<ilist<i32>> init) {
            height = init.size();
            width = ranges::max(init | views::transform(lam(x, x.size())));

            data = std::vector(height, std::vector<i32>(width));

            i32 i = 0;
            for (auto it = init.begin(); it != init.end(); it++, i++) {
                i32 j = 0;
                for (auto it2 = it->begin(); it2 != it->end(); it2++, j++) {
                    data[i][j] = *it2;
                }
            }
        }

        auto operator* (const Matrix &other) const -> Matrix {
            assert(other.height == width);
            Matrix res(height, other.width);

            for (i32 i = 0; i < height; i++) {
                for (i32 j = 0; j < other.width; j++) {
                    for (i32 k = 0; k < other.height; k++) {
                        res.data[i][j] = (res.data[i][j] + mul(data[i][k], other.data[k][j])) % mod;
                    }
                }
            }
            return res;
        }
        auto operator*= (const Matrix &other) -> Matrix & {
            return *this = *this * other;
        }
        auto static identity(i32 N) -> Matrix {
            Matrix res(N, N);
            for (i32 i = 0; i < N; i++) {
                res.data[i][i] = 1;
            }
            return res;
        }

        auto friend qpow(Matrix a, i64 b) -> Matrix {
            assert(a.height == a.width);
            Matrix res = Matrix::identity(a.height);

            for (; b; b >>= 1, a = a * a) {
                if (b & 1)  res = res * a;
            }
            return res;
        }
        auto friend operator>> (std::istream &is, Matrix &mat) -> std::istream & {
            for (i32 i = 0; i < mat.height; i++) {
                for (i32 j = 0; j < mat.width; j++) {
                   is >> mat.data[i][j];
                }
            }
            return is;
        }
        auto friend operator<< (std::ostream &os, Matrix const &mat) -> std::ostream & {
            for (i32 i = 0; i < mat.height; i++) {
                for (i32 j = 0; j < mat.width; j++) {
                    os << mat.data[i][j] << " ";
                }
                os << endl;
            }
            return os;
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i64 N;  i32 M, K;
        std::cin >> N >> M >> K;

        u32 max_stat = (u32)1 << M;
        // 状态的高位表示下标较高的花圃对应颜色
        // 一次即可处理出一个状态可以由谁转移过来。
        // 1 表示 C 形状花圃
        std::vector<std::pair<u32, u32>> edges;
        for (u32 i = 0; i < max_stat; i++) {
            if (std::popcount(i) > K)  continue;
            // 一个状态的低 M-1 位需要和上一个状态的高 M-1 位相同
            u32 common = i & (((u32)1 << (M-1)) - 1);
            edges.push_back({i, common << 1});
            if (std::popcount(common << 1 | 1) <= K)  edges.push_back({i, common << 1 | 1});
        }

        Matrix T{static_cast<i32>(max_stat), static_cast<i32>(max_stat)};
        for (auto [x, y]: edges) {
            T.data[y][x] = 1;  // 很奇怪，建反向边是正确的
        }

        // 答案矩阵和转移矩阵其实是一样的
        auto ans_mat = qpow(T, N);
        i32 ans = 0;
        for (u32 i = 0; i < max_stat; i++) {
            ans = (ans + ans_mat.data[i][i]) % mod;
        }
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_8620626636490178::solve();
    return 0;
}
