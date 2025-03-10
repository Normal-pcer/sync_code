/**
 * @link https://www.luogu.com.cn/problem/P2151
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
 * 原图中的一个点加上一个方向（从何转移）作为一个新点。
 * 或许更合理的表述是原图中的一条边对应新图中的一个点。
 * 例如，原图：
 * 1 2（A）
 * 2 3（B）
 * 3 4（C）
 * 2 4（D）
 * 新图：
 * A B
 * A D
 * B C
 * 
 * 即：原图中的两条有向边，首尾顺次连接即可在新图建边。
 * 特别地，不可以是 A -> B -> A 形式。
 * 
 * 接下来希望在新图中走 N - 1 步，使用矩阵快速转移即可。
 * 
 * 等一下，如果存在重边其实是可以的。更合理的判断方法可能是 i ^ 1 == j
 */
namespace Solution_1037714372308437 {
    i32 constexpr mod = 45989;
    class Matrix {
        i32 height, width;
        std::vector<std::vector<i32>> data;
    public:
        Matrix(i32 height, i32 width): height(height), width(width), data(height, std::vector<i32>(width)) {}

        auto operator[] (uz idx) -> std::vector<i32> & { return data[idx]; }
        auto operator[] (uz idx) const -> std::vector<i32> const & { return data[idx]; }
        
        auto operator* (Matrix const &other) const -> Matrix {
            assert(width == other.height);
            Matrix res{height, other.width};

            for (i32 i = 0; i < height; i++) {
                for (i32 j = 0; j < other.width; j++) {
                    for (i32 k = 0; k < width; k++) {
                        res[i][j] = (res[i][j] + data[i][k] * other.data[k][j] % mod) % mod;
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
            auto res = identity(height), a = *this;
            for (; b != 0; b >>= 1, a = a * a) {
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
    void solve() {
        i32 N;  std::cin >> N;  // 原图点数量
        i32 M;  std::cin >> M;  // 原图边数量
        i32 T;  std::cin >> T;  // 目标距离
        i32 A;  std::cin >> A;  // 起点
        i32 B;  std::cin >> B;  // 终点

        std::vector<std::pair<i32, i32>> edges;
        for (auto m = M; m --> 0; ) {
            i32 x, y;  std::cin >> x >> y;
            edges.push_back({x, y});
            edges.push_back({y, x});
        }

        struct Node {
            i32 to;
            i32 edge_index;
        };

        auto edges_cnt = static_cast<i32>(edges.size());
        debug {
            for (i32 i = 0; i < edges_cnt; i++) {
                auto [x, y] = edges[i];
                std::cout << "edge [" << i << "] " << x << " " << y << std::endl;
            }
        }
        Matrix mat{edges_cnt, edges_cnt};  // 新图的邻接矩阵
        for (i32 i = 0; i < edges_cnt; i++) {
            for (i32 j = 0; j < edges_cnt; j++) {
                auto [x0, y0] = edges[i];
                auto [x1, y1] = edges[j];
                if (y0 == x1 and (i ^ 1) != j) {
                    debug  std::cout << "edge connect " << i << " " << j << std::endl;
                    mat[i][j] = 1;
                }
            }
        }

        debug {
            std::cout << "mat = " << endl << mat << std::endl;
            std::cout << std::format("mat.qpow({})", T - 1) << std::endl;
            auto res = Matrix::identity(edges_cnt);
            for (auto i = 0; i < T - 1; i++) {
                res = res * mat;
                std::cout << "i = " << i << ", res = " << endl << res << std::endl;
            }
        }
        auto res_mat = mat.qpow(T - 1);  // 经历 T - 1 步
        debug  std::cout << "res_mat = " << endl << res_mat << std::endl;

        std::vector<i32> edges_from_begin, edges_to_end;
        for (i32 i = 0; i < edges_cnt; i++) {
            auto [x, y] = edges[i];
            if (x == A)  edges_from_begin.push_back(i);
            if (y == B)  edges_to_end.push_back(i);
        }

        i32 ans = 0;
        for (auto e1: edges_from_begin) {
            for (auto e2: edges_to_end) {
                ans = (ans + res_mat[e1][e2]) % mod;
            }
        }

        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1037714372308437::solve();
    return 0;
}
