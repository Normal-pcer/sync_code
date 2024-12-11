/**
 * @link https://neooj.com:8082/oldoj/problem.php?id=3342
 */

#include "lib"
using namespace lib;

namespace Solution_9736888855439916 {
    std::vector<int> values;
    struct Segment {
        int left, right, index;
        bool update;
    };

    std::vector<Segment> segs;
    std::vector<int> values;
    int getIndex(int x) {
        auto it = std::lower_bound(values.begin(), values.end(), x);
        assert(*it == x);
        return std::distance(values.begin(), it);
    }
    void cdq(decltype(segs)::iterator begin, decltype(segs)::iterator end) {
        auto x = std::distance(segs.begin(), begin), y = std::distance(segs.begin(), end);
        debug  std::cout << std::format("cdq({}, {})", x, y) << std::endl;
        if (begin + 1 == end)  return;
        // auto mid = std::midpoint(begin, end);
        auto mid = begin + (std::distance(begin, end) >> 1);
    }
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, Q;  std::cin >> N;
        for (auto i = 0; i < N; i++) {
            auto &[left, right, index, update] = segs.at(i);
            std::cin >> left >> right, index = i, update = false;
            values.push_back(left), values.push_back(right);
        }
        std::cin >> Q;
        for (auto i = 0; i < N; i++) {
            auto &[left, right, index, update] = segs.at(i);
            std::cin >> left >> right, index = i, update = true;
            values.push_back(left), values.push_back(right);
        }

        std::sort(values.begin(), values.end());
        values.erase(std::unique(values.begin(), values.end()), values.end());
        for (auto &item: segs)  item.left = getIndex(item.left), item.right = getIndex(item.right);

    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_9736888855439916::solve();
    return 0;
}
