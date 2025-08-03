/**
 * @link https://www.luogu.com.cn/problem/P2586
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/range.hpp"

#include "./libs/fixed_int.hpp"
using f32 = float;  using f64 = double;
using namespace lib;

namespace AntKiller {
    /**
     * 表示一个类型 T 是否能像 Func 一样调用
     */
    template <typename T, typename Func>
    concept CallableAs = std::convertible_to<T, std::function<Func>>;

    struct Vec2i {
        i32 x = 0, y = 0;

        auto operator+= (Vec2i const &other) -> Vec2i & {
            x += other.x, y += other.y;
            return *this;
        }
        auto operator+ (Vec2i const &other) const -> Vec2i {
            auto tmp = *this;
            return tmp += other;
        }
        auto operator-= (Vec2i const &other) -> Vec2i & {
            x -= other.x, y -= other.y;
            return *this;
        }
        auto operator- (Vec2i const &other) const -> Vec2i {
            auto tmp = *this;
            return tmp -= other;
        }
        auto operator* (Vec2i const &other) const -> i64 {
            return x * other.x + y * other.y;
        }
        auto sqrLen() const -> i64 {
            return x * x + y * y;
        }
        auto len() const -> f64 {
            return std::sqrt(sqrLen());
        }
        auto friend abs(Vec2i const &vec) -> f64 {
            return vec.len();
        }
        auto operator== (Vec2i const &other) -> bool { return x == other.x and y == other.y; }
    };

    struct Position {
        i32 x = -1, y = -1;

        // 位置是否有效
        auto valid() const -> bool {
            return x >= 0 and y >= 0;
        }
        
        auto operator<=> (Position const &) const = default;
        auto operator- (Position const &other) const -> Vec2i {
            return {x - other.x, y - other.y};
        }
    };
    class Ant {  // 蚂蚁类采用了精妙的设计
        i32         health = 0;         // 蚂蚁的生命值
        i32         maxHealth = 0;      // 蚂蚁的最大生命值
        Position    pos;                // 蚂蚁的当前位置
        bool        hasCake_ = false;   // 已经拿到了蛋糕
        Position    prevPos;            // 上一次的位置
        i32         level = 0;          // 等级
        i32         age = 1;            // 年龄
    public:
        Ant() {}
        /**
         * 在指定位置生成一只满血的蚂蚁
         */
        Ant(Position pos, i32 maxHealth, i32 level): health(maxHealth), maxHealth(maxHealth), pos(pos), level(level) {}

        auto getHealth() const -> i32 { return health; }
        auto getPos() const -> Position { return pos; }
        auto getPrevPos() const -> Position { return prevPos; }

        auto isDead() const -> bool { return getHealth() < 0; }
        auto hasCake() const -> bool { return hasCake_; }
        auto getLevel() const -> i32 { return level; }
        auto increaseAge() -> void { age++; }
        auto getAge() const -> i32 { return age; }
        /**
         * 受到一次伤害
         */
        auto damaged(i32 amount) -> void {
            health -= amount;
        }
        /**
         * 给予蛋糕
         */
        auto giveCake() -> void {
            hasCake_ = true;
        }
        /**
         * 治疗，同时钳制在生命值上限以下
         */
        auto heal(i32 amount) -> void {
            health = std::min(health + amount, maxHealth);
        }

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
            std::array<i32, 4> dx{  0, +1,  0, -1 };
            std::array<i32, 4> dy{ +1,  0, -1,  0 };

            auto dirToPos = lam(i, (Position{pos.x + dx[i], pos.y + dy[i]}));
            auto target_dirs_ = range(4)
                | views::filter(lam(i, isReachable(dirToPos(i))))
                | views::filter(lam(i, dirToPos(i) != prevPos));
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
        Tower(Position pos, i32 attackRange, i32 attackDamage): 
            pos(pos), attackRange(attackRange), attackDamage(attackDamage) {}

        auto getPos() const -> Position { return pos; }
        auto getAttackRange() const -> i32 { return attackRange; }
        auto getAttackDamage() const -> i32 { return attackDamage; }

        /**
         * 攻击
         * 从给定的蚁群中选择攻击目标，并攻击所有满足要求的蚂蚁。
         */
        auto attack(auto &ants) const -> void {
            auto target = [&]() -> Ant {
                auto it_has_cake = ranges::find_if(ants, lam(x, x.hasCake()));
                if (it_has_cake != ants.end())  return *it_has_cake;
                return ranges::min(
                    ants,
                    std::less{},
                    lam(x, (x.getPos() - getPos()).sqrLen())
                );
            }();

            // 炮塔特性：所有在这一条路径上的蚂蚁都会减血
            for (Ant &ant: ants) {
                auto O = getPos();
                auto A = ant.getPos();
                auto B = target.getPos();
                auto a = A - O, b = B - O;
                // 蚂蚁所在的圆（r = 0.5）和这条线段有交点就能打到
                // 事实上，由于几个点都是整点，只需要判断点与直线的距离，以及两个距离的关系
                
                // cos theta = Vec a * Vec b / |a| * |b|
                auto point_to_line = [&]() -> f64 {
                    auto cos_theta = (a * b) / (abs(a) * abs(b));
                    auto sin_theta = sqrt(1 - cos_theta * cos_theta);
                    return abs(a) * sin_theta;
                }();

                auto reachable = point_to_line <= 0.5 and abs(b) - abs(a) >= -0.5 and abs(a) <= getAttackRange();
                if (reachable) {
                    debug  std::cout << std::format("Reachable O = ({}, {}), A = ({}, {}), B = ({}, {})", O.x, O.y, A.x, A.y, B.x, B.y) << std::endl;
                    ant.damaged(attackDamage);
                }
            }
        }
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
        std::vector<Tower>  towers;             // 所有炮塔
        i32                 nextAntId = 0;      // 下一个蚂蚁的 ID
        std::map<i32, Ant>  ants;               // id -> 存活的蚂蚁
        std::set<Position>  occupied;           // 被占用的位置
        bool                good = true;        // 游戏还在继续
        i32                 time = 0;           // 当前游戏时间
    public:
        World() {}
        World(i32 height, i32 width): height(height), width(width), pheromone(height + 1, std::vector<i32>(width + 1, 0)) {}

        auto getHeight() const -> i32 { return height; }
        auto getWidth() const -> i32 { return width; }
        auto isCakeLost() const -> bool { return cakeLost; }
        auto isGood() const -> bool { return good; }
        auto getTime() const -> i32 { return time; }

        auto addTower(Tower const &tower) -> void {
            towers.push_back(tower);
            occupied.insert(tower.getPos());
        }

        /**
         * 尝试生成蚂蚁
         */
        auto summonAnt() -> void {
            i32 constexpr antLimit = 6;
            if (ants.size() >= antLimit)  return;

            Position constexpr spawn{0, 0};  // 蚂蚁出生点
            auto level = nextAntId / 6 + 1;
            auto maxHealth = static_cast<i32>(4 * std::pow(1.1, level));
            ants.emplace(nextAntId++, Ant{spawn, maxHealth, level});
        }
        /**
         * 计算蚂蚁新留下的信息素
         */
        auto updatePheromone() -> void {
            for (auto &[id, ant]: ants) {
                auto pos = ant.getPos();
                assert(pos.valid());
                if (ant.hasCake()) {
                    pheromone[pos.x][pos.y] += 5;
                } else {
                    pheromone[pos.x][pos.y] += 2;
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
                return pheromone[pos.x][pos.y];
            };
            auto isReachable = [&](Position pos) -> bool {
                // 没有其他的蚂蚁
                return pos.valid() and pos.x <= height and pos.y <= width and not occupied.contains(pos);
            };
            for (auto &[id, ant]: ants) {
                occupied.erase(ant.getPos());
                ant.move(Ant::MoveContext{takeCake, getPheromone, isReachable});
                occupied.insert(ant.getPos());
            }

            // 蚂蚁获得蛋糕
            for (auto &[id, ant]: ants) {
                if (ant.getPos() == Position{height, width} and not isCakeLost()) {
                    ant.giveCake();
                    cakeLost = true;
                    break;
                }
            }
        }
        /**
         * 炮塔进行攻击
         */
        auto performAttack() -> void {
            for (auto &tower: towers) {
                auto values = ants | views::values;
                tower.attack(values);
            }
            // 进行收尾
            // 移除死亡的蚂蚁
            for (auto it = ants.begin(); it != ants.end(); ) {
                if (it->second.isDead()) {
                    // 归还蛋糕
                    if (it->second.hasCake()) {
                        cakeLost = false;
                    }
                    it = ants.erase(it);
                } else {
                    ++it;
                }
            }
        }

        auto isGameOver() const -> bool {
            // 如果有一个蚂蚁在 (0, 0) 处并且拿着蛋糕，游戏结束
            return ranges::any_of(
                ants | views::values,
                [](auto &ant) { return ant.getPos() == Position{0, 0} and ant.hasCake(); }
            );
        }

        /**
         * 使信息素衰减
         */
        auto fadePheromone() -> void {
            for (auto &row: pheromone) {
                for (auto &val: row) {
                    val = std::max(val - 1, 0);
                }
            }
        }

        /**
         * 游戏进行一刻。
         * @returns 是否结束游戏
         */
        auto tick() -> bool {
            time++;
            assert(isGood());
            summonAnt();
            updatePheromone();
            moveAnts();
            performAttack();
            if (isGameOver())  return good = false, true;
            fadePheromone();
            for (auto &[id, ant]: ants) {
                ant.increaseAge();
            }
            return false;
        }

        /**
         * 总结当前局势
         */
        auto summary() const -> std::string {
            // if (not isGood())  return ;
            std::stringstream ss;
            
            ss << (isGood()? "The game is going on": std::format("Game over after {} seconds", getTime())) << endl;
            auto ant_cnt = ants.size();
            ss << ant_cnt << endl;

            for (auto &[id, ant]: ants) {
                auto age = ant.getAge() - 1;
                auto level = ant.getLevel();
                ss << age << " " << level << " " << ant.getHealth() << " " << ant.getPos().x << " " << ant.getPos().y << endl;
            }
            return ss.str();
        }

        /**
         * 输出详细的调试信息
         */
        auto detail() const -> std::string {
            std::stringstream ss;

            // 打印地图
            // 数字表示信息素浓度
            // a-z 表示编号为 id % 26 的蚂蚁
            // x 表示炮塔
            std::vector antAt(getHeight() + 1, std::vector<char>(getWidth() + 1, '\0'));
            std::vector towerAt(getHeight() + 1, std::vector<char>(getWidth() + 1, '\0'));
            ss << "Ants: " << endl;
            for (auto const &[id, ant]: ants) {
                auto pos = ant.getPos();
                ss << char('a' + id % 26) << ": ";
                if (pos.valid()) {
                    ss << "pos = " << pos.x << ' ' << pos.y << ", health = " << ant.getHealth() << ", age = " << ant.getAge() << ", cake = " << ant.hasCake() << endl;
                    antAt[pos.x][pos.y] = 'a' + id % 26;
                }
            }
            for (auto const &tower: towers) {
                auto pos = tower.getPos();
                if (pos.valid()) {
                    towerAt[pos.x][pos.y] = 'x';
                }
            }
            ss << "Map:" << endl;
            for (auto i = 0; i < height + 1; i++) {
                for (auto j = 0; j < width + 1; j++) {
                    if (antAt[i][j])  ss << antAt[i][j];
                    else if (towerAt[i][j])  ss << "x";
                    else {
                        auto p = pheromone[i][j];
                        if (p < 10)  ss << p;
                        else  ss << "+";
                    }
                }
                ss << endl;
            }
            return ss.str();
        }
    };
    void solve() {
        // test
        i32 height, width;
        std::cin >> height >> width;
        World world{height, width};

        i32 tower_cnt, tower_damage, tower_range;
        std::cin >> tower_cnt >> tower_damage >> tower_range;
        for (auto c = tower_cnt; c --> 0; ) {
            i32 x, y;
            std::cin >> x >> y;
            world.addTower(Tower{Position{x, y}, tower_range, tower_damage});
        }

        i32 time;  std::cin >> time;
        for (auto t = time; t --> 0; ) {
            debug  std::cout << world.summary();
            debug  std::cout << world.detail();
            if (world.tick()) {
                break;
            }
        }
        std::cout << world.summary();
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    AntKiller::solve();
    return 0;
}
