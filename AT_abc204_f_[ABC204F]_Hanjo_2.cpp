/**
 * @link https://www.luogu.com.cn/problem/AT_abc204_f
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"
#include "./libs/debug_log.hpp"

using namespace lib;

/**
 * 状压。一个砖最多会影响到最后两列。
 * F[i][j] 表示铺完了 i 列，最后一列的二进制状态为 j。
 * 
 * 考虑如何转移。每一次只是想尽可能填满一行，但是连带着可能会影响到下一行。
 * 可能需要写一个 dfs。
 */
namespace Solution_6621393943570355 {
    /**
     * 表示一个类型 T 是否能像 Func 一样调用
     */
    template <typename T, typename Func>
    concept CallableAs = std::convertible_to<T, std::function<Func>>;

    i32 constexpr mod = 998'244'353;
    using Status = std::uint8_t;

    /**
     * placeBlocks 的递归实现（见下文）。
     * 
     * @param p 第一列的 [0, p) 格子保证被填满。
     * @param height 一列的高度。
     * @param first 当前第一列的状态。
     * @param second 当前第二列的状态。
     * @param f 回调函数
     */
    template <typename Func>
    auto placeBlocksRecursion(i32 p, i32 height, Status first, Status second, Func &&f) -> void {
        if (p > height)  return;  // 超出边界
        // 跳过已经被填充的格子
        while (p != height and (first & (Status)1 << p))  p++;
        if (p == height) {
            f(second);  // 调用回调函数
            return;
        }

        // 1 * 1
        if ((((Status)1 << p) & first) == 0) {
            placeBlocksRecursion(p + 1, height, first | ((Status)1 << p), second, std::forward<Func>(f));
        }

        // 2 * 1
        if ((((Status)1 << p) & first) == 0 and (((Status)1 << p) & second) == 0) {
            placeBlocksRecursion(p + 1, height, first | ((Status)1 << p), second | ((Status)1 << p), std::forward<Func>(f));
        }

        // 1 * 2
        if ((((Status)1 << p) & first) == 0 and (((Status)1 << (p + 1)) & first) == 0) {
            placeBlocksRecursion(p + 2, height, first | ((Status)1 << p) | ((Status)1 << (p + 1)), second, std::forward<Func>(f));
        }
    }

    /**
     * 尝试在两列之间放置地板。
     * 目标是填满第一列，同时记录第二列的状态。
     * 
     * @param height 一列的高度。
     * @param st 第一列的初始状态。
     * @param f 当第一列被填满一次之后，将会调用 f。参数如下：
     * f(Status second)
     * 即传入此时第二列的状态。
     */
    template <typename Func>
    auto placeBlocks(i32 height, Status st, Func &&f) -> void {
        placeBlocksRecursion(0, height, st, 0, std::forward<Func>(f));
    }

    class Matrix {
        i32 height, width;
        std::vector<std::vector<i32>> data;
    public:
        Matrix(i32 height, i32 width): height(height), width(width), data(height, std::vector<i32>(width)) {}

        auto operator[] (uz idx) const -> auto const & { return data[idx]; }
        auto operator[] (uz idx) -> auto & { return data[idx]; }

        auto operator* (Matrix const &other) const -> Matrix {
            assert(other.height == width);
            Matrix res{height, other.width};

            for (i32 i = 0; i < height; i++) {
                for (i32 k = 0; k < width; k++) {
                    for (i32 j = 0; j < other.width; j++) {
                        res[i][j] = (res[i][j] + static_cast<i64>(data[i][k]) * other.data[k][j] % mod) % mod;
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

        auto qpow(i64 b) const -> Matrix {
            assert(height == width);
            assert(b >= 0);
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

        i32 H;  i64 W;
        std::cin >> H >> W;

        auto stat_last = (Status)1 << H;
        // std::vector F(W + 1, std::vector<i32>(stat_last));

        Matrix T{stat_last, stat_last};

        // F[0][0] = 1;
        Matrix G0{1, stat_last};
        G0[0][0] = 1;
        // for (i32 i = 0; i < W; i++) {
            for (Status j = 0; j < stat_last; j++) {
                placeBlocks(H, j, [&](Status next) -> void {
                    // F[i + 1][next] = (F[i + 1][next] + F[i][j]) % mod;
                    T[j][next] = (T[j][next] + 1) % mod;
                });
            }
        // }
        auto GW = G0 * T.qpow(W);
        auto ans = GW[0][0];
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_6621393943570355::solve();
    return 0;
}
