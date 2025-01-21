/**
 * @link https://www.luogu.com.cn/problem/P4944
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_3854369097708222 {
    struct Vec2d {
        int x, y;  // 对应读入的行号，列号

        auto operator+= (Vec2d const &other) -> Vec2d & { return x += other.x, y += other.y, *this; }
        auto operator-= (Vec2d const &other) -> Vec2d & { return x -= other.x, y -= other.y, *this; }
        auto operator+ (Vec2d const &other) -> Vec2d { auto res = *this; return res += other; }
        auto operator- (Vec2d const &other) -> Vec2d { auto res = *this; return res -= other; }
    };
    class Snake;
    struct Block {
        enum Type {
            Empty, Occupied, Food
        } type;
    };
    struct World {
        int N, M;  // x 和 y 的最大值
        int K;
        std::vector<std::vector<Block>> map;
        std::vector<Snake> snakes;

        auto check(Vec2d pos) -> bool { return 0 <= pos.x and pos.x < N and 0 <= pos.y and pos.y < M; }
    };
    
    struct Snake {
        std::shared_ptr<World> world = nullptr;
        std::deque<Vec2d> components;  // 保证 front 为蛇头
        bool _isDead = false;

        auto die() -> void;  // 去世
        auto size() const -> int;  // 蛇的长度
        auto move(Vec2d) -> void;  // 向目标方向移动
        auto isDead() const -> bool;
        auto popBackComponent() -> void;
        auto popFrontComponent() -> void;
        auto pushBackComponent(Vec2d) -> void;
        auto pushFrontComponent(Vec2d) -> void;
    };
    auto Snake::size() const -> int { return components.size(); }
    auto Snake::isDead() const -> bool { return _isDead; }
    auto Snake::die() -> void {
        for (auto [x, y]: components) {
            world->map[x][y] = {Block::Food};
        }
        components.clear(), _isDead = true;
    }
    auto Snake::move(Vec2d direction) -> void {
        assert(not isDead());
        // 检查蛇头的目标位置
        auto head_target = components.front() + direction;
        if (not world->check(head_target)) {
            // 移动到地图外
            die();
            return;
        }
        auto head_target_block = world->map[head_target.x][head_target.y];

        if (head_target_block.type == Block::Empty) {
            // 移动到空地
            // 等价于移除尾部一段，头部新增一段
            pushFrontComponent(head_target);
            popBackComponent();
        } else if (head_target_block.type == Block::Food) {
            // 吃掉食物
            pushFrontComponent(head_target);
        } else if (head_target_block.type == Block::Occupied) {
            // 去世
            die();
        }
    }
    auto Snake::popBackComponent() -> void {
        auto pos = components.back();
        components.pop_back();
        world->map[pos.x][pos.y] = {Block::Empty};
    }
    auto Snake::popFrontComponent() -> void {
        auto pos = components.front();
        components.pop_front();
        world->map[pos.x][pos.y] = {Block::Empty};
    }
    auto Snake::pushBackComponent(Vec2d pos) -> void {
        components.push_back(pos);
        world->map[pos.x][pos.y] = {Block::Occupied};
    }
    auto Snake::pushFrontComponent(Vec2d pos) -> void {
        components.push_front(pos);
        world->map[pos.x][pos.y] = {Block::Occupied};
    }
    struct WorldFactory {
        static auto fromIstream(std::istream &st) -> std::shared_ptr<World> {
            auto res = std::make_shared<World>();
            int N, M, K;  st >> N >> M >> K;
            res->N = N, res->M = M, res->K = K;
            std::vector<std::string> lines(N);
            std::vector map(N, std::vector<Block>(M));
            // 读入字符形式的地图
            for (auto i: range(N)) {
                lines[i].resize(M);
                for (auto j: range(M)) {
                    std::cin >> lines[i][j];
                    if (lines[i][j] == '&')  map[i][j] = {Block::Food};
                    else if (lines[i][j] == '.')  map[i][j] = {Block::Empty};
                    else  map[i][j] = {Block::Occupied};
                }
            }

            std::vector vis(N, std::vector<char>(M));
            auto walk = [&](Vec2d pos) -> std::deque<Vec2d> {
                std::deque<Vec2d> q{pos};
                std::deque<Vec2d> ans;
                while (not q.empty()) {
                    auto [x, y] = q.front();  q.pop_front();
                    if (vis[x][y])  continue;
                    vis[x][y] = true;
                    ans.push_back({x, y});
                    
                    for (auto d: {0x12, 0x10, 0x21, 0x01}) {
                        auto dx = (d >> 4) - 1, dy = (d & 15) - 1;
                        if (x + dx < 0 or x + dx >= N or y + dy < 0 or y + dy >= M)  continue;
                        if (lines[x + dx][y + dy] == '.')  continue;
                        if (lines[x + dx][y + dy] == '&')  continue;
                        if (lines[x + dx][y + dy] == '@')  continue;
                        q.push_back({x + dx, y + dy});
                    }
                }
                return ans;
            };

            std::vector<Snake> snakes;
            for (auto i: range(N)) {
                for (auto j: range(M)) {
                    if (lines[i][j] == '@' and not vis[i][j]) {
                        snakes.push_back(Snake{});
                        snakes.back().world = res;
                        snakes.back().components = walk({i, j});
                    }
                }
            }
            res->snakes = std::move(snakes);
            res->map = std::move(map);
            return res;
        }
    };
    void solve() {
        auto world = WorldFactory::fromIstream(std::cin);
        auto cnt = world->snakes.size();
        std::vector<std::string> ops(cnt);
        for (auto &op: ops) {
            op.resize(world->K);
            for (auto &ch: op) {
                std::cin >> ch;
            }
        }
        // return;

        for (auto t: range(world->K)) {
            for (auto i: range(cnt)) {
                auto &snake = world->snakes[i];
                if (snake.isDead())  continue;
                snake.move([&](char ch) -> Vec2d {
                    switch (ch) {
                    case 'W':  return {-1, 0};
                    case 'A':  return {0, -1};
                    case 'S':  return {+1, 0};
                    case 'D':  return {0, +1};
                    default:  assert(false), __builtin_unreachable();
                    }
                }(ops[i][t]));
                debug {
                    std::cout << "-----" << t << std::endl;
                    for (auto j: range(cnt)) {
                        for (auto k: world->snakes[j].components) {
                            std::cout << std::format("({}, {}) ", k.x, k.y);
                        }
                        std::cout << std::endl;
                    }
                    for (auto &line: world->map) {
                        for (auto &item: line) {
                            std::cout << [](Block::Type t) -> char {
                                switch (t) {
                                    case Block::Occupied:  return '#';
                                    case Block::Empty:  return '.';
                                    case Block::Food:  return '&';
                                    default:  assert(false), __builtin_unreachable();
                                }
                            }(item.type);
                        }
                        std::cout << std::endl;
                    }
                }
            }
        }

        std::vector<std::pair<int, int>> finally;
        for (auto &x: world->snakes) {
            finally.push_back({x.components.size(), finally.size()});
        }
        ranges::sort(finally, lam(x, y, x.first == y.first? x.second < y.second: x.first > y.first));
        auto cnt_food = 0;
        for (auto &line: world->map) {
            for (auto &item: line) {
                if (item.type == Block::Food) {
                    cnt_food++;
                }
            }
        }

        for (auto [x, y]: finally) {
            std::cout << x << " " << y + 1 << endl;
        }
        std::cout << cnt_food << endl;
    }
}

int main(int dyy_forever, char const *lmy_forever[]) {
    DEBUG_MODE = (dyy_forever-1) and not strcmp("-d", lmy_forever[1]);
    Solution_3854369097708222::solve();
    return 0;
}
