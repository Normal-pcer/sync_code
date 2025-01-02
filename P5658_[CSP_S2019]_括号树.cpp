/**
 * @link https://www.luogu.com.cn/problem/P5658
 */

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_1521759611605568 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;  std::cin >> N;
        std::string s;  std::cin >> s;
        s.insert(s.begin(), '_');
        std::vector<int> fa(N+1);
        for (auto &x: fa | views::drop(2))  std::cin >> x;

        std::vector<std::vector<int>> forward(N+1);
        for (auto i: range(1, N+1)) {
            forward[i].push_back(fa[i]);
            forward[fa[i]].push_back(i);
        }

        std::deque<int> st;
        std::vector<ll> F(N+1), G(N+1);
        std::function<void(int, int)> dfs = [&](int p, int prev) {
            auto top = -1;
            if (s[p] == '(') {
                st.push_back(p);
            } else {
                if (not st.empty()) {
                    top = st.back();
                    st.pop_back();
                    F[p] = F[fa[top]] + 1;
                }
            }
            G[p] = F[p] + G[prev];
            for (auto x: forward[p])  if (x != prev) {
                dfs(x, p);
            }
            if (s[p] == '(') {
                st.pop_back();
            } else {
                if (top != -1) {
                    st.push_back(top);
                }
            }
        };

        dfs(1, 0);
        auto iota = range(N+1);
        auto ans = std::inner_product(iota.begin(), iota.end(), G.begin(), 0LL, std::bit_xor<>{}, std::multiplies<>{});
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1521759611605568::solve();
    return 0;
}
