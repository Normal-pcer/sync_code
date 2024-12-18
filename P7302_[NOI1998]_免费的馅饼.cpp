/**
 * @link https://www.luogu.com.cn/problem/P7302
 */

#include "./lib"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_1307252172119349 {
    class SegTree {
        struct Node {
            int ls, rs;
            int max;
        };
        std::deque<Node> tr {{}};
        int root, root_begin, root_end;
        void pushUp(int p) {
            auto ls = tr[p].ls, rs = tr[p].rs;
            tr[p].max = std::max(tr[ls].max, tr[rs].max);
        }
        int max(int begin, int end, int &p, int n_begin, int n_end) {
            if (p == 0)  return 0;
            if (n_begin >= begin and n_end <= end) {
                return tr[p].max;
            }
            auto n_mid = std::midpoint(n_begin, n_end);
            auto res = 0;
            if (n_mid > begin)  chkMax(res, max(begin, end, tr[p].ls, n_begin, n_mid));
            if (n_mid < end)  chkMax(res, max(begin, end, tr[p].rs, n_mid, n_end));
            return res;
        }
        void update(int pos, int val, int &p, int n_begin, int n_end) {
            if (p == 0)  p = tr.size(), tr.push_back({});
            if (n_begin == pos and n_end == pos + 1) {
                chkMax(tr[p].max, val);
                return;
            }
            auto n_mid = std::midpoint(n_begin, n_end);
            if (n_mid > pos)  update(pos, val, tr[p].ls, n_begin, n_mid);
            else  update(pos, val, tr[p].rs, n_mid, n_end);
            pushUp(p);
        }
    public:
        SegTree(int begin, int end): root(0), root_begin(begin), root_end(end) {}
        void update(int pos, int val) {
            update(pos, val, root, root_begin, root_end);
        }
        int max(int begin, int end) {
            return max(begin, end, root, root_begin, root_end);
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);
        
        int W, N;  std::cin >> W >> N;
        struct Cake { int time, pos, val; };
        std::vector<Cake> cakes(N);
        for (auto &[time, pos, val]: cakes)  std::cin >> time >> pos >> val;
        ranges::sort(cakes, std::less{}, lam(x, x.time));

        // std::vector<int> F(cakes.size());
        // auto ans = 0;
        // for (auto i: range(N)) {
        //     for (auto j: range(i)) {
        //         if (abs(cakes.at(i).pos - cakes.at(j).pos) < (cakes.at(i).time - cakes.at(j).time) * 2) {
        //             chkMax(F.at(i), F.at(j));
        //         }
        //     }
        //     F.at(i) += cakes.at(i).val;
        //     chkMax(ans, F.at(i));
        // }

        std::vector sgts(2, SegTree(0, 2e8+5));
        auto ans = 0;
        for (auto i: range(N)) {
            auto cur = 0;
            auto f0 = 2 * cakes.at(i).time - cakes.at(i).pos;
            auto f1 = 2 * cakes.at(i).time + cakes.at(i).pos;
            chkMax(cur, sgts.at(0).max(0, f0));
            chkMax(cur, sgts.at(1).max(0, f1));
            cur += cakes.at(i).val;

            sgts.at(0).update(f0, cur);
            sgts.at(1).update(f1, cur);
            chkMax(ans, cur);
        }
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1307252172119349::solve();
    return 0;
}
