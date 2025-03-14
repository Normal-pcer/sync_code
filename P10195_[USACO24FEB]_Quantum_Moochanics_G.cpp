/**
 * @link https://www.luogu.com.cn/problem/P10195
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"
using namespace lib;

/**
 * 观察一个粒子的运动方式
 * 
 * 3 -> 4 -> 2 -> 5 -> 1
 * 7 -> 6 -> 8 -> 5 -> 9
 * 
 * 两个粒子想要相撞，每一个“周期”它们的最近距离都会减少 2。
 * 即可求出两个粒子相撞的最短时间。
 * 
 * 由于一开始是正负交替的，所以最先碰到的一定是相邻的两个粒子。当有两个粒子相撞之后，显然还会是这种情况持续下去。
 * 记录每对相邻粒子的相撞时间，选择最短的一个。
 * 接下来又会有新的元素变为相邻。
 * 可以使用优先队列维护最小的相撞时间。
 */
namespace Solution_7448195981164406 {
    void solve() {
        i32 N;  std::cin >> N;

        struct Particle {
            i64 init_pos = 0;       // 初始位置
            i32 speed = 0;          // 每秒移动距离
            bool color = false;     // 是否为反物质
            i32 index = 0;       
            auto operator<=> (Particle const &) const = default; 
            auto string() const -> std::string {
                return std::format("Particle({}, {}, {}, {})", init_pos, speed, color, index);
            }
        };
        std::list<Particle> particles(N);  // 使用链表维护，删除复杂度低
        for (auto &x: particles)  std::cin >> x.init_pos;
        for (auto &x: particles)  std::cin >> x.speed;
        for (i32 i = 0; auto &x: particles)  x.color = i & 1, x.index = i, i++;

        // 获取两个粒子的相遇时间
        auto get_meet_time = [&](Particle const &lhs, Particle const &rhs) -> i64 {
            assert(lhs.color != rhs.color);
            
            auto dis = rhs.init_pos - lhs.init_pos;
            auto decrease_per_term = rhs.speed + lhs.speed;
            auto term_count = dis / decrease_per_term;  // 总共需要多少轮
            auto remains = dis % decrease_per_term;  // 剩余的距离
            if (remains != 0)  term_count++;
            if (rhs.color) {
                // 左侧是正，右侧是负
                return 2 * term_count - 1;
            } else {
                return 2 * term_count;
            }
        };

        // 需要支持移除非堆顶元素，使用 set 维护。
        auto it_compare_three_way = [&](auto const &lhs, auto const &rhs) -> std::strong_ordering {
            if (*lhs == *particles.rbegin())  return std::strong_ordering::greater;
            if (*rhs == *particles.rbegin())  return std::strong_ordering::less;
            if (auto res = get_meet_time(*lhs, *std::next(lhs)) <=> get_meet_time(*rhs, *std::next(rhs)); res != 0) {
                return res;
            }
            return *lhs <=> *rhs;
        };

        auto it_less = lam(const &x, const &y, it_compare_three_way(x, y) < 0);
        std::set<std::list<Particle>::iterator, decltype(it_less)> s(it_less);

        ranges::copy(
            views::iota(particles.begin(), particles.end()),
            std::inserter(s, s.end())
        );
        std::vector<i64> ans(N);
        while (not s.empty()) {
            // assert(ranges::all_of(views::iota(particles.begin(), particles.end()), lam(x, s.contains(x))));
            auto x_it = *s.begin();
            auto y_it = std::next(x_it);
            assert(y_it != particles.end());
            assert(s.contains(y_it));

            auto cur = get_meet_time(*x_it, *y_it);
            ans[x_it->index] = ans[y_it->index] = cur;
            s.erase(s.find(x_it));
            s.erase(s.find(y_it));
            // 重新更新左侧的位置
            auto left = x_it == particles.begin()? particles.end(): std::prev(x_it);
            if (left != particles.end())  s.erase(s.find(left));
            particles.erase(x_it);
            particles.erase(y_it);
            if (left != particles.end())  s.insert(left);
            assert(s.size() == particles.size());

            never {
                if (s.size() == 1348) {
                    std::fstream fs("1.out", std::ios::out);
                    decltype(s) s2(s.key_comp());
                    ranges::copy(
                        views::iota(particles.begin(), particles.end()),
                        std::inserter(s2, s2.end())
                    );
                    for (auto x: s2) {
                        fs << "particle " << x->string() << std::endl;
                    }
                    for (auto x: s) {
                        fs << "set " << x->string() << std::endl;
                    }
                    fs.close();
                }
            }
        }

        for (auto x: ans)  std::cout << x << " ";
        std::cout << endl;
    }
}

int main(int dyy, char const *cyy[]) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
    DEBUG_MODE = (dyy - 1) and not strcmp("-d", cyy[1]);

    i32 T;  std::cin >> T;
    while (T --> 0) {
        Solution_7448195981164406::solve();
    }
    return 0;
}
