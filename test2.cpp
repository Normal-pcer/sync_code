/**
 * @link https://www.luogu.com.cn/problem/P6569
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"
using namespace lib;

/**
 * 一个点下一轮的点权由上一轮固定的几个点转移而来，比较像矩阵乘法的形式。
 * 有若干次查询。每次查询做一下矩阵快速幂即可。
 * 具体地，G[i][0][j] 表示第 i 天第 j 个城市的魔法值。
 * 那么会有一个转移矩阵 T，乘若干次即可。
 * 这里矩阵乘法的“乘”还是乘，“加”变为异或。
 * 然而乘法和按位异或之间并不满足分配律。
 * 但是注意到每次乘以 0/1，可以替换为按位与 0/~0。
 * 按位与、按位异或是满足分配律的。
 * 
 */
namespace Solution_7296640937131565 {
    class Matrix {
        i32 height, width;
        std::vector<std::vector<u32>> data;
    public:
        Matrix(i32 height, i32 width): height(height), width(width), data(height, std::vector<u32>(width)) {}

        auto operator[] (uz idx) const -> std::vector<u32> const & { return data[idx]; }
        auto operator[] (uz idx) -> std::vector<u32> & { return data[idx]; }

        auto operator* (Matrix const &other) const -> Matrix {
            assert(width == other.height);
            Matrix res{height, other.width};

            for (i32 i = 0; i < height; i++) {
                for (i32 j = 0; j < other.width; j++) {
                    for (i32 k = 0; k < width; k++) {
                        res[i][j] ^= data[i][k] & other.data[k][j];
                    }
                }
            }
            return res;
        }

        auto static identity(i32 N) -> Matrix {
            Matrix res{N, N};
            for (i32 i = 0; i < N; i++) {
                res[i][i] = ~(u32)0;
            }
            return res;
        }
        auto qpow(i64 b) const -> Matrix {
            assert(b >= 0);
            assert(height == width);

            auto a = *this, res = identity(height);
            for (; b != 0; b >>= 1, a = a * a) {
                if (b & 1)  res = res * a;
            }
            return res;
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N, M, Q;  std::cin >> N >> M >> Q;
        std::vector<i32> init(N + 1);
        for (auto &x: init | views::drop(1))  std::cin >> x;

        std::vector<std::pair<i32, i32>> edges(M);
        for (auto &[x, y]: edges)  std::cin >> x >> y;

        Matrix T{N + 1, N + 1};  // 转移矩阵
        // 每一条边的一个点都会影响到另一个点，所以 T 为邻接矩阵
        for (auto [x, y]: edges) {
            T[x][y] = T[y][x] = ~(u32)0;
        }

        Matrix G0{1, N + 1};
        for (i32 i = 1; i <= N; i++)  G0[0][i] = init[i];

        for (auto q = Q; q --> 0; ) {
            i32 x;  std::cin >> x;
            auto Gx = G0 * T.qpow(x);
            auto ans = Gx[0][1];

            std::cout << ans << endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_7296640937131565::solve();
    return 0;
}
