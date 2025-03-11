/**
 * @link https://www.luogu.com.cn/problem/P3821
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"
using namespace lib;

/**
 * 首先一个显然的发现是类似 P2579，游走周期都不会太大，所以取它们的 lcm 是可行的，此时即为全图的一个周期。
 * 希望最小化路径上边权的最大值。
 * 
 * P2579 要求的是两个点之间有多少种路线。最大值的最小值直接做显然不是很好做。
 * 可以使用二分。每一次选定一个值，把所有边权大于这个值的直接去掉，判断这两个点现在是否可达。
 * 
 * 垃圾题。
 */
namespace Solution_2703541250956849 {
    template <typename T, typename U, typename Func, typename Comp = std::less<>>
    auto lowerBoundValue(T begin, T end, U val, Func &&func, Comp &&cmp = {}) -> T {
        while (end - begin >= 1) {
            auto mid = std::midpoint(begin, end);
            if (cmp(func(mid), val))  begin = mid + 1;
            else  end = mid;
        }
        return begin;
    }
    class Matrix {
        i32 height, width;
        std::vector<std::vector<char>> data;
    public:
        Matrix(i32 height, i32 width): height(height), width(width), data(height, std::vector<char>(width)) {}

        auto operator[](uz idx) const -> std::vector<char> const & { return data[idx]; }
        auto operator[](uz idx) -> std::vector<char> & { return data[idx]; }
        auto operator* (Matrix other) const -> Matrix {
            assert(width == other.height);
            Matrix res{height, other.width};

            for (i32 i = 0; i < height; i++) {
                for (i32 j = 0; j < other.width; j++) {
                    for (i32 k = 0; k < width; k++) {
                        res[i][j] |= data[i][k] and other[k][j];
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
                    os << static_cast<i32>(mat[i][j]);
                }
            }
            return os;
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 node_count, edge_count, start, end, time_need;
        std::cin >> node_count >> edge_count >> start >> end >> time_need;
        start--, end--;

        using Edge = std::tuple<i32, i32, i32>;
        std::vector<Edge> origin_edges(edge_count);
        for (auto &[x, y, val]: origin_edges) {
            std::cin >> x >> y >> val, x--, y--;
            if (x > y)  std::swap(x, y);
        }

        ranges::reverse(origin_edges);
        auto proj = lam(x, std::pair(std::get<0>(x), std::get<1>(x)));
        ranges::stable_sort(origin_edges, std::less{}, proj);
        auto to_be_erased = ranges::unique(origin_edges, std::equal_to{}, proj);
        origin_edges.erase(to_be_erased.begin(), to_be_erased.end());
        edge_count = origin_edges.size();

        i32 fish_count;  std::cin >> fish_count;
        std::vector<std::vector<i32>> routes(fish_count);
        for (auto &x: routes) {
            i32 t;  std::cin >> t;
            x.resize(t);
            for (auto &item: x)  std::cin >> item, item--;
        }

        auto check_with_edges = [&](std::vector<std::pair<i32, i32>> const &edges) -> bool {
            std::vector A(12, Matrix{1, node_count});  // 每个矩阵：到达某个点的方案数
            Matrix graph{node_count, node_count};
            for (auto [x, y]: edges) {
                graph[x][y] = graph[y][x] = 1;
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
                debug  std::cout << ans_mat << std::endl;
                for (i32 f = 0; f < fish_count; f++) {
                    ++routes_cur[f];
                    if (routes_cur[f] == routes[f].end())  routes_cur[f] = routes[f].begin();
                    auto fish_pos = *routes_cur[f];
                    for (i32 prev = 0; prev < node_count; prev++) {
                        ans_mat[prev][fish_pos] = 0;
                    }
                }
            }
            auto ans = ans_mat[start][end];
            return ans;
        };
        
        auto check_with_limit = [&](i32 limit) -> bool {  // 通过给定的血量检查
            std::vector<std::pair<i32, i32>> edges_selected;
            ranges::copy(
                origin_edges
                    | views::filter([&](Edge const &e) { return std::get<2>(e) <= limit; })
                    | views::transform([&](Edge const &e) { return std::pair{std::get<0>(e), std::get<1>(e)}; }),
                std::back_inserter(edges_selected)
            );
            return check_with_edges(edges_selected);
        };

        auto max_k = ranges::max(origin_edges | views::transform(lam(const &e, std::get<2>(e))));
        auto min_k = ranges::min(origin_edges | views::transform(lam(const &e, std::get<2>(e))));
        auto ans = lowerBoundValue(min_k, max_k + 1, true, check_with_limit);
        char const constexpr *impossible = "'IMP0SSBLE!!'";  // 题面是这么说的。
        if (ans == max_k + 1)  std::cout << impossible << endl;
        else  std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_2703541250956849::solve();
    return 0;
}
