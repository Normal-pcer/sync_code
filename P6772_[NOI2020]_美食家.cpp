/**
 * @link https://www.luogu.com.cn/problem/P6772
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"
#include "./libs/debug_log.hpp"

using namespace lib;

/**
 * F[t][i][j] 表示走 t 步，从 i 点到 j 点的最大收益。
 * [t-1] -> F[t] 只需要一次矩阵乘法。
 * 两场节日之间不会有什么大事，可以直接快速幂。
 * 复杂度感觉有点悬，先写一下。
 * 
 * 完蛋读错题了。通过一个路还有时间要求。
 * 问题不大。可以把一条路拆开，反正 w <= 5.
 * 
 * 过不去。被卡脖子了。
 * 
 * 一共需要做 K 次快速幂，每次快速幂的复杂度为 
 * 5n * 5n * 5n * log(5n)
 * 
 * 预处理出 P**1, P**2, P**4, P**8, ...
 * 每次计算希望 F <- P**x
 * 这个过程可以变成若干次 F <- P**2**i
 * 每一次乘法，由于 F 是 1 * 5n 的，实际复杂度为 5n * 5n，进行 log 次
 * 
 * 又卡脖子？
 * 不能每次跑快速幂，预处理改成递推即可。
 * 否则预处理复杂度为两个 log。
 * 
 * WA 一个点，令人忍俊不禁。
 * 
 */
namespace Solution_9386299235767661 {
    i64 constexpr inf64 = 0x3f3f3f3f3f3f3f3f;  // ! 别忘了改回来
    i64 constexpr infLimit = 4e18;
    // 乘 -> 加，加 -> max
    class Matrix {
        i32 height, width;
        std::vector<std::vector<i64>> data;
    public:
        Matrix(i32 height, i32 width): height(height), width(width), data(height, std::vector<i64>(width, -inf64)) {}

        auto operator[] (uz idx) const -> auto const & { return data[idx]; }
        auto operator[] (uz idx) -> auto & { return data[idx]; }

        auto operator* (Matrix const &other) const -> Matrix {
            assert(width == other.height);
            Matrix res{height, other.width};

            for (i32 i = 0; i < height; i++) {
                for (i32 j = 0; j < other.width; j++) {
                    for (i32 k = 0; k < width; k++) {
                        chkMax(res[i][j], data[i][k] + other.data[k][j]);
                        // printValues(i, j, data[i][k], other.data[k][j], data[i][k] + other.data[k][j]);
                    }
                }
            }
            return res;
        }

        auto static identity(i32 N) -> Matrix {
            Matrix res{N, N};
            for (i32 i = 0; i < N; i++) {
                res[i][i] = 0;
            }
            return res;
        }

        auto qpow(i64 b) -> Matrix {
            assert(height == width);
            assert(b >= 0);
            auto a = *this;
            auto res = identity(height);

            for (; b != 0; b >>= 1, a = a * a) {
                if (b & 1)  res = res * a;
            }
            return res;
        }

        auto friend operator<< (std::ostream &os, Matrix const &mat) -> std::ostream & {
            i32 max_l = 0;

            i64 constexpr inf_limit = 4e18;
            for (i32 i = 0; i < mat.height; i++) {
                for (i32 j = 0; j < mat.width; j++) {
                    if (mat[i][j] < -inf_limit)  chkMax(max_l, 4);
                    else  chkMax<i32>(max_l, std::to_string(mat[i][j]).length());
                }
            }
            chkMax<i32>(max_l, std::to_string(mat.width).length() + 2);

            for (i32 j = 0; j < mat.width; j++) {
                if (j != 0)  os << " ";
                os << std::setw(max_l + 1) << std::format("[{}]", j);
            }
            for (i32 i = 0; i < mat.height; i++) {
                os << endl;
                for (i32 j = 0; j < mat.width; j++) {
                    if (j != 0)  os << " ";
                    os << std::setw(max_l + 1);
                    if (mat[i][j] < -inf_limit)  os << "-inf";
                    else  os << mat[i][j];
                }
            }
            return os;
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N, M, T, K;  std::cin >> N >> M >> T >> K;
        std::vector<i32> reward(N + 1);
        for (auto &x: reward | views::drop(1))  std::cin >> x;

        struct Edge {
            i32 x, y;
            i32 val;
        };
        std::vector<Edge> edges(M);
        for (auto &[x, y, val]: edges)  std::cin >> x >> y >> val;

        // w < 5，直接把需要特定时长的边变为若干段
        struct VPoint {  // 到原始点 origin 的距离为 dis 的虚拟点
            i32 origin;
            i32 dis;
        };
        std::vector<i32> max_dis_from(N + 1);  // 距离原始点 i 最远的距离
        for (auto [x, y, val]: edges) {
            chkMax(max_dis_from[x], val);
        }
        std::vector<VPoint> vpoints;  // 虚拟点列表
        std::vector<i32> origin_to_vpoint(N + 1);  // 原始点对应的虚拟点编号
        std::vector<Edge> vedges;  // 连接虚拟点的边
        for (i32 i = 1; i <= N; i++) {
            origin_to_vpoint[i] = static_cast<i32>(vpoints.size());
            vpoints.push_back({i, 0});

            for (i32 j = 1; j < max_dis_from[i]; j++) {
                auto cur_vp = static_cast<i32>(vpoints.size());
                vedges.push_back({cur_vp - 1, cur_vp, 0});  // 从上一个点连边；这条边本身没有奖励
                vpoints.push_back({i, j});
            }
        }

        for (auto [x, y, val]: edges) {
            // 只需把 y 连到虚拟点 {.origin = x, .dis = val - 1}
            auto prev_vpoint_index = origin_to_vpoint[x] + val - 1;
            assert(vpoints[prev_vpoint_index].origin == x);
            assert(vpoints[prev_vpoint_index].dis == val - 1);
            vedges.push_back({prev_vpoint_index, origin_to_vpoint[y], reward[y]});  // 虚拟边的边权表示目标的点权
        }

        auto vN = static_cast<i32>(vpoints.size());

        debug {
            for (i32 i = 0; i < vN; i++) {
                std::cout << std::format("vpoint[{}]: origin = {}, dis = {}", i, vpoints[i].origin, vpoints[i].dis) << std::endl;
            }

            for (auto [x, y, val]: vedges) {
                std::cout << "vedge " << x << " " << y << " " << val << std::endl;
            }
        }

        struct Festival {
            i32 time, pos, extra;
        };
        std::vector<Festival> festivals(K);
        for (auto &[t, p, x]: festivals)  std::cin >> t >> p >> x, p = origin_to_vpoint[p];  // 直接改成对应的虚拟点
        ranges::sort(festivals, std::less{}, lam(x, x.time));

        Matrix P{vN, vN};  // 转移矩阵
        for (auto &[x, y, val]: vedges) {
            chkMax<i64>(P[x][y], val);
        }

        Matrix F{1, vN};  // 从原点走到点 j
        i32 F_index = 0;

        F[0][0] = reward[1];

        std::vector<Matrix> p_pow_2_pow_i {P};
        auto log_T = std::__lg(T) + 1;
        for (i32 i = 1; i < log_T; i++) {
            auto const &prev = p_pow_2_pow_i.back();
            p_pow_2_pow_i.push_back(prev * prev);
        }

        auto transform_n = [&](Matrix const &F, i32 n) -> Matrix {
            auto res = F;
            for (i32 i = 0; i < log_T; i++) {
                if (n & (1 << i)) {
                    res = res * p_pow_2_pow_i[i];
                }
            }
            return res;
        };

        for (auto f: festivals) {
            // 先转移到下次节日
            // for (; F_index < f.time; F_index++) {
            //     debug  std::cout << "F[" << F_index << "] = \n" << F << endl;
            //     F = F * P;
            // }

            // 使 F 转移 N 次
            
            F = transform_n(F, f.time - F_index), F_index = f.time;
            // 节日开始
            // 对于每一种走到那个城市的方案都可以 +extra
            F[0][f.pos] += f.extra;
            debug  std::cout << "F[" << F_index << "] = \n" << F << endl;
        }

        // 转移到目标天数
        // for (; F_index < T; F_index++) {
        //     debug  std::cout << "F[" << F_index << "] = \n" << F << endl;
        //     F = F * P;
        // }
        F = transform_n(F, T - F_index), F_index = T;
        debug  std::cout << "F[" << F_index << "] = \n" << F << endl;

        auto ans = F[0][0];
        if (ans < -infLimit)  ans = -1;
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_9386299235767661::solve();
    return 0;
}
