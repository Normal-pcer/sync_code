/**
 * @link https://www.luogu.com.cn/problem/P2579
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
 * 2 3 4 的 lcm 为 12。
 * 第 j 轮迭代的 A[i] 矩阵表示固定走 j * 12 + i 步的邻接矩阵。
 * 每次经过 12 轮，所有鱼的路径一定经过整数个周期。
 * 
 * 先暴力跑 12 轮，接下来的 12N 轮可以直接通过矩阵快速幂转移。
 */
namespace Solution_5913354425395240 {
    i32 constexpr mod = 10000;
    auto mul(i32 a, i32 b) -> i32 {
        return static_cast<i64>(a) * b % mod;
    }
    class Matrix {
        i32 height, width;
        std::vector<std::vector<i32>> data;
    public:
        Matrix(i32 height, i32 width): height(height), width(width), data(height, std::vector<i32>(width)) {}

        auto operator[](uz idx) const -> std::vector<i32> const & { return data[idx]; }
        auto operator[](uz idx) -> std::vector<i32> & { return data[idx]; }
        auto operator* (Matrix other) const -> Matrix {
            assert(width == other.height);
            Matrix res{height, other.width};

            for (i32 i = 0; i < height; i++) {
                for (i32 j = 0; j < other.width; j++) {
                    for (i32 k = 0; k < width; k++) {
                        res[i][j] = (res[i][j] + mul(data[i][k], other[k][j])) % mod;
                    }
                }
            }
            return res;
        }

        auto static identity(i32 N) -> Matrix {
            Matrix res{N, N};
            for (i32 i = 0; i < N; i++) {
                res[i][i] = 1;
            }
            return res;
        }

        auto qpow(i64 b) -> Matrix {
            assert(height == width);
            auto a = *this, res = identity(height);
            for (; b; b >>= 1, a = a * a) {
                if (b & 1)  res = res * a;
            }
            return res;
        }

        auto friend operator<< (std::ostream &os, Matrix const &mat) -> std::ostream & {
            for (i32 i = 0; i < mat.height; i++) {
                if (i != 0)  os << endl;
                for (i32 j = 0; j < mat.width; j++) {
                    if (j != 0)  os << " ";
                    os << mat[i][j];
                }
            }
            return os;
        }
    };
}

using Solution_5913354425395240::Matrix;
template <>
struct std::formatter<Matrix, char> {
    template <typename T>
    auto constexpr parse(T &context) const -> T::iterator {
        auto it = context.begin();
        while (it != context.end() and *it != '}')  it++;
        return it;
    }

    template <typename T>
    auto format(Matrix const &mat, T &context) const -> T::iterator {
        std::stringstream ss;
        ss << mat;
        return ranges::copy(ss.str(), context.out()).out;
    }
};

namespace Solution_5913354425395240 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 node_count, edge_count, start, end, time_need;
        std::cin >> node_count >> edge_count >> start >> end >> time_need;

        std::vector A(12, Matrix{1, node_count});  // 每个矩阵：到达某个点的方案数
        Matrix graph{node_count, node_count};
        for (auto e = edge_count; e --> 0; ) {
            i32 x, y;  std::cin >> x >> y;
            graph[x][y] = graph[y][x] = 1;
        }

        i32 fish_count;  std::cin >> fish_count;
        std::vector<std::vector<i32>> routes(fish_count);
        for (auto &x: routes) {
            i32 t;  std::cin >> t;
            x.resize(t);
            for (auto &item: x)  std::cin >> item;
        }

        i32 term_count = time_need / 12;
        i32 remains = time_need % 12;

        // 记录每个鱼此时的位置
        std::vector<std::vector<i32>::iterator> routes_cur(fish_count);
        for (i32 i = 0; i < fish_count; i++)  routes_cur[i] = routes[i].begin();

        auto Once = Matrix::identity(node_count);  // 乘以这个矩阵相当于转移 12 次
        for (i32 t = 0; t < 12; t++) {
            Once = Once * graph;
            for (i32 f = 0; f < fish_count; f++) {
                ++routes_cur[f];
                if (routes_cur[f] == routes[f].end())  routes_cur[f] = routes[f].begin();
                auto fish_pos = *routes_cur[f];
                for (i32 prev = 0; prev < node_count; prev++) {
                    Once[prev][fish_pos] = 0;
                }
            }
        }

        auto ans_mat = Once.qpow(term_count);
        for (i32 t = 0; t < remains; t++) {
            ans_mat = ans_mat * graph;
            for (i32 f = 0; f < fish_count; f++) {
                ++routes_cur[f];
                if (routes_cur[f] == routes[f].end())  routes_cur[f] = routes[f].begin();
                auto fish_pos = *routes_cur[f];
                for (i32 prev = 0; prev < node_count; prev++) {
                    ans_mat[prev][fish_pos] = 0;
                }
            }
            debug  std::cout << std::format("t = {}  ans_mat =\n{}", t, ans_mat) << std::endl;
        }
        auto ans = ans_mat[start][end];
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_5913354425395240::solve();
    return 0;
}
