/**
 * @link https://www.luogu.com.cn/problem/P2886
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"
using namespace lib;

/**
 * S 到 E 经过 N 条边的方案数 F[N] 可以直接通过矩阵乘法转移
 * 即：F[i][i, j] * Graph[j, k] -> F[i+1][i, k]，可以发现很像矩阵乘法的形式（Graph[j, k] 为 0/1）
 * 而本题需要求最短路。即上述式子的相加变成了取 min，相乘变成了相加。仍然可以用类似矩阵乘法的方式转移。
 * 这个东西看起来挺符合结合律，所以仍然可以用快速幂进行快速转移。
 */
namespace Solution_5697763215940175 {
    class StrangeMatrix {
        i32 height, width;
        std::vector<std::vector<i32>> data;
    public:
        StrangeMatrix(i32 height, i32 width): height(height), width(width), data(height, std::vector<i32>(width, inf)) {}

        auto operator* (StrangeMatrix const &other) const -> StrangeMatrix {
            assert(width == other.height);
            StrangeMatrix res{height, other.width};

            for (i32 i = 0; i < height; i++) {
                for (i32 k = 0; k < width; k++) {
                    for (i32 j = 0; j < other.width; j++) {
                        chkMin(res.data[i][j], data[i][k] + other.data[k][j]);
                    }
                }
            }
            return res;
        }

        auto static identity(i32 N) -> StrangeMatrix {
            StrangeMatrix res{N, N};
            for (i32 i = 0; i < N; i++) {
                res[i][i] = 0;
            }
            return res;
        }

        auto qpow(i64 b) const -> StrangeMatrix {
            auto a = *this;
            assert(a.height == a.width);
            auto res = identity(a.height);
            for (; b; b >>= 1, a = a * a) {
                if (b & 1)  res = res * a;
            }
            return res;
        }

        auto operator[] (uz idx) -> std::vector<i32> & {
            return data[idx];
        }
        auto operator[] (uz idx) const -> std::vector<i32> const & {
            return data[idx];
        }

        auto friend operator<< (std::ostream &os, StrangeMatrix const &sm) -> std::ostream & {
            for (i32 i = 0; i < sm.height; i++) {
                if (i != 0)  os << endl;
                for (i32 j = 0; j < sm.width; j++) {
                    if (j != 0)  os << " ";
                    if (sm[i][j] >= inf) {
                        os << "inf";
                    } else {
                        os << std::fixed << std::setw(3) << (sm[i][j]);
                    }
                }
            }
            return os;
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N, T, S, E;  std::cin >> N >> T >> S >> E;
        std::vector<std::tuple<i32, i32, i32>> edges;
        std::vector<i32> points;
        for (auto t = T; t --> 0; ) {
            i32 val, x, y;  std::cin >> val >> x >> y;
            edges.push_back({x, y, val});
            points.push_back(x), points.push_back(y);
        }
        ranges::sort(points);
        points.erase(ranges::unique(points).begin(), points.end());
        auto find = [&](i32 x) -> i32 {
            auto it = ranges::lower_bound(points, x);
            assert(*it == x);
            return std::distance(points.begin(), it);
        };
        i32 nodeCount = points.size();
        StrangeMatrix Graph{nodeCount, nodeCount};  // 邻接矩阵
        for (i32 i = 0; i < nodeCount; i++) {
            for (i32 j = 0; j < nodeCount; j++) {
                Graph[i][j] = inf;
            }
        }
        for (auto [x, y, val]: edges) {
            x = find(x), y = find(y);
            Graph[x][y] = val;
            Graph[y][x] = val;
        }

        // Graph 矩阵本身为 F[1]
        // 需要乘以 pow(Graph, N - 1)

        // std::cout << Graph << endl << endl << Graph * StrangeMatrix::identity(nodeCount) << endl;

        // auto tmp = Graph;
        // for (i32 _ = 3; _ --> 0; ) {
        //     std::cout << tmp << endl << endl;
        //     tmp = tmp * Graph;
        // }

        auto res_mat = Graph * Graph.qpow(N - 1);
        auto ans = res_mat[find(S)][find(E)];
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_5697763215940175::solve();
    return 0;
}
