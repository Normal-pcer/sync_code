/**
 * 
 */

#include "./lib"

#include "./libs/io.hpp"

using namespace lib;

namespace Solution_1105056652004698 {
    struct MaxItem {
        int val, src;
        bool operator< (const MaxItem &other) const {
            return val < other.val;
        }
    };
    class BIT {
        using Node = MaxItem;
        std::vector<Node> c;
        static int constexpr lowbit(int x) { return x & -x; }
    public:
        BIT(int N): c(N+1) {}
        MaxItem max(int x) const {
            x++;  auto res = Node{0, -1};
            while (x) {
                chkMax(res, c.at(x));
                x -= lowbit(x);
            }
            return res;
        }
        void update(int x, Node val) {
            x++;
            while (x < (int)c.size()) {
                chkMax(c.at(x), val);
                x += lowbit(x);
            }
        }
    };
    void solve() {
        int N;  io >> N;
        std::vector<int> nums(N);
        for (auto &i: nums)  io >> i;

        std::vector<int> values(nums.begin(), nums.end());
        std::sort(values.begin(), values.end()), values.erase(std::unique(values.begin(), values.end()), values.end());
        auto getIndex = [&](int x) {
            auto it = std::lower_bound(values.begin(), values.end(), x);
            assert(*it == x);
            return std::distance(values.begin(), it);
        };

        std::transform(nums.begin(), nums.end(), nums.begin(), getIndex);
        std::vector<MaxItem> F(N);
        BIT tree(values.size());
        MaxItem max{-1, -1};
        for (auto i = 0; i < N; i++) {
            auto item = nums.at(i);
            auto cur = tree.max(item);
            cur.val++;
            F.at(i) = cur;
            tree.update(item, {cur.val, i});
            chkMax(max, {cur.val, i});
        }

        std::vector<int> steps {max.src};
        while (max.src != -1) {
            max = F.at(max.src);
            steps.push_back(max.src);
        }
        steps.pop_back();
        std::reverse(steps.begin(), steps.end());
        for (auto i: steps)  io << values.at(nums.at(i)) << ' ';
        io << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1105056652004698::solve();
    return 0;
}
