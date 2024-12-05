/**
 * @link https://www.luogu.com.cn/problem/P2717
 */

#include "./lib"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_1810923481163192 {
    std::vector<int> a;
    std::vector<int> ps;

    template <typename T = std::vector<int>::iterator>
    ll cdq(T begin, T end) {
        auto x = begin - ps.begin(), y = end - ps.begin();
        debug  std::cout << std::format("cdq({}, {})", x, y) << std::endl;
        if (begin + 1 == end)  return 0;

        auto mid = begin + (std::distance(begin, end) >> 1);
        auto ans = cdq(begin, mid);
        std::vector<int> right_backup(mid, end);
        std::sort(begin, mid), std::sort(mid, end);

        auto i = begin, j = mid;
        while (i != mid or j != end) {
            if (j == end or (i != mid and *i <= *j)) {
                i++;
            } else {
                ans += i - begin;
                debug {
                    std::cout << std::format("[{}]{} update with ", j - ps.begin(), *j);
                    for (auto it = ps.begin(); it != i; it++)  std::cout << std::format("[{}]{} ", it - ps.begin(), *it);
                    std::cout << std::endl;
                }
                j++;
            }
        }

        std::copy(right_backup.begin(), right_backup.end(), mid);
        return ans + cdq(mid, end);
    }

    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, K;  std::cin >> N >> K;
        a.resize(N);
        for (auto &i: a)  std::cin >> i, i -= K;

        ps.resize(a.size()), std::partial_sum(a.begin(), a.end(), ps.begin());
        ps.insert(ps.begin(), 0);

        debug for (auto i: ps)  std::cout << i << ' ';
        debug std::cout << std::endl;

        auto ans = cdq(ps.begin(), ps.end());

        std::cout << ans << std::endl;
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    Solution_1810923481163192::solve();
    return 0;
}
