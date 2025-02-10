/**
 * @link https://www.luogu.com.cn/problem/P9905
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

using namespace lib;

/**
 * 首先考虑一维的情况：
 * 给定一个序列，求每个长为 x 的子段的最大值
 * 单调队列维护即可（滑动窗口），复杂度 O(N)
 * 
 * 扩展到二维，求每一个子矩阵
 * 可以处理出来每一行上 M 个数的最大值，记作矩阵 B
 * 在矩阵 B 的每一列上连续取 N 个数的最大值即为答案
 */
namespace Solution_1603938145106922 {
    // 在一行上跑一遍滑动窗口
    auto slide(std::vector<int> const &arr, int width) -> std::vector<int> {
        auto ans_size = arr.size() - width + 1;
        std::vector<int> ans(ans_size);

        std::deque<int> q;  // 单调递减队列，存储 arr 中的下标
        for (auto i = 0; i < static_cast<int>(arr.size()); i++) {
            while (not q.empty() and i - q.front() >= width)  q.pop_front();
            // 所有小于当前元素的都可以直接弹出
            while (not q.empty() and arr[q.back()] <= arr[i])  q.pop_back();
            q.push_back(i);
            auto cur_ans = arr[q.front()];
            if (i >= width - 1)  ans[i - width + 1] = cur_ans;
        }

        return ans;
    }
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, M;  std::cin >> N >> M;
        std::vector mat(N, std::vector<int>(M));
        for (auto &row: mat) {
            for (auto &item: row)  std::cin >> item;
        }
        int R, C;  std::cin >> R >> C;

        std::vector<std::vector<int>> max_per_row(N);
        for (auto i = 0; i < N; i++) {
            max_per_row[i] = slide(mat[i], C);
        }

        std::vector max_per_column(M - C + 1, std::vector<int>(N));
        for (auto j = 0; j < M - C + 1; j++) {
            for (auto i = 0; i < N; i++) {
                max_per_column[j][i] = max_per_row[i][j];
            }
        }

        std::vector<std::vector<int>> ans_rotate(M - C + 1);
        for (auto j = 0; j < M - C + 1; j++) {
            ans_rotate[j] = slide(max_per_column[j], R);
        }

        std::vector ans(N - R + 1, std::vector<int>(M - C + 1));
        for (auto i = 0; i < N - R + 1; i++) {
            for (auto j = 0; j < M - C + 1; j++) {
                ans[i][j] = ans_rotate[j][i];
            }
        }

        for (auto &row: ans) {
            for (auto &item: row)  std::cout << item << " ";
            std::cout << endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1603938145106922::solve();
    return 0;
}
