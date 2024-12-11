
/**
 * @link https://www.luogu.com.cn/problem/SP2916
 */

#include "./lib"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_1730635379514186 {
    class SegTree {
        struct Node {
            int begin, end;
            int max = -inf, lmax = -inf, rmax = -inf;
            int tot = 0;

            operator std::string () const {
                return std::format("[{}, {}) max={}, lmax={}, rmax={}, tot={}", begin, end, max, lmax, rmax, tot);
            }
        };
        std::vector<Node> tr;
        std::vector<int> initializer;
#define ls (p<<1)
#define rs (p<<1 | 1)
        static Node &merge(Node &root, Node const &left, Node const &right) {
            root.max = std::max({left.max, right.max, left.rmax + right.lmax});
            root.lmax = left.lmax, root.rmax = right.rmax;
            root.tot = left.tot + right.tot;
            chkMax(root.lmax, left.tot + right.lmax);
            chkMax(root.rmax, right.tot + left.rmax);
            return root;
        }
        void push_up(int p) {
            merge(tr[p], tr[ls], tr[rs]);
        }
        void build(int begin, int end, int p = 1) {
            tr[p].begin = begin, tr[p].end = end;
            if (begin + 1 == end) {
                tr[p].lmax = tr[p].rmax = tr[p].max = tr[p].tot = initializer.at(begin);
                return;
            }
            auto mid = std::midpoint(begin, end);
            build(begin, mid, ls), build(mid, end, rs);
            push_up(p);
        }
    public:
        SegTree(std::vector<int> &&init): tr(init.size() << 2), initializer(std::move(init)) {
            build(0, initializer.size());
            std::vector<int>().swap(initializer);
        }
        void modify(int pos, int val, int p = 1) {
            if (tr[p].begin + 1 == tr[p].end) {
                tr[p].lmax = tr[p].rmax = tr[p].max = tr[p].tot = val;
                return;
            }
            if (tr[rs].begin <= pos)  modify(pos, val, rs);
            else  modify(pos, val, ls);
            push_up(p);
        }
        Node query(int begin, int end, int p = 1) {
            if (begin == end) {
                return Node{begin, end};
            }
            assert(begin < end);
            if (tr[p].begin >= begin and tr[p].end <= end) {
                return tr[p];
            }
            Node ans = {begin, end};
            Node ans_l = ans, ans_r = ans;
            if (tr[ls].end > begin)  ans_l = query(begin, end, ls);
            if (tr[rs].begin < end)  ans_r = query(begin, end, rs);
            return merge(ans, ans_l, ans_r);
        }
#undef ls
#undef rs
    };
    void solve() {
        int N;  std::cin >> N;
        std::vector<int> init(N+1);
        for (auto &i: init | views::drop(1))  std::cin >> i;
        SegTree seg(std::move(init));

        int M;  std::cin >> M;
        for (auto _: range(M)) {
            int x1, y1, x2, y2;  std::cin >> x1 >> y1 >> x2 >> y2;
            y1++, y2++;
            struct Point {
                int pos;
                int type;  // 0 = left, 1 = right

                auto operator<=> (const Point &other) const noexcept = default;
            };
            std::vector<Point> luo {
                {.pos = x1, .type = 0},
                {.pos = y1, .type = 0},
                {.pos = x2, .type = 1},
                {.pos = y2, .type = 1}
            };

            ranges::sort(luo);
            auto right1_it = ranges::find(luo, Point{.pos = y2, .type = 1});
            auto left0_it = ranges::find(luo, Point{.pos = x1, .type = 0});
            assert(right1_it != luo.end()), assert(left0_it != luo.end());

            // right1 右侧的点没有意义，可以截断
            for (auto it: range(right1_it, luo.end()))  it->pos = right1_it->pos;
            // 同样，left0 左侧的点可以截断
            for (auto it: range(luo.begin(), left0_it))  it->pos = left0_it->pos;
            
            debug {
                for (auto [pos, type]: luo) {
                    std::cout << std::format("{{{}, {}}} ", pos, type) << ' ';
                }
                std::cout << std::endl;
            }

            std::vector<std::pair<int, int>> rgs(2);  // 两种类型的对应范围
            for (auto target_type: {0, 1}) {
                auto pts = luo  | views::filter(lam(x, x.type == target_type))
                                | ranges::to<std::vector>();
                rgs.at(target_type) = {pts.at(0).pos, pts.at(1).pos};
            }
            debug {
                std::cout << rgs.at(0).first << ' ' << rgs.at(0).second << "; " << rgs.at(1).first << ' ' << rgs.at(1).second << std::endl;
            }
            // 如果两个区间没有交集
            if (rgs[0].second <= rgs[1].first) {
                // 中间一段必须取
                auto [a_begin, a_end] = rgs[0];
                auto c_begin = rgs[0].second, c_end = rgs[1].first;
                auto [b_begin, b_end] = rgs[1];

                auto a = seg.query(a_begin, a_end), b = seg.query(b_begin, b_end), c = seg.query(c_begin, c_end);
                auto ans = c.tot + b.lmax + a.rmax;
                
                debug {
                    std::cout << (std::string)a << std::endl;
                    std::cout << (std::string)b << std::endl;
                    std::cout << (std::string)c << std::endl;
                }

                std::cout << ans << endl;
            } else {
                auto a_begin = rgs[0].first, a_end = rgs[1].first;
                auto c_begin = rgs[1].first, c_end = rgs[0].second;
                auto b_begin = rgs[0].second, b_end = rgs[1].second;

                auto a = seg.query(a_begin, a_end), b = seg.query(b_begin, b_end), c = seg.query(c_begin, c_end);
                auto ans = std::max({
                    c.max,
                    c.lmax + a.rmax,
                    c.rmax + b.lmax,
                    c.tot + a.rmax + b.lmax
                });

                debug {
                    std::cout << (std::string)a << std::endl;
                    std::cout << (std::string)b << std::endl;
                    std::cout << (std::string)c << std::endl;
                }

                std::cout << ans << endl;
            }
            debug std::cout << std::flush;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);

    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    int T;  std::cin >> T;
    while (T --> 0) {
        Solution_1730635379514186::solve();
    }
    return 0;
}
