/**
 * @link https://www.luogu.com.cn/problem/P5343
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"
using namespace lib;

/**
 * 记 F[x] 表示使用指定长度块填充至总长度为 x 的方案数。
 * 
 * F[x] <- F[x - len]
 * 
 * x <= 100
 * 只有最近的 100 项可能有用
 */
namespace Solution_4188520239411906 {
    template <typename T>
    using init_list = std::initializer_list<T>;
    i32 constexpr mod = 1e9 + 7;
    class Matrix {
        i32 height, width;
        std::vector<std::vector<i32>> data;
    public:
        Matrix(i32 height, i32 width): height(height), width(width), data(height, std::vector<i32>(width)) {}
        Matrix(init_list<init_list<i32>> const &il) {
            height = il.size();
            width = ranges::max(il | views::transform(lam(const &x, x.size())));
            data = std::vector(height, std::vector<i32>(width));

            for (auto [it, i] = std::pair{il.begin(), 0}; it != il.end(); it++, i++) {
                for (auto [it2, j] = std::pair{it->begin(), 0}; it2 != it->end(); it2++, j++) {
                    data[i][j] = *it2;
                }
            }
        }

        auto operator[] (uz idx) const -> std::vector<i32> const & { return data[idx]; }
        auto operator[] (uz idx) -> std::vector<i32> & { return data[idx]; }
        auto operator* (Matrix const &other) const -> Matrix {
            assert(width == other.height);
            Matrix res{height, other.width};

            for (i32 i = 0; i < height; i++) {
                for (i32 j = 0; j < other.width; j++) {
                    for (i32 k = 0; k < width; k++) {
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
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i64 N;  std::cin >> N;
        // 所有可以使用的长度
        auto len_collection = [&]() -> std::vector<i32> {
            auto sort_unique = [&](auto &vec) -> void {
                ranges::sort(vec);
                auto erase = ranges::unique(vec);
                vec.erase(erase.begin(), erase.end());
            };
            
            i32 n0;  std::cin >> n0;
            std::vector<i32> v0(n0);
            for (auto &x: v0)  std::cin >> x;

            i32 n1;  std::cin >> n1;
            std::vector<i32> v1(n1);
            for (auto &x: v1)  std::cin >> x;

            sort_unique(v0), sort_unique(v1);
            std::vector<i32> res;
            ranges::set_intersection(v0, v1, std::back_inserter(res));
            return res;
        }();

        auto max_len = ranges::max(len_collection);
        Matrix A0{1, max_len + 1};  // A[i][0][j] 表示 F[i - j]，越界为 0
        A0[0][0] = 1;
        
        Matrix T{max_len + 1, max_len + 1};  // 转移矩阵
        for (auto len: len_collection) {
            T[len - 1][0] = 1;
        }
        for (i32 i = 1; i <= max_len; i++) {
            T[i - 1][i] = 1;
        }

        auto AN = A0 * T.qpow(N);
        auto ans = AN[0][0];
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_4188520239411906::solve();
    return 0;
}
