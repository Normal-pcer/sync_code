/**
 * @link https://www.luogu.com.cn/problem/P4159
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"
using namespace lib;

/**
 * A 到 B 之间有特定长度的一段路径，可以转换成有多段路径。
 * 例如记 A0，A1，A2，..., Ai 表示距离 A 点距离为 i
 * A -> B 距离为 3，转换为：
 * A0 -> A1 -> A2 -> B0
 */
namespace Solution_9601630652620079 {
    i32 mod = 2009;
    auto mul(i32 x, i32 y) -> i32 {
        return static_cast<i64>(x) * y % mod;
    }
    template <typename T>
    using ilist = std::initializer_list<T>;

    class Matrix {
        i32 height, width;
        std::vector<std::vector<i32>> data;
    public:
        Matrix(i32 height, i32 width): height(height), width(width), data(height, std::vector<i32>(width)) {}
        Matrix(ilist<ilist<i32>> init) {
            height = init.size();
            width = ranges::max(init | views::transform(lam(const &x, x.size())));

            data = std::vector(height, std::vector<i32>(width));
            for (auto [it, i] = std::pair{init.begin(), 0}; it != init.end(); it++, i++) {
                for (auto [it2, j] = std::pair{it->begin(), 0}; it2 != it->end(); it2++, j++) {
                    data[i][j] = *it2;
                }
            }
        }
        auto operator[] (uz idx) const -> std::vector<i32> const & { return data[idx]; };
        auto operator[] (uz idx) -> std::vector<i32> & { return data[idx]; };

        auto static identity(i32 N) -> Matrix {
            Matrix res{N, N};
            for (i32 i = 0; i < N; i++) {
                res[i][i] = 1;
            }
            return res;
        }
        auto operator* (Matrix const &other) const -> Matrix {
            assert(width == other.height);
            Matrix res{height, other.width};
            
            for (i32 i = 0; i < height; i++) {
                for (i32 j = 0; j < other.width; j++) {
                    for (i32 k = 0; k < width; k++) {
                        res[i][j] = (res[i][j] + mul(data[i][k], other.data[k][j])) % mod;
                    }
                }
            }
            return res;
        }
        auto qpow(i64 b) const -> Matrix {
            assert(height == width);
            auto a = *this;
            auto res = identity(height);
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
                    os << mat.data[i][j];
                }
            }
            return os;
        }
    };
    void solve() {
        i32 N, T;  std::cin >> N >> T;
        std::vector dis(N, std::vector<i32>(N));
        for (i32 i = 0; i < N; i++) {
            for (i32 j = 0; j < N; j++) {
                char cur;  std::cin >> cur;
                i32 cur_len = cur - '0';
                dis[i][j] = cur_len;
            }
        }

        struct Point {
            i32 origin = -1;
            i32 dis = 0;
        };
        std::vector<Point> points;
        std::vector<std::pair<i32, i32>> edges;
        std::vector<i32> origin_points;  // A0, B0... 的下标
        for (i32 i = 0; i < N; i++) {
            i32 points_index_begin = points.size();
            origin_points.push_back(points_index_begin);
            auto max_dis = ranges::max(ranges::max(dis[i]), 1);
            for (i32 dis = 0; dis < max_dis; dis++) {
                points.push_back({i, dis});
                if (dis > 0)  edges.push_back({points_index_begin + dis - 1, points_index_begin + dis});
            }
            for (i32 j = 0; j < N; j++) {
                if (dis[i][j] != 0) {
                    edges.push_back({points_index_begin + dis[i][j] - 1, -j});
                }
            }
        }
        i32 points_count = points.size();
        Matrix graph{points_count, points_count};  // 邻接矩阵
        for (auto [x, y]: edges) {
            if (y < 0)  graph[x][origin_points[-y]] = 1;
            else  graph[x][y] = 1;
        }

        auto res_mat = graph.qpow(T);
        i32 ans = res_mat[origin_points[0]][origin_points[N-1]];
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_9601630652620079::solve();
    return 0;
}
