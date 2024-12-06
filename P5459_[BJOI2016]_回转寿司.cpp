/**
 * @link https://www.luogu.com.cn/problem/P5459
 */

#include "./lib"

#include "./libs/range.hpp"


using namespace lib;

namespace Solution_1547776023366501 {
    std::vector<ll> ps;

    int L, R;

    template <typename T = std::vector<ll>::iterator>
    ll cdq(T begin, T end) {
        auto x = begin - ps.begin(), y = end - ps.begin();

        if (begin + 1 == end)  return 0;
        auto mid = begin + (std::distance(begin, end) >> 1);
        ll ans = cdq(begin, mid);
        std::vector right_cp(mid, end);

        std::sort(begin, mid), std::sort(mid, end);

        auto i = begin, j = mid;
        auto front = begin;  // [front, i) 和 j 之间产生贡献
        debug std::cout << std::format("cdq({}, {})", x, y) << std::endl;
        while (j != end) {
            while (i != mid and *i + L <= *j)  i++;
            while (front != i and *front + R < *j)  front++;

            debug {
                std::cout << std::format("[{}]{} With ", j - ps.begin(), *j);
                for (auto it: range(front, i)) {
                    std::cout << std::format("[{}]{}", it - ps.begin(), *it);
                }
                std::cout << std::endl;
            }

            ans += std::distance(front, i);
            j++;
        }

        std::copy(right_cp.begin(), right_cp.end(), mid);
        return ans + cdq(mid, end);
    }

    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;  std::cin >> N >> L >> R;
        ps.resize(N);
        for (auto &i: ps)  std::cin >> i;
        std::partial_sum(ps.begin(), ps.end(), ps.begin());
        ps.insert(ps.begin(), 0);

        debug for (auto i: ps)  std::cout << i << ' ';
        debug std::cout << std::endl;

        auto ans = cdq(ps.begin(), ps.end());
        std::cout << ans << std::endl;
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    Solution_1547776023366501::solve();
    return 0;
}
