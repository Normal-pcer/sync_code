/**
 * @link https://www.luogu.com.cn/problem/P5522
 */

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_1439430371952756 {
    class SegTree {
        struct Node {
            int begin, end;
            unsigned s = 0, t = 0;
        };
        std::vector<Node> tr;
        std::vector<std::string> init;

#define ls (p << 1)
#define rs (p << 1 | 1)
        static auto getST(std::string const &str) -> std::pair<unsigned, unsigned> {
            unsigned s = 0, t = 0;
            for (auto i: range(str.size())) {
                switch (str[i]) {
                    case '0':  s |= 1 << i; break;
                    case '1':  t |= 1 << i; break;
                }
            }
            return {s, t};
        }
        Node &merge(Node &root, Node const &left, Node const &right) const {
            root.s = left.s | right.s;
            root.t = left.t | right.t;
            return root;
        }
        void push_up(int p) {
            merge(tr[p], tr[ls], tr[rs]);
        }
        void update(int pos, unsigned s, unsigned t, int p = 1) {
            if (tr[p].begin == pos and pos + 1 == tr[p].end) {
                tr[p].s = s, tr[p].t = t;
                return;
            }
            if (pos < tr[ls].end)  update(pos, s, t, ls);
            else  update(pos, s, t, rs);
            push_up(p);
        }
        void build(int begin, int end, int p = 1) {
            tr[p].begin = begin, tr[p].end = end;
            if (end - begin == 1) {
                std::tie(tr[p].s, tr[p].t) = getST(init.at(begin));
                return;
            }
            auto mid = std::midpoint(begin, end);
            build(begin, mid, ls), build(mid, end, rs);
            push_up(p);
        }

    public:
        SegTree(std::vector<std::string> &&initial): 
                tr(initial.size() << 2), init(std::move(initial)) {
            build(0, init.size());
        }
        void update(int pos, std::string const &str) {
            auto [s, t] = getST(str);
            update(pos, s, t);
        }
        Node query(int begin, int end, int p = 1) {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                return tr[p];
            }
            Node res0 = {begin, end};
            Node res1 = res0, res2 = res0;
            if (tr[ls].end > begin)  res1 = query(begin, end, ls);
            if (tr[rs].begin < end)  res2 = query(begin, end, rs);
            return merge(res0, res1, res2);
        }
#undef ls
#undef rs
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, M, Q;  std::cin >> N >> M >> Q;
        std::vector<std::string> init(M);
        for (auto &s: init)  std::cin >> s;
        init.insert(init.begin(), "");
        
        SegTree sgt(std::move(init));
        auto ans = 0;
        for (auto _: range(Q)) {
            int op;  std::cin >> op;
            if (op == 0) {
                int l, r;  std::cin >> l >> r;
                auto node = sgt.query(l, r+1);
                auto ans_cur = node.s & node.t
                        ? 0
                        : 1 << std::__popcount(~(node.s | node.t) & ((1<<N)-1));
                ans ^= ans_cur;
            } else {
                int pos;  std::string str;
                std::cin >> pos >> str;
                sgt.update(pos, str);
            }
        }
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1439430371952756::solve();
    return 0;
}
