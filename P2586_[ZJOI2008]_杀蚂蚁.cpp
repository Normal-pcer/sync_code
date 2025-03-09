/**
 * @link https://www.luogu.com.cn/problem/P2586
 */
#ifndef ONLINE_JUDGE
#define GNU_DEBUG
#define _GLIBCXX_DEBUG 1
#define _GLIBCXX_DEBUG_PEDANTIC 1
#define _GLIBCXX_SANITIZE_VECTOR 1
#endif
#include <bits/stdc++.h>
bool DEBUG_MODE = false;
#define debug if(DEBUG_MODE)
template <typename T> inline auto chkMax(T &base, const T &cmp) -> T & { return (base = std::max(base, cmp)); }
template <typename T> inline auto chkMin(T &base, const T &cmp) -> T & { return (base = std::min(base, cmp)); }
#define never if constexpr(0)
const int inf = 0x3f3f3f3f;  const long long infLL = 0x3f3f3f3f3f3f3f3fLL; using ll = long long; using ull = unsigned long long;
const char endl = '\n';

#define __lambda_1(expr) [&](){return expr;}
#define __lambda_2(a, expr) [&](auto a){return expr;}
#define __lambda_3(a, b, expr) [&](auto a, auto b){return expr;}
#define __lambda_4(a, b, c, expr) [&](auto a, auto b, auto c){return expr;}
#define __lambda_overload(a, b, c, d, e, args...) __lambda_##e
#define lambda(...) __lambda_overload(__VA_ARGS__, 4, 3, 2, 1)(__VA_ARGS__)
#define lam lambda
namespace lib{
#if __cplusplus > 201703LL
namespace ranges = std::ranges;
namespace views = std::views;
#endif
}
#include "libs/range.hpp"
using i16 = int16_t; using i32 = int32_t; using i64 = int64_t;
using u16 = uint16_t; using u32 = uint32_t; using u64 = uint64_t; using uz = size_t;

using namespace lib;

namespace AntKiller {
    /**
     * 表示一个类型 T 是否能像 Func 一样调用
     */
    template <typename T, typename Func>
    concept CallableAs = std::convertible_to<T, std::function<Func>>;

    struct Position {
        i32 x = -1, y = -1;

        // 位置是否有效
        auto valid() const -> bool {
            return x >= 0 and y >= 0;
        }
        
        auto operator<=> (Position const &) const = default;
    };
    class Ant {  // 蚂蚁类采用了精妙的设计
        i32         health = 0;         // 蚂蚁的生命值
        Position    pos;                // 蚂蚁的当前位置
        bool        hasCake_ = false;   // 已经拿到了蛋糕
        i32         age = 0;            // 蚂蚁的年龄
        Position    prevPos;            // 上一次的位置
    public:
        Ant() {}

        auto getHealth() const -> i32 { return health; }
        auto getPos() const -> Position { return pos; }
        auto getAge() const -> i32 { return age; }
        auto getPrevPos() const -> Position { return prevPos; }

        auto isDead() const -> bool { return getHealth() < 0; }
        auto hasCake() const -> bool { return hasCake_; }


        /**
         * 蚂蚁移动必须的上下文
         */
        template <typename F1, typename F2, typename F3>
        struct MoveContext {
            static_assert(CallableAs<F1, bool(Position)>);
            F1 takeCake;            // 返回一个位置是否有蛋糕，如果有就去掉并返回 true

            static_assert(CallableAs<F2, i32(Position)>);
            F2 getPheromone;        // 已经有的信息素浓度

            static_assert(CallableAs<F3, bool(Position)>);
            F3 isReachable;         // 判断蚂蚁是否可以将这个位置列入可选项
        };

        /**
         * 决断一个最佳移动位置并进行移动，然后在新位置留下信息素
         * 
         * @param ctx 移动所需的上下文，需要实现对应的函数
         */
        template <typename ...Ts>
        auto move(MoveContext<Ts...> ctx) -> void {
            assert(not isDead());

            auto [takeCake, getPheromone, isReachable] = ctx;

            // 所有可达的点中，选择信息素浓度最大的
            std::array<i32, 4> dx{ +1,  0, -1,  0 };
            std::array<i32, 4> dy{  0, +1,  0, -1 };

            auto pos = getPos();
            auto dirToPos = lam(i, (Position{pos.x + dx[i], pos.y + dy[i]}));
            auto target_dirs_ = range(4)
                | views::filter(lam(i, isReachable(dirToPos(i))));
            std::vector target_dirs(target_dirs_.begin(), target_dirs_.end());
            auto best_it = ranges::max_element(
                target_dirs,
                std::less{},
                lam(i, getPheromone(dirToPos(i)))
            );

            // 如果年龄是 5 的倍数需要选择上一个方向
            if (age % 5 == 0 and not target_dirs.empty()) {
                if (best_it != target_dirs.end()) {
                    if (best_it == target_dirs.begin())  best_it = target_dirs.end();
                    --best_it;
                }
            }

            // 最好的移动目标
            auto best_pos = best_it == target_dirs.end() ? pos : dirToPos(*best_it);
            // 更新位置
            prevPos = pos;
            pos = best_pos;
            // 尝试拿蛋糕
            if (takeCake(best_pos)) {
                hasCake_ = true;
            }
        }

    };
    class Tower {
        Position        pos;                // 塔的位置
        i32             attackRange = 0;    // 攻击范围
        i32             attackDamage = 0;   // 攻击伤害
    public:
        Tower() {}

        auto getPos() const -> Position { return pos; }
        auto getAttackRange() const -> i32 { return attackRange; }
        auto getAttackDamage() const -> i32 { return attackDamage;}
    };
    /**
     * 世界。
     * 维护蚂蚁和炮塔的信息。
     */
    class World {
        template <typename T>
        using Matrix = std::vector<std::vector<T>>;

        i32                 height = 0;         // 地图高度
        i32                 width = 0;          // 地图宽度
        Matrix<i32>         pheromone;          // 每个点上的信息素浓度
        bool                cakeLost = false;   // 蛋糕是否被抢走
        std::set<Tower>     towers;             // 所有炮塔
        i32                 nextAntId = 0;      // 下一个蚂蚁的 ID
        std::map<i32, Ant>  ants;               // id -> 存活的蚂蚁
    public:
        World() {}

        auto getHeight() const -> i32 { return height; }
        auto getWidth() const -> i32 { return width; }
        auto isCakeLost() const -> bool { return cakeLost; }


        /**
         * 计算蚂蚁新留下的信息素
         */
        auto updatePheromone() -> void {
            for (auto &[id, ant]: ants) {
                auto pos = ant.getPos();
                assert(pos.valid());
                if (ant.hasCake()) {
                    pheromone[pos.y][pos.x] += 5;
                } else {
                    pheromone[pos.y][pos.x] += 2;
                }
            }
        }

        /**
         * 所有的蚂蚁按照既定策略移动
         */
        auto moveAnts() -> void {
            auto takeCake = [&](Position pos) -> bool {
                if (not cakeLost and pos == Position{getHeight(), getWidth()}) {
                    cakeLost = true;
                    return true;
                }
                return false;
            };
            auto getPheromone = [&](Position pos) -> i32 {
                if (not pos.valid())  assert(false);
                return pheromone[pos.y][pos.x];
            };
            auto isReachable = [&](Position pos) -> bool {
                // 没有其他的蚂蚁
                return ranges::none_of(ants, lam(x, x.second.getPos() == pos));
            };
            for (auto &[id, ant]: ants) {
                ant.move(Ant::MoveContext{takeCake, getPheromone, isReachable});
            }
        }
        /**
         * 炮塔进行攻击
         */
        auto performAttack() -> void {
            // TODO
        }
    };
    void solve() {
        // test
        World world;
        world.moveAnts();
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    AntKiller::solve();
    return 0;
}
