/**
 * @link https://www.luogu.com.cn/problem/P1966
 */

#include "./lib"

using namespace lib;

namespace Solution_1163391625770544 {
    int N;  const int _N = 262'144;
    const int mod = 999'999'997;

    ll c[_N];
    inline int lowbit(int x) { return x & -x; }
    void update(int x, ll val) {
        while (x < _N) {
            c[x] += val;
            if (c[x] >= mod)  c[x] -= mod;
            x += lowbit(x);
        }
    }

    int query(int x) {
        auto res = 0LL;
        while (x) {
            res += c[x];
            x -= lowbit(x);
        }
        return res;
    }

    std::vector<int> values = {-inf};
    inline int getIndex(int x) {
        return std::lower_bound(values.begin(), values.end(), x) - values.begin();
    }

    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        std::cin >> N;
        std::vector<std::pair<int, int>> pairs(N);
        for (auto &pair: pairs)  std::cin >> pair.first;
        for (auto &pair: pairs)  std::cin >> pair.second;
        
        std::set<int> values_first;
        for (auto i = 0ULL; i < pairs.size(); i++) {
            values_first.insert(pairs[i].first);
        }
        std::map<int, int> firstValueToRank;
        auto rank = 0;
        for (auto i: values_first) {
            rank++;
            firstValueToRank[i] = rank;
        }

        auto cnt_reversed = 0LL;  // 逆序对数量
        for (auto i = 0; i < N; i++) {
            // auto cur = height[i];
            // auto cur_leq = query(getIndex(cur));
            // auto cur_greater = i - cur_leq;
            // cnt_reversed += cur_greater;
            // cnt_reversed %= mod;
            // update(getIndex(cur), 1);
        }

        std::cout << cnt_reversed << std::endl;
    }
}

int main() {
    initDebug;
    Solution_1163391625770544::solve();
    return 0;
}