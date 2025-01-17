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
 * F[i][j] 表示 [i, i + 2**j] 区间内是否存在 '.'。
 * 比较：F[x][j] 和 F[y][j] 均为 false：继续跳
 * F[x][j] 和 F[y][j] 均为 true：减小 j
 * 否则：可以判断
 */
namespace Solution_6710300631688193 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;  std::cin >> N;
        std::vector<int> vec(N);
        for (auto &x: vec)  std::cin >> x;

        auto lg = std::__lg(N) + 1;
        std::vector F(N, std::vector<char>(lg));
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_6710300631688193::solve();
    return 0;
}
