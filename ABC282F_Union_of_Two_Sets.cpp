/**
 * 
 */

#include "./lib_legecy.hpp"
using namespace lib;

namespace Solution_1779691446829941 {

    void solve() {
        std::ios::sync_with_stdio(false), std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;
        std::cin >> N;

        auto lg = std::__lg(N);
        std::vector<std::pair<int, int>> pairs;
        std::vector<std::vector<size_t>> index(N+5, std::vector<size_t>(lg+2));
        from(j, 0, lg) {
            from(l, 1, N) {
                auto len = 1 << j;
                auto r = l + len - 1;
                if (r > N)  break;

                pairs.push_back({l, r});
                index.at(l).at(j) = pairs.size();
            }
        }

        std::cout << pairs.size() << std::endl;
        for (auto [l, r]: pairs) {
            std::cout << l << ' ' << r << '\n';
        }
        std::cout << std::flush;

        int Q;
        std::cin >> Q;
        from(_, 1, Q) {
            int L, R;
            std::cin >> L >> R;

            auto len = R - L + 1;
            auto lg = std::__lg(len);
            
            auto seg1_len = 1 << (lg);
            auto seg1_left = L;
            auto seg1_index = index.at(seg1_left).at(lg);

            auto seg2_left = R - seg1_len + 1;
            auto seg2_index = index.at(seg2_left).at(lg);

            std::cout << seg1_index << ' ' << seg2_index << std::endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    Solution_1779691446829941::solve();
    return 0;
}