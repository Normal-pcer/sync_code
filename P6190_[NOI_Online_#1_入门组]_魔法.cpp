/**
 * @link https://www.luogu.com.cn/problem/P6190
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
#include "./libs/debug_log.hpp"

using namespace lib;

/**
 * 我常常追忆过去。
 * 
 * F[i][j][k] 表示从 i 到 j 使用 k 次魔法的最短路
 * 
 * 考虑转移：
 * 枚举一个中间点 x，便有：
 * F[i][j][k] <- F[i][x][k - 1] + F[x][j][1]
 * 
 * 最后的答案是 F[first][last][k] 的最小值。
 * 这样转移似乎也比较规则，可以使用矩阵优化。
 * 不行。统计答案需要用到所有的值。
 * 
 * 考虑怎么简化统计答案的过程。可以更改一下状态的定义：
 * F[i][j][k] 表示从 i 到 j 使用 **至多** k 次魔法的最短路。
 * 这样的优势是统计答案只需要取一个值，可以用矩阵快速幂优化转移。
 * 
 * F[i][j][k] <- F[i][x][k - 1] + F[x][j][1]
 *            <- F[i][j][k - 1]
 * 相比于通常的直接通过邻接矩阵转移，这个稍微复杂一些，但是不多。
 * F[x][j][1] 在每一次转移的过程中完全可以看作一个定值。对于以下的答案矩阵：
 * [F[i][1], F[i][2], F[i][3], F[i][4], ..., F[i][N]]
 * 转移时把正常矩阵乘法的 乘 -> 加，加 -> min
 * 除了邻接矩阵以外，还需要支持 F[i][j][k - 1] + 0 的转移，相当于添加了一个 j -> j 边权为 0 的自环。
 * 边权为 1e9 级别，矩阵及答案等需要使用 64 位整数。
 */
namespace Solution_7584560610871545 {
    i64 constexpr inf64 = 0x3f3f3f3f3f3f3f3fLL;
    class Matrix {
        i32 height = 0, width = 0;
        std::vector<std::vector<i64>> data;
    public:
        Matrix() {}
        Matrix(i32 height, i32 width): height(height), width(width), data(height, std::vector<i64>(width, inf64)) {}

        auto operator[] (uz idx) const -> std::vector<i64> const & { return data[idx]; }
        auto operator[] (uz idx) -> std::vector<i64> & { return data[idx]; }

        auto static identity(i32 N) -> Matrix {
            Matrix res{N, N};
            for (i32 i = 0; i < N; i++) {
                for (i32 j = 0; j < N; j++) {
                    if (i == j)  res[i][j] = 0;
                    else  res[i][j] = inf64;
                }
            }
            return res;
        }

        auto operator* (const Matrix &other) const -> Matrix {
            assert(width == other.height);
            Matrix res{height, other.width};
            for (i32 i = 0; i < height; i++) {
                for (i32 j = 0; j < other.width; j++) {
                    for (i32 k = 0; k < width; k++) {
                        i64 cur = inf64;
                        if (data[i][k] == inf64)  cur = inf64;
                        else if (other.data[k][j] == inf64)  cur = inf64;
                        else  cur = data[i][k] + other.data[k][j];
                        chkMin(res[i][j], cur);
                    }
                }
            }
            return res;
        }

        auto qpow(i64 b) const -> Matrix {
            assert(height == width);
            assert(b >= 0);
            auto a = *this;
            auto res = identity(height);
            assert(res * a == a);
            for (; b != 0; b >>= 1, a = a * a) {
                if (b & 1)  res = res * a;
            }
            return res;
        }
        auto operator== (Matrix const &other) const -> bool { return data == other.data; }
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
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N, M, K;  std::cin >> N >> M >> K;
        struct Edge {
            i32 a, b, val;
        };
        std::vector<Edge> edges(M);
        for (auto &[a, b, val]: edges)  std::cin >> a >> b >> val;

        struct GraphNode {
            i32 to, val;
        };
        std::vector<std::vector<GraphNode>> graph(N + 1);
        for (auto &[a, b, val]: edges) {
            graph[a].push_back({b, val});
        }

        // 预处理以下问题：
        // 从每个点开始走，使用恰好一次魔法，到达终点的最小代价
        // G[t][i][j] 表示从 i 走到 j 恰好使用 t 次魔法的最小代价
        std::vector G(2, std::vector(N + 1, std::vector<i64>(N + 1, inf64)));
        {
            // 从每个点开始跑一遍 dijkstra？
            // 被肘了。是负权。我们没救了。
#if false
            {
                // 从 s 开始，最多使用 max_cnt 次魔法
                auto walk_from = [&](i32 s, i32 max_cnt, std::vector<i64> &out) -> void {
                    struct Node {
                        i32 p;      // 点
                        i32 val;    // 花费
                        i32 cnt;    // 已经使用魔法的次数

                        auto operator<=> (Node const &other) const -> std::weak_ordering {
                            return val <=> other.val;
                        }
                    };
                    std::priority_queue<Node, std::vector<Node>, std::greater<>> q;
                    q.push({s, 0});

                    std::vector<char> vis(N + 1);
                    while (not q.empty()) {
                        auto x = q.top();  q.pop();
                        if (vis[x.p])  continue;
                        vis[x.p] = true;

                        for (auto [next, edge_cost]: graph[x.p]) {
                            if (not vis[next]) {
                                q.push({next, x.val + edge_cost, x.cnt});
                                if (x.cnt + 1 <= max_cnt) {
                                    q.push({next, x.val - edge_cost, x.cnt + 1});  // 遗憾离世
                                }
                            }
                        }
                    }
                };
            }
#endif
            // 用类似 Floyd 的方式，枚举对哪一条边 x, y 施法，
            // F[1][i][j] <- F[0][i][x] - val + F[0][y][j]
            for (auto [x, y, val]: edges)  G[0][x][y] = val;
            for (i32 i = 1; i <= N; i++)  G[0][i][i] = 0;
            for (i32 k = 1; k <= N; k++) {
                for (i32 i = 1; i <= N; i++) {
                    for (i32 j = 1; j <= N; j++) {
                        chkMin(G[0][i][j], G[0][i][k] + G[0][k][j]);
                    }
                }
            }
            for (auto [x, y, val]: edges) {
                for (i32 i = 1; i <= N; i++) {
                    for (i32 j = 1; j <= N; j++) {
                        chkMin(G[1][i][j], G[0][i][x] - val + G[0][y][j]);
                    }
                }
            }

            for (i32 i = 1; i <= N; i++) {
                for (i32 j = 1; j <= N; j++) {
                    debug  printValues(i, j, G[0][i][j], G[1][i][j]);
                }
            }
        }
        
        // G[0][1][i] 全都放到一个矩阵里面
        Matrix mat{1, N + 1};
        for (i32 i = 1; i <= N; i++)  mat[0][i] = G[0][1][i];

        // 考虑如何做快速批量转移
        Matrix T{N + 1, N + 1};
        // mat[1][i] <- old_mat[1][j] + T[j][i]
        // T[j][i]  = G[k][1][i] - G[k - 1][i][j]
        //          = G[k - 1][i][j] + G[1][j][i] - G[k - 1][i][j]
        //          = G[1][j][i]
        
        for (i32 j = 1; j <= N; j++) {
            for (i32 i = 1; i <= N; i++) {
                T[j][i] = G[1][j][i];
            }
        }

        // 允许自己转移到自己，代价为 0
        for (i32 i = 1; i <= N; i++) {
            chkMin<i64>(T[i][i], 0);
        }

        auto ans_mat = mat;
        // for (auto i = 1; i <= K; i++) {
            // std::cout << ans_mat << endl;
            // ans_mat = ans_mat * T;
        // }
        ans_mat = ans_mat * T.qpow(K);
        // std::cout << ans_mat << endl;
        std::cout << ans_mat[0][N] << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_7584560610871545::solve();
    return 0;
}
