#include <bits/stdc++.h>
#define FILE_NAME "C"
#if false
#pragma GCC optimize(3)
#pragma GCC optimize("Ofast")
#pragma GCC optimize("inline")
#endif  // if false
using ll = long long;
using ull = unsigned long long;

const int inf = 0x3f3f3f3f;
const long long infLL = 0x3f3f3f3f3f3f3f3f;

template <typename T>
T &chkMax(T &base, const T &compare) {
    return (base = std::max(base, compare));
}

template <typename T>
T &chkMin(T &base, const T &compare) {
    return (base = std::min(base, compare));
}

namespace Solution {

    inline unsigned hash(std::string const &str) {
        unsigned res = 0;
        for (auto ch: str) {
            res = (res << 5) + (ch ^ 96);
        }
        return res;
    }

    void solve() {
        int N;  std::cin >> N;
        std::vector<std::string> colors_str(N<<1);
        for (auto &c: colors_str)  std::cin >> c;
        std::vector<unsigned> colors(N<<1);
        std::transform(colors_str.begin(), colors_str.end(), colors.begin(), hash);

        // std::vector<int> up, down;
        std::vector<int> up_q, down_q;
        auto ans = inf;
        std::map<unsigned, int> map;  // 1 = 上, -1 = 下, 0 = 未指定
        std::function<void(int, int, int)> dfs = [&](int p, int max1, int max2) {
            if (max1 + max2 >= ans)  return;
            if (p >= (int)colors.size()) {
                // chkMin(ans, count(up) + count(down));
                if (up_q.empty() and down_q.empty()) {
                    chkMin(ans, max1 + max2);
                }
                return;
            }

            auto color = colors.at(p);

            bool found = map.find(color) != map.end();
            int log = found? map.at(color): 0;

            // up.push_back(p);
            int origin = -1;
            if (log != -1) {
                if (log == 0)  map.insert({color, 1});
                if (not up_q.empty() and colors.at(up_q.back()) == color) {
                    origin = up_q.back();
                    up_q.pop_back();
                } else {
                    up_q.push_back(p);
                }
                dfs(p+1, std::max(max1, (int)up_q.size()), std::max(max2, (int)down_q.size()));
                if (origin == -1) {
                    assert(up_q.size());
                    up_q.pop_back();
                } else {
                    up_q.push_back(p);
                }
                if (log == 0)  map.erase(map.find(color));
            }
            
            if (log != 1) {
                origin = -1;
                if (log == 0)  map.insert({color, -1});
                if (not down_q.empty() and colors.at(down_q.back()) == color) {
                    origin = down_q.back();
                    down_q.pop_back();
                } else {
                    down_q.push_back(p);
                }
                dfs(p+1, std::max(max1, (int)up_q.size()), std::max(max2, (int)down_q.size()));
                if (origin == -1) {
                    assert(down_q.size());
                    down_q.pop_back();
                } else {
                    down_q.push_back(p);
                }
                if (log == 0)  map.erase(map.find(color));
            }
        };

        dfs(0, 0, 0);

        if (ans >= inf)  std::cout << -1 << std::endl;
        else  std::cout << ans << std::endl;
    }
}

int main() {
#ifndef ONLINE_JUDGE
    std::freopen(FILE_NAME ".in", "r", stdin);
    std::freopen(FILE_NAME ".out", "w", stdout);
#endif

    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    int T;
    std::cin >> T;

    while (T --> 0)  Solution::solve();

    return 0;
}