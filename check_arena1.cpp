/**
 * @link https://www.luogu.com.cn/problem/P10115
 */
#include "./lib_v5.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

namespace Solution_9756163526698074 {
    auto solve_force() -> void {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N;  std::cin >> N;
        std::string s;  std::cin >> s;
        s.insert(s.begin(), '_');  // 1-index
        
        std::vector<i32> a(N + 1);  // 1-index
        for (auto &x: a | views::drop(1))  std::cin >> x;

        using boolean = char;
        std::vector is_legal(N + 1, std::vector<boolean>(N + 1, false));  // [i, j] 是否为合法括号串
        for (i32 i = 1; i <= N; i++) {
            i32 stack_size = 0;
            for (i32 j = i; j <= N; j++) {
                if (s[j] == '(') {
                    stack_size++;
                } else {
                    if (stack_size == 0) break;
                    stack_size--;
                    if (stack_size == 0) is_legal[i][j] = true;
                }
            }
        }

        std::vector<i64> F(N + 1, -infLL);  // [1, i] 区间的最大分数
        F[0] = 0;
        for (i32 i = 1; i <= N; i++) {
            chkMax(F[i], F[i - 1]);
            for (i32 j = 1; j <= i; j++) {
                if (is_legal[j][i]) {
                    chkMax(F[i], F[j - 1] - a[j] + a[i]);
                }
                // std::cout << "F[" << i << "] = " << F[i] << endl;
            }
        }

        auto ans = F[N];
        std::cout << ans << endl;
    }
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N; std::cin >> N;
        std::string s; std::cin >> s;
        s.insert(s.begin(), '_');
        std::vector<i32> a(N + 1);
        for (auto &x: a | views::drop(1)) std::cin >> x;

        std::vector<i32> prev(N + 1, 0);
        std::vector<i32> stack;
        for (i32 j = 1; j <= N; j++) {
            if (s[j] == '(') {
                stack.push_back(j);
            } else {
                if (not stack.empty()) {
                    prev[j] = stack.back();
                    stack.pop_back();
                }
            }
        }

        std::vector<i64> F(N + 1, -infLL);
        std::vector<i64> G(N + 1, -infLL);

        assert(N >= 1);
        F[0] = 0, G[0] = -a[1];
        for (i32 i = 1; i <= N; i++) {
            chkMax(F[i], F[i - 1]);
            if (prev[i] != 0) chkMax(F[i], G[prev[i] - 1] + a[i]);
            if (i != N) chkMax(G[i], F[i] - a[i + 1]);
        }

        auto ans = F[N];
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_9756163526698074::solve_force();
    return 0;
}
