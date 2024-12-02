/**
 * @link https://www.luogu.com.cn/problem/P11362
 */

#include "./lib"

using namespace lib;

namespace Solution_1767269893947123 {

    inline ll pow(ll a, ll b, const int p) {
        ll res = 1;
        for (; b; b >>= 1, a = a * a % p)  if (b&1) res = res * a % p;
        return res;
    }

    int solve() {
        int N, M, V;  std::cin >> N >> M >> V;
        std::vector<std::pair<int, int>> limits(M);

        for (auto &[c, d]: limits)  std::cin >> c >> d;

        std::map<int, int> fixed;
        for (auto [c, d]: limits) {
            if (fixed.find(c) == fixed.end())  fixed.insert({c, d});
            else if (fixed.at(c) != d)  return 0;
        }

        std::vector<int> keys, values;
        for (auto [k, v]: fixed)  keys.push_back(k), values.push_back(v);
    
        auto ans = 1LL;  const int mod = 1e9+7;
        // if (keys.front() != 1)  ans = (pow(V, 2 * keys.front(), mod) - (pow(V, keys.front() - 1, mod) * (V-1) % mod) + mod) % mod;
        if (keys.front() != 1) {
            ans *= (pow(V, 2 * (keys.front() - 1), mod)) % mod;
            ans %= mod;
        }

        for (auto i = 0; i < (int)keys.size() - 1; i++) {
            auto y = keys.at(i+1), x = keys.at(i);
            auto ans_cur = (pow(V, 2 * (y - x), mod) - (pow(V, y - x - 1, mod) * (V - 1) % mod) + mod) % mod;
            ans = ans * ans_cur % mod;
        }

        if (keys.back() != N) {
            ans *= (pow(V, 2 * (N - keys.back()), mod)) % mod;
            ans %= mod;
        }
        return ans;
    }
}

int main() {
    initDebug;
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
    int T;  std::cin >> T;
    while (T --> 0)  std::cout << Solution_1767269893947123::solve() << '\n';
    return 0;
}
