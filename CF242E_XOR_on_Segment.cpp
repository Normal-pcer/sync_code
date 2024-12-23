/**
 * @link https://www.luogu.com.cn/problem/CF242E
 * @link http://codeforces.com/problemset/problem/242/E
 */

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_1225567919228798 {
    class SegTree {
        struct Node {
            int begin, end, len;
            int sum = 0;
            bool tag = false;
        };
        std::vector<Node> tr;
        std::vector<char> init;
#define ls (p<<1)
#define rs (p<<1|1)
        void push_up(int p) {
            tr[p].sum = tr[ls].sum + tr[rs].sum;
        }
        void push_down(int p) {
            if (tr[p].tag) {
                tr[ls].sum = tr[ls].len - tr[ls].sum;
                tr[rs].sum = tr[rs].len - tr[rs].sum;
                tr[ls].tag = !tr[ls].tag, tr[rs].tag = !tr[rs].tag;
                tr[p].tag = false;
            }
        }
        void build(int begin, int end, int p) {
            tr[p].begin = begin, tr[p].end = end, tr[p].len = end - begin;
            if (begin + 1 == end) {
                tr[p].sum = init.at(begin);
                return;
            }
            auto mid = std::midpoint(begin, end);
            build(begin, mid, ls), build(mid, end, rs);
            push_up(p);
        }
    public:
        void swap(SegTree &other) {
            tr.swap(other.tr), init.swap(other.init);
        }
        SegTree(std::vector<char> &&init): tr(init.size() << 2), init(std::move(init)) {
            if (this->init.empty())  return;
            build(0, this->init.size(), 1);
        }
        void invert(int begin, int end, int p = 1) {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                tr[p].sum = tr[p].len - tr[p].sum;
                tr[p].tag = !tr[p].tag;
                return;
            }
            push_down(p);
            if (tr[ls].end > begin)  invert(begin, end, ls);
            if (tr[rs].begin < end)  invert(begin, end, rs);
            push_up(p);
        }
        int sum(int begin, int end, int p = 1) {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                return tr[p].sum;
            }
            push_down(p);
            auto ans = 0;
            if (tr[ls].end > begin)  ans += sum(begin, end, ls);
            if (tr[rs].begin < end)  ans += sum(begin, end, rs);
            return ans;

        }
        void print(int p = 1) {
            push_down(p);
            std::cout << std::format("[{}, {}): {}, {}", tr[p].begin, tr[p].end, tr[p].sum, tr[p].tag? "true": "false") << std::endl;
            if (tr[p].begin + 1 == tr[p].end)  return;
            print(ls), print(rs);
        }
#undef ls
#undef rs
    };
    class Bits {
        std::vector<SegTree> segs;
    public:
        Bits(std::vector<int> const &init) {
            // auto max = *std::max_element(init.begin(), init.end());
            auto digits = 20;
            segs.resize(digits, SegTree(std::vector<char>{}));

            for (auto i: range(digits)) {
                std::vector<char> tmp(init.size());
                std::transform(init.begin(), init.end(), tmp.begin(), lam(x, !!(x & (1<<i))));
                segs[i] = SegTree(std::move(tmp));
            }
        }
        void exclusiveOr(int begin, int end, int num) {
            for (auto i: range(std::__lg(num) + 1)) {
                if (num & (1<<i)) {
                    segs.at(i).invert(begin, end);
                }
            }
        }
        ll sum(int begin, int end) {
            auto ans = 0LL;
            for (auto i: range(segs.size())) {
                ans += (ll)segs.at(i).sum(begin, end) << i;
            }
            return ans;
        } 
        void print() {
            for (auto i: range(5)) {
                std::cout << std::format("i = {}", i) << std::endl;
                segs.at(i).print();
            }
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;  std::cin >> N;
        std::vector<int> init(N+1);
        for (auto &i: init | views::drop(1))  std::cin >> i;
        
        Bits bits(init);

        // bits.print();

        int M;  std::cin >> M;
        for (auto _: range(M)) {
            int op, x, y;  std::cin >> op >> x >> y;
            if (op == 1) {
                std::cout << bits.sum(x, y+1) << std::endl;
            } else {
                int z;  std::cin >> z;
                bits.exclusiveOr(x, y+1, z);
            }
            never {
                bits.print();
            }
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1225567919228798::solve();
    return 0;
}
