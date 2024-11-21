#include <bits/stdc++.h>
#define filename "subseq3"
using ll = long long;
using ull = unsigned long long;
using i128 = __int128;

namespace Solution {
    int N;
    const int mod = 998244353;

    std::vector<int> origin;
    std::vector<int> result;

    std::map<ull, int> hashed;
    
    template <typename T>
    ull hash(T begin, T end) {
        const int K = 131;
        auto tmp = 1ULL;
        auto ans = 0ULL;
        for (auto it = begin; it != end; it++, tmp *= K) {
            ans += tmp * (*it);
        }
        return ans;
    }

    void dfs(int p) {
        if (p >= N) {
            if (not result.empty()) {
                // for (auto i: result)  std::cout << i << ' ';
                auto current_hash = hash(result.begin(), result.end());
                // std::cout << current_hash;
                // std::cout << std::endl;
                hashed[current_hash]++;
                if (hashed[current_hash] == mod)  hashed[current_hash] = 0;
            }
            return;
        }
        dfs(p+1);
        result.push_back(origin[p]);
        dfs(p+1);
        result.pop_back();
    }

    void solve() {
        std::cin >> N;

        origin.resize(N);
        for (auto i = 0; i < N; i++)  std::cin >> origin[i];

        if (std::all_of(origin.begin(), origin.end(), [&](auto x) {return x==1;})) {
            auto ans = 0;
            for (auto i = 1; i <= N; i++) {
                ans = (ans + i * i * i) % mod;
            }
            std::cout << ans << std::endl;
            return;
        }

        std::vector<int> permutation(N);
        for (auto i = 1; i <= N; i++)  permutation[i-1] = i;

        std::vector<int> sorted(origin);
        std::sort(sorted.begin(), sorted.end());
        // if (hash(sorted.begin(), sorted.end()) == hash(permutation.begin(), permutation.end())) {
        if (sorted == permutation) {
            std::vector<std::vector<int>> C(256, std::vector<int>(256));
            C[0][0] = 1;
            for (auto i = 1; i < 256; i++) {
                C[i][0] = 1;
                for (auto j = 1; j <= i; j++) {
                    C[i][j] = (C[i-1][j-1] + C[i-1][j]) % mod;
                }
            }

            auto ans = 0;
            for (auto i = 1; i <= N; i++) {
                ans = (ans + C[N][i]) % mod;
            }
            std::cout << ans << std::endl;
            return;
        }

        dfs(0);

        auto ans = 0;
        for (auto [key, times]: hashed) {
            ans = (ans + times * times * times) % mod;
        }

        std::cout << ans << std::endl;
    }
}

int main() {
#ifndef ONLINE_JUDGE
    std::freopen(filename".in", "r", stdin);
    std::freopen(filename".out", "w", stdout);
#endif
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution::solve();
    return 0;
}