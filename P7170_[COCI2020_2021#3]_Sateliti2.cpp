/**
 * @link https://www.luogu.com.cn/problem/P7170
 */
#include "libs/debug_macros.hpp"
#include "lib"
#include "libs/range.hpp"
using namespace lib;
/**
 * 先考虑一维的情况
 * **...***.**...***.
 *          ^ 复制一次
 * 尝试对 [0, N) 排序：
 * 从 i 开始取 N 个数，比较哪一个更先遇到 '.'。
 * 考虑倍增。
 * F[i][j] 表示 [i, i + 2**j) 区间内是否存在 '.'。
 * 比较：F[x][j] 和 F[y][j] 均为 false：继续跳
 * F[x][j] 和 F[y][j] 均为 true：减小 j
 * 否则：可以判断
 */
namespace Solution_6710300631688193 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;  std::cin >> N;
        std::vector<char> vec(N);
        for (auto &x: vec)  std::cin >> x;
        vec.resize(N + N);
        std::copy(vec.begin(), vec.begin() + N, vec.begin() + N);

        auto lg = std::__lg(N + N) + 1;
        std::vector F(N + N, std::vector<char>(lg));
        for (auto i: range(N + N)) {
            F[i][0] = (vec[i] == '.');
        }
        for (auto j: range(1, lg)) {
            for (auto i: range(N + N)) {
                if (i + (1 << j) > N + N)  break;
                F[i][j] = F[i][j-1] or F[i + (1 << (j-1))][j-1];
            }
        }
        for (auto i: range(N)) {
            for (auto j: range(lg)) {
                std::cout << static_cast<int>(F[i][j]) << ' ';
            }
            std::cout << std::endl;
        }
        // a < b
        auto cmp = [&](int a, int b) -> bool {
            for (auto j = lg - 1; j --> 0; ) {
                bool flag0 = F[a][j], flag1 = F[b][j];
                if (not flag0 and not flag1) {
                    a += 1 << j, b += 1 << j;
                } else if (flag0 and not flag1) {
                    return false;
                } else if (flag1 and not flag0) {
                    return true;
                }
            }
            return false;
        };
        std::vector<int> nums {ranges::min(range(0, N), cmp)};
        std::cout << endl;
        for (auto x: nums) {
            std::vector<char> tmp(vec);
            tmp.resize(N);
            std::rotate(tmp.begin(), tmp.begin() + x, tmp.end());
            for (auto ch: tmp)  std::cout << ch;
            std::cout << std::endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_6710300631688193::solve();
    return 0;
}
