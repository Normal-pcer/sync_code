/**
 * @link https://www.luogu.com.cn/problem/P1939
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
 * a[x] = a[x - 1] + a[x - 3]
 * [a[x - 1], a[x - 2], a[x - 3]] 如何变成 [a[x], a[x - 1], a[x - 2]]
 * a[x] = 1*a[x-1] + 0*a[x-2] + 1*a[x-3]
 * a[x-1] = 1*a[x-1] + 0*a[x-2] + 0*a[x-3]
 * a[x-2] = 0*a[x-1] + 1*a[x-2] + 0*a[x-3]
 */
namespace Solution_1083002564197378 {
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
        Matrix a{ {1, 1, 1} };
        Matrix b{
            {1, 1, 0},
            {0, 0, 1}, 
            {1, 0, 0}
        };

        i32 T;  std::cin >> T;
        for (i32 t = T; t --> 0; ) {
            i32 N;  std::cin >> N;
            if (N <= 3)  std::cout << 1 << endl;
            else {
                auto ans_mat = a * qpow(b, N - 3);
                auto ans = ans_mat.data[0][0];
    
                std::cout << ans << endl;
            }
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1083002564197378::solve();
    return 0;
}
