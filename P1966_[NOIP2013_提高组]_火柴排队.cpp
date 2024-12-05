/**
 * @link https://www.luogu.com.cn/problem/P1966
 */

#include "./lib"

using namespace lib;

namespace Solution_1163391625770544 {
    int N;  const int _N = 262'144;
    const int mod = 99'999'997;

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
            if (res >= mod)  res -= mod;
            x -= lowbit(x);
        }
        return res;
    }

    std::vector<int> values = {-inf};
    inline int getIndex(int x) {
        return std::lower_bound(values.begin(), values.end(), x) - values.begin();
    }

    void solve() {
#if true
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);
#endif

        std::cin >> N;
        std::vector<std::pair<int, int>> pairs(N);
        for (auto &pair: pairs)  std::cin >> pair.first;
        for (auto &pair: pairs)  std::cin >> pair.second;
        
        std::vector<int> all_first(N), all_second(N);
        std::transform(pairs.begin(), pairs.end(), all_first.begin(), lam(x, x.first));
        std::transform(pairs.begin(), pairs.end(), all_second.begin(), lam(x, x.second));
        std::sort(all_first.begin(), all_first.end());
        std::sort(all_second.begin(), all_second.end());

        auto mapFirst = [&](int x) {
            return std::lower_bound(all_first.begin(), all_first.end(), x) - all_first.begin();
        };
        auto mapSecond = [&](int x) {
            return std::lower_bound(all_second.begin(), all_second.end(), x) - all_second.begin();
        };

        std::vector<int> height(N), buc(N);
        for (auto i = 0ULL; i < pairs.size(); i++) {
            auto mapped_first = mapFirst(pairs[i].first);
            buc[mapped_first] = i;
        }
        for (auto i = 0ULL; i < pairs.size(); i++) {
            auto mapped_second = mapSecond(pairs[i].second);
            height[i] = buc[mapped_second] + 1;
        }

        debug {
            for (auto i: height) {
                std::cout << i << ' ';
            }
            std::cout << std::endl;
        }

        auto cnt_reversed = 0LL;  // 逆序对数量
        for (auto i = 0; i < N; i++) {
            auto cur = height[i];
            auto cur_leq = query(cur);
            auto cur_greater = i - cur_leq;
            cnt_reversed += cur_greater;
            cnt_reversed %= mod;
            update(cur, 1);
        }

        std::cout << cnt_reversed << std::endl;
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    Solution_1163391625770544::solve();
    return 0;
}