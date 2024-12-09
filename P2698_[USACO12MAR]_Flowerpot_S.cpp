/**
 * @link https://www.luogu.com.cn/problem/P2698
 */

#include "./lib_v2.hpp"

#include "./libs/io.hpp"

#include "./libs/range.hpp"

#include "./libs/binary.hpp"

using namespace lib;

namespace Solution_5418195447506292 {
    void solve() {
        int N, D;  io >> N >> D;
        std::vector<std::pair<int, int>> drips(N);

        for (auto &[x, y]: drips)  io >> x >> y;
        ranges::sort(drips);

        auto check = [&](int W) {
            // W++;
            std::deque<int> increasing;
            std::deque<int> decreasing;

            int ans = -inf;

            for (auto i: range(drips.size())) {
                auto [x, y] = drips.at(i);

                while (not increasing.empty() and x - drips.at(increasing.front()).first > W)  increasing.pop_front();
                while (not decreasing.empty() and x - drips.at(decreasing.front()).first > W)  decreasing.pop_front();

                while (not increasing.empty() and drips.at(increasing.back()).second >= y)  increasing.pop_back();
                while (not decreasing.empty() and drips.at(decreasing.back()).second <= y)  decreasing.pop_back();

                increasing.push_back(i), decreasing.push_back(i);
                if (not increasing.empty() and not decreasing.empty() and x >= W) {
                    auto cur = drips.at(decreasing.front()).second - drips.at(increasing.front()).second;
                    chkMax(ans, cur);
                }
            }

            if (not increasing.empty() and not decreasing.empty() and drips.back().first >= W) {
                auto cur = drips.at(decreasing.front()).second - drips.at(increasing.front()).second;
                chkMax(ans, cur);
            }

            debug io << std::format("check {} -> {}", W, ans) << endl;
            return ans;
        };

        auto max = drips.back().first + 5;
        auto ans = binary::lower_bound_mapping(0, max, D, check);
        if (ans >= max)  io << -1 << endl;
        else  io << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    Solution_5418195447506292::solve();
    return 0;
}
