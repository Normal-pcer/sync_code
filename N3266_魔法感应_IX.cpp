/**
 * @link https://neooj.com:8082/oldoj/problem.php?id=3266
 */

#include "./lib_v3.hpp"

#include "./libs/io.hpp"

using namespace lib;

namespace Solution_1640310679725336 {
    class SegTree {
        struct Node {
            int begin, end;
            int sum;
        };
        std::vector<Node> tr;
        std::vector<int> lastPos;
        int size = 1;
#define ls (p << 1)
#define rs (p << 1 | 1)
        void pushUp(int p) {
            tr[p].sum = tr[ls].sum + tr[rs].sum;
        }
        void build(int begin, int end, int p = 1) {
            tr[p].begin = begin, tr[p].end = end;
            if (begin + 1 == end)  return;
            auto mid = begin + ((end - begin) >> 1);
            build(begin, mid, ls), build(mid, end, rs);
            pushUp(p);
        }
        void add(int pos, int val, int p = 1) {
            if (tr[p].begin == pos and tr[p].begin + 1 == tr[p].end) {
                tr[p].sum += val;
                return;
            }
            if (pos < tr[ls].end)  add(pos, val, ls);
            else  add(pos, val, rs);
            pushUp(p);
        }
        int sum(int begin, int end, int p = 1) const {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                return tr[p].sum;
            }
            auto res = 0;
            if (tr[ls].end > begin)  res += sum(begin, end, ls);
            if (tr[rs].begin < end)  res += sum(begin, end, rs);
            return res;
        }
    public:
        SegTree(int MaxPos, int MaxVal): tr((MaxPos+1) << 2), lastPos(MaxVal+1) {
            build(0, MaxPos);
        }
        int pushBack(int x) {
            auto left = lastPos.at(x);
            if (left != 0) {
                add(left, -1);
            }
            auto res = left? sum(left, size): 0;
            lastPos.at(x) = size, add(size, +1), size++;
            return res;
        }
#undef ls
#undef rs
    };
    void solve() {
        int N;  io >> N;
        std::vector<int> groups(N);
        for (auto &i: groups)  io >> i;
        groups.insert(groups.begin(), 0);

        SegTree sgt(groups.size(), *std::max_element(groups.begin(), groups.end()));
        auto ans = 0;
        for (auto i = 1; i < (int)groups.size(); i++) {
            chkMax(ans, sgt.pushBack(groups[i]));
        }
        
        io << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1640310679725336::solve();
    return 0;
}
