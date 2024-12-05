/**
 * 
 */

#include "./lib"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_1547776023366501 {
    std::vector<ll> ps;

    int L, R;

    template <typename T = std::vector<ll>::iterator>
    ll cdq(T begin, T end) {
        auto mid = begin + (std::distance(begin, end) >> 1);
        ll ans = cdq(begin, mid);
        std::vector right_cp(mid, end);

        auto i = begin, j = mid;
        while (i != mid or j != end) {
            if (j == end or (i != mid and *i < *j)) {

            }
        }

        std::copy(right_cp.begin(), right_cp.end(), mid);
    }

    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;  std::cin >> N >> L >> R;
        ps.resize(N);
        for (auto &i: ps)  std::cin >> i;
        std::partial_sum(ps.begin(), ps.end(), ps.begin());
        ps.insert(ps.begin(), 0);

        for (auto i: ps)  std::cout << i << ' ';
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    Solution_1547776023366501::solve();
    return 0;
}
