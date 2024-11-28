/**
 * 
 */

#include "./lib"

#include "./libs/io.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_5420335328929199 {
    
    struct Cow {
        int pos, type;

        std::strong_ordering operator<=> (const Cow &other) const noexcept = default;
    };
    
    void solve() {
        int N;  io >> N;
        std::vector<Cow> cows(N);
        std::vector<int> types;
        for (auto &[pos, type]: cows)  io >> pos >> type, types.push_back(type);
        ranges::sort(cows);

        auto max_type = -inf;
        auto total_type = 0;
        ranges::sort(types), types.erase(std::unique(types.begin(), types.end()), types.end());
        total_type = types.size(), max_type = *std::max_element(types.begin(), types.end());

        std::map<int, int> cnt;

        auto type_cnt = 0;
        auto ans = inf;
        for (size_t ptr1 = 0, ptr2 = 0; ptr2 != cows.size(); ptr2++) {
            if (cnt[cows[ptr2].type] == 0)  type_cnt++;
            cnt[cows[ptr2].type]++;
            while (ptr1 != ptr2 and cnt[cows.at(ptr1).type] > 1)  cnt[cows[ptr1].type]--, ptr1++;
            if (type_cnt == total_type)  chkMin(ans, cows.at(ptr2).pos - cows.at(ptr1).pos);
        }

        io << ans << endl;
    }
}

int main() {
    initDebug;
    Solution_5420335328929199::solve();
    return 0;
}
