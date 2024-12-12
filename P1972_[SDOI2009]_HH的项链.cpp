/**
 * @link https://www.luogu.com.cn/problem/P1972
 */

#include "./lib"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_2248219951765610 {
    class SegTree {
        struct Node {
            int begin, end;
            int sum;
        };
        std::vector<int> lastPos;
        std::vector<Node> tr;
        int size = 1;
#define ls (p<<1)
#define rs (p<<1|1)
        void pushUp(int p) {
            tr[p].sum = tr[ls].sum + tr[rs].sum;
        }
        void add(int pos, int val, int p = 1) {
            if (tr[p].begin == pos and pos + 1 == tr[p].end) {
                tr[p].sum += val;
                return;
            }
            if (pos < tr[rs].begin)  add(pos, val, ls);
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
        void build(int begin, int end, int p = 1) {
            tr[p].begin = begin, tr[p].end = end;
            if (begin + 1 == end)  return;
            auto mid = std::midpoint(begin, end);
            build(begin, mid, ls), build(mid, end, rs);
        }
    public:
        SegTree(size_t MaxPos, size_t MaxVal): lastPos(MaxVal+1), tr((MaxPos+1)<<2) {
            build(0, MaxPos);
        }
        void pushBack(int x) {
            if (lastPos.at(x)) {
                add(lastPos.at(x), -1);
            }
            add(size, 1);
            lastPos.at(x) = size, size++;
        }
        int sumFrom(int begin) const {
            return sum(begin, size);
        }
#undef ls
#undef rs
    };
    struct Query {
        int left, right;
        int index;
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;  std::cin >> N;
        std::vector<int> kinds(N+1);
        for (auto &i: kinds | views::drop(1))  std::cin >> i;
        int M;  std::cin >> M;
        std::vector<Query> queries(M);
        for (auto i: range(queries.size())) {
            auto &[x, y, index] = queries[i];
            std::cin >> x >> y, index = i;
        }

        std::sort(queries.begin(), queries.end(), lam(x, y, x.right < y.right));
        SegTree sgt(N+1, *std::max_element(kinds.begin(), kinds.end()));
        std::vector<int> response(M);
        for (auto prev_r = 0, i = 0; i != (int)queries.size(); i++) {
            auto [l, r, index] = queries[i];
            while (prev_r != r)  prev_r++, sgt.pushBack(kinds.at(prev_r));
            response.at(index) = sgt.sumFrom(l);
        }
        for (auto i: response)  std::cout << i << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_2248219951765610::solve();
    return 0;
}
