/**
 * @link https://www.luogu.com.cn/problem/AT_abc264_c
 * @link https://atcoder.jp/contests/abc264/tasks/abc264_c
 */
#ifdef ONLINE_JUDGE
#pragma GCC optimize(3)
#pragma GCC optimize("Ofast")
#endif
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_6871291529187168 {
    using status = __int128;
    int W1, H1, W2, H2;
    std::vector<std::vector<int>> matrix1, matrix2;
    bool check(status st) {
        std::vector<std::vector<int>> tmp(H2);
        auto line = tmp.begin();
        for (auto i: range(matrix1.size())) {
            bool flag = false;  // 添加新元素
            for (auto j: range(matrix1[i].size())) {
                if (((status)1 << (i * W1 + j)) & st) {
                    line->push_back(matrix1[i][j]);
                    flag = true;
                }
            }
            if (flag)  line++;
            if (line == tmp.end())  break;
        }
        return tmp == matrix2;
    }
    void dfs(status st, int cur) {
        debug {
            for (auto i: range(H1)) {
                for (auto j: range(W1)) {
                    std::cout << !!(1 & (st >> (i * W1 + j)));
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
        if (cur == W1 + H1) {
            if (check(st))  throw 1;
            return;
        }
        dfs(st, cur + 1);  // 啥也不干
        if (cur < W1) {
            // 去掉一列
            auto col = cur;
            for (auto i: range(H1)) {
                st &= ~((status)1 << (i * W1 + col));
            }
        } else {
            // 去掉一行
            auto row = cur - W1;
            for (auto j: range(W1)) {
                st &= ~((status)1 << (row * W1 + j));
            }
        }
        dfs(st, cur + 1);
    }
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);
        std::cin >> H1 >> W1;
        matrix1.resize(H1, std::vector<int>(W1));
        for (auto i: range(H1)) {
            for (auto j: range(W1)) {
                std::cin >> matrix1[i][j];
            }
        }
        std::cin >> H2 >> W2;
        matrix2.resize(H2, std::vector<int>(W2));
        for (auto i: range(H2)) {
            for (auto j: range(W2)) {
                std::cin >> matrix2[i][j];
            }
        }
        try {
            dfs(((status)1 << (H1 * W1)) - 1, 0);
        } catch (int) {
            std::cout << "Yes" << std::endl;
            return;
        }
        std::cout << "No" << std::endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_6871291529187168::solve();
    return 0;
}
