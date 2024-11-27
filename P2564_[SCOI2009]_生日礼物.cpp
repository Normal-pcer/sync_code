/**
 * @link https://www.luogu.com.cn/problem/P2564
 */

#include "./lib"

#include "./libs/io.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_1131332509775190 {
    void solve() {
        struct Object {
            int pos, color;

            auto operator<=> (Object const &other) const noexcept = default;
        };

        int N, K;  io >> N >> K;
        std::vector<Object> objects; 

        for (auto i: range(K)) {
            int cnt;  io >> cnt;
            for (auto _: range(cnt)) {
                int pos;  io >> pos;
                objects.push_back({pos, i});
            }
        }

        ranges::sort(objects);
        debug {
            for (auto [pos, color]: objects) {
                std::cout << std::format("{} {}", pos, color) << std::endl;
            }
        }

        std::vector<int> cnt(K);

        std::deque<int> q;
        auto p1 = 0, p2 = 0, c = 0;
        auto ans = infLL;
        for (p1 = 0, p2 = 0; p2 != (int)objects.size(); p2++) {
            if (cnt.at(objects.at(p2).color) == 0)  c++;
            cnt.at(objects.at(p2).color)++;
            while (p2 != p1 and cnt.at(objects.at(p1).color) > 1)  cnt.at(objects.at(p1).color)--, p1++;
            if (c == K)  chkMin(ans, (ll)objects.at(p2).pos - objects.at(p1).pos);
            // std::cout << std::format("{} {}", p1, p2) << std::endl;
            // for (auto i: cnt)  std::cout << i << ' ';
            // std::cout << std::endl;
        }

        io << ans << endl;
    }
}

int main() {
    initDebug;
    Solution_1131332509775190::solve();
    return 0;
}
