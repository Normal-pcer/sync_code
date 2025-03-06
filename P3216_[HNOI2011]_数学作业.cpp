/**
 * @link https://www.luogu.com.cn/problem/P3216
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
 * 所有的 t 位数拼接起来
 * 
 * F[i] = F[i-1] * 10^t + i
 * 
 * 考虑如下矩阵：
 * F[i], i, 1
 * 
 * 它进行一次转移只需乘以下面的矩阵：
 * 10**t, 0, 0
 *     1, 1, 0
 *     0, 1, 1
 * 
 */
namespace Solution_4215287862333989 {
    i32 mod = 1;
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
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i64 N;  std::cin >> N;
        i32 M;  std::cin >> M;

        mod = M;
        N++;

        Matrix A{
            {0, 1, 1}
        };  // F[0] 对应的矩阵

        for (auto [t, pow_10_t] = std::pair<i32, u64>{0, 1}; pow_10_t <= N; t++, pow_10_t *= 10) {
            i32 x = pow_10_t * 10 % mod;
            Matrix B{
                {  x,  0,  0},
                {  1,  1,  0},
                {  0,  1,  1}
            };

            auto iter_count = N - pow_10_t;
            chkMin(iter_count, pow_10_t * 9);
            A = A * B.qpow(iter_count);
            // std::cout << A << endl;
        }

        auto ans = A[0][0];
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_4215287862333989::solve();
    return 0;
}
