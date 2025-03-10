#include <chrono>
#include <iostream>
#include <random>
#include <thread>

namespace Solution {
    struct Probability {
        int nume = 0, deno = 10000;  // 分子，分母
    };
    namespace Random {
        std::mt19937 rng{std::random_device{}()};

        // 给定概率返回 true，否则返回 false
        auto choose(Probability p) -> bool {
            return static_cast<int>(rng() % p.deno) < p.nume;
        }
    }

    /** 角色活动祈愿
     * 考虑「捕获明光」机制
     * @link https://www.bilibili.com/video/BV13XBiYZErT
     */
    class CharacterWish {
        int fiveStarCount = 0;      // 已经多少次没有获得五星
        int fourStarCount = 0;      // 四星
        int captureCount = 1;       // 捕获明光的计数器，初始值为 1
        bool determinedUp = false;  // 下一次五星必为 UP
    public:
        // 当前是第 x 抽
        static auto fourStarProb(int x) -> Probability {
            if (x >= 10)  return {10000};  // 100%
            if (x == 9)  return {5500};  // 55%
            return {500};  // 5% 
        }
        static auto fiveStarProb(int x) -> Probability {
            if (x <= 73)  return {60};  // 0.6%
            return {std::min(60 + 600 * (x - 73), 10000)};  // 此后每一抽增加 6% 概率
        }
        struct Result {
            int star = 0;       // 星级
            bool up = false;    // 是否为 UP 角色
            int info = 0;       // 额外信息
        };
        // 祈愿一次
        auto operator() () -> Result {
            // 确定星级
            fiveStarCount++, fourStarCount++;
            auto prob5 = fiveStarProb(fiveStarCount);
            if (not Random::choose(prob5)) {  // 不是五星
                auto prob4 = fourStarProb(fourStarCount);
                if (not Random::choose(prob4))  return {3};  // 三星
                else  return {4};
            }

            // 五星
            fiveStarCount = 0;
            bool up = false;
            int info = 0;
            if (determinedUp)  up = true, info = 1;  // 消耗大保底
            else {
                // 捕获明光
                if (captureCount == 3)  up = true, captureCount = 1;
                else if (captureCount == 2 and Random::choose({5000}))  up = true, captureCount = 1;
                else {
                    if (Random::choose({5000}))  up = true, captureCount = std::max(0, captureCount - 1);
                    else  up = false, captureCount++;  // 小保底歪，下次为大保底
                }
            }
            determinedUp = not up;  // 仅当本次不是 up 的情况下有大保底
            return {5, up, info};
        }
    };

    auto solve() -> void {
        CharacterWish wish;

        int cnt = 0, up_cnt = 0, tot = 0, i = 0;
        while (true) {
            using namespace std::chrono_literals;

            auto [star, up, info] = wish();
            cnt++;
            if (star == 5) {
                if (up) {
                    if (info != 1)  up_cnt++, tot++;
                } else {
                    tot++;
                }
                cnt = 0;
            }
            i++;
            if (i % 100000 == 0)  std::cout << i << ": 小保底不歪" << static_cast<double>(up_cnt) / tot << std::endl;
        }
    }
}

int main() {
    Solution::solve();
    return 0;
}