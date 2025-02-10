/**
 * @link https://www.luogu.com.cn/problem/P9905
 */
#ifndef ONLINE_JUDGE
#define GNU_DEBUG
#define _GLIBCXX_DEBUG 1
#define _GLIBCXX_DEBUG_PEDANTIC 1
#define _GLIBCXX_SANITIZE_VECTOR 1
#endif
#include <bits/stdc++.h>
bool DEBUG_MODE=false;
#define debug if(DEBUG_MODE)
template <typename T> inline auto chkMax(T& base, const T& cmp) { return (base = std::max(base, cmp)); }
template <typename T> inline auto chkMin(T& base, const T& cmp) { return (base = std::min(base, cmp)); }
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
