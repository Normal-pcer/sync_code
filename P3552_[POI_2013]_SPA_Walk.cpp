/**
 * @link https://www.luogu.com.cn/problem/P3552
 */
#include "./lib_v7.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

namespace Solution_9439060302064223 {
    template <typename T, typename U, typename Func, typename Comp = std::less<>>
    auto lowerBoundValue(T begin, T end, U val, Func &&f, Comp &&cmp = {}) -> T {
        while (end - begin >= 1) {
            auto mid = std::midpoint(begin, end);
            if (cmp(f(mid), val)) begin = mid + 1;
            else end = mid;
        }
        return begin;
    }
    i32 constexpr inf = 0x3f3f3f3f;

    auto solve() -> void {
        i32 n, m;
        std::cin >> n >> m;

        struct TimePoint {
            i32 count;  // 此时的人数
            i32 startCount;  // 多少个人以这个点开始
            i32 endCount;  // 多少个人以这个点结束
        };
        struct Message {
            i32 self;
            i32 time;
            i32 totalCount;  // 包括自己的人数
        };
        struct Person {
            i32 start = inf;  // 开始上班
            i32 end = -inf;  // 停止上班
        };

        std::vector<Message> messages(m);
        for (auto &msg: messages) {
            std::cin >> msg.time >> msg.self >> msg.totalCount;
            msg.self--, msg.totalCount++;
        }

        auto check = [&](i32 messageCount) -> bool {
            auto selected = messages | views::take(messageCount);

            std::vector<TimePoint> time(m + 1);  // 记录每个时间点的信息
            std::vector<Person> persons(n);  // 每个人的信息
            std::vector<std::uint8_t> hasPerson(n);  // 是否遍历过某个人
            for (auto const &msg: selected) {
                // 发生矛盾
                if (time[msg.time].count != 0 and time[msg.time].count != msg.totalCount) return false;
                time[msg.time].count = msg.totalCount;
                chkMin(persons[msg.self].start, msg.time);
                chkMax(persons[msg.self].end, msg.time);
                hasPerson[msg.self] = true;
            }

            // 统计以某个时间为端点的人数
            for (i32 p = 0; p < n; p++) {
                if (hasPerson[p]) {
                    time[persons[p].start].startCount++;
                    time[persons[p].end].endCount++;
                }
            }

            i32 least = 0;  // 已经开始，一定没有结束；这些人必须参与
            i32 late = 0;  // 已经晚点下班的人；可以用来减少人数
            i32 early = 0;  // 可以提前上班的人；可以用来增加人数
            i32 used = 0;  // 多少个人已经被分配工作了

            // 枚举时间
            for (i32 t = 0; t <= m; t++) {
                if (time[t].count == 0) continue;  // 没有限制的时间
                // 有人开始工作（转化为 least）
                least += time[t].startCount;

                if (least > time[t].count) return false;
                // 优先让人提前上班
                // 如果不行，就让自由人来
                early -= time[t].startCount, time[t].startCount = 0;
                if (early < 0) used += -early, early = 0;

                // 如果现在的总人数不足，找几个人提前上班
                if (auto dis = time[t].count - (least + late + early); dis > 0) {
                    early += dis, used += dis;
                }

                // 如果现在总人数过多
                if (auto dis = (least + late + early) - time[t].count; dis > 0) {
                    // 优先减少加班的人，否则减少提前上班的人
                    late -= dis;
                    if (late < 0) early -= -late, late = 0;
                }

                // 有人结束工作
                // 先继续让他加班
                least -= time[t].endCount;
                late += time[t].endCount;

                if (used > n) return false;
            }
            return true;
        };

        // 寻找第一个不满足条件的
        auto ans = lowerBoundValue(0, m + 1, false, check, std::greater{});
        assert(ans != 0);
        std::cout << ans - 1 << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    i32 t;
    std::cin >> t;

    for (auto _ = t; _ --> 0; ) {
        Solution_9439060302064223::solve();
    }
    return 0;
}
