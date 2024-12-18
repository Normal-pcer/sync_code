/**
 * @link https://www.luogu.com.cn/problem/CF1557D
 * @link http://codeforces.com/problemset/problem/1557/D
 */

#include "./lib"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_6606883188646907 {
    struct MaxItem {
        int val = 0, src = -1;
        std::weak_ordering operator<=> (const MaxItem &other) const {
            return val <=> other.val;
        }
        operator bool () const {
            return val != 0;
        }
        operator std::string () const {
            return std::format("MaxItem({}, {})", val, src);
        }
    };
    class SegTree {
        struct Node {
            int begin, end;
            MaxItem max, tag;
        };
        std::vector<Node> tr;
#define ls (p << 1)
#define rs (p << 1 | 1)
        void pushUp(int p) {
            tr[p].max = std::max(tr[ls].max, tr[rs].max);
        }
        void updateNode(int p, MaxItem val) {
            chkMax(tr[p].max, val);
            chkMax(tr[p].tag, val);
        }
        void pushDown(int p) {
            if (tr[p].tag) {
                for (auto s: {ls, rs}) {
                    updateNode(s, tr[p].tag);
                }
                tr[p].tag = {};
            }
        }
        void build(int begin, int end, int p) {
            tr[p].begin = begin, tr[p].end = end;
            if (tr[p].begin + 1 == tr[p].end)  return;
            auto mid = std::midpoint(begin, end);
            build(begin, mid, ls), build(mid, end, rs);
        }
    public:
        SegTree(int begin, int end): tr((end - begin) << 2) {
            build(begin, end, 1);
        }
        MaxItem max(int begin, int end, int p = 1) {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                return tr[p].max;
            }
            pushDown(p);
            MaxItem res;
            if (tr[ls].end > begin)  chkMax(res, max(begin, end, ls));
            if (tr[rs].begin < end)  chkMax(res, max(begin, end, rs));
            return res;
        }
        void update(int begin, int end, MaxItem val, int p = 1) {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                updateNode(p, val);
                return;
            }
            pushDown(p);
            if (tr[ls].end > begin)  update(begin, end, val, ls);
            if (tr[rs].begin < end)  update(begin, end, val, rs);
            pushUp(p);
        }
#undef ls
#undef rs
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, M;  std::cin >> N >> M;
        std::vector<std::vector<std::pair<int, int>>> updates(N);
        std::vector<int> values;
        for (auto _: range(M)) {
            int line, x, y;  std::cin >> line >> x >> y;
            line--, y++;
            values.push_back(x), values.push_back(y);
            updates.at(line).push_back({x, y});
        }
        std::sort(values.begin(), values.end()), values.erase(std::unique(values.begin(), values.end()), values.end());
        auto getIndex = [&](int x) {
            auto it = std::lower_bound(values.begin(), values.end(), x);
            assert(*it == x);
            return std::distance(values.begin(), it);
        };
        for (auto &line: updates) {
            for (auto &[x, y]: line) {
                x = getIndex(x), y = getIndex(y);
            }
        }

        SegTree sgt(0, values.size());
        std::vector<MaxItem> F(N);
        MaxItem max;
        for (auto i: range(N)) {
            auto &cur = F.at(i);
            for (auto [begin, end]: updates.at(i)) {
                chkMax(cur, sgt.max(begin, end));
            }
            cur.val += 1;
            for (auto [begin, end]: updates.at(i)) {
                sgt.update(begin, end, {cur.val, i});
            }
            chkMax(max, {cur.val, i});
        }

        debug {
            for (auto i: F)  std::cout << std::string(i) << ' ';
            std::cout << endl;
        }

        std::vector<int> steps {max.src};
        while (steps.back() != -1) {
            steps.push_back(F.at(steps.back()).src);
        }
        steps.pop_back();
        std::vector<int> remains;
        {
            std::vector<char> buc(N);
            for (auto i: steps)  buc.at(i) = true;
            for (auto i: range(N)) {
                if (not buc.at(i)) {
                    remains.push_back(i);
                }
            }
        }

        std::cout << remains.size() << endl;
        for (auto i: remains)  std::cout << i + 1 << ' ';
        std::cout << std::endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_6606883188646907::solve();
    return 0;
}
