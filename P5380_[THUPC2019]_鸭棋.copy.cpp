/**
 * @link https://www.luogu.com.cn/problem/P5380
 */

#include <bits/stdc++.h>
#define from(i,b,e) for(int i=(b);i<=(e);i++)
#define lambda(arg, expr) [&](auto arg){return expr;}

using std::string, std::cin, std::cout;

template <typename T> struct vector: public std::vector<T> {
    template <class Func>
    bool some(const Func&& f) {
        for (auto& i: *this) {
            if (f(i))  return true;
        }
        return false;
    }
};

const int _X = 10;
const int _Y = 9;

struct Vec2d {
    int x, y;
    Vec2d operator+(const Vec2d &other) const { return {x+other.x, y+other.y}; } // 加
    Vec2d operator-(const Vec2d &other) const { return {x-other.x, y-other.y}; } // 减
    Vec2d operator*(const int other) const { return {x*other, y*other}; } // 数乘
    Vec2d operator/(const int other) const { return {x/other, y/other}; } // 乘以 1/other
    int operator*(const Vec2d &other) const { return x*other.x + y*other.y; } // 数量积
    int cross(const Vec2d &other) const { return x*other.y - y*other.x; } // 叉乘的模长
    bool operator==(const Vec2d &other) const { return x==other.x and y==other.y; } // 相等
    bool operator!=(const Vec2d &other) const { return x!=other.x or y!=other.y; } // 不等

    int sqrDistance(const Vec2d &other) const { // 两点间欧几里得距离的平方
        return (x-other.x)*(x-other.x)+(y-other.y)*(y-other.y); 
    }
    int manhattan(const Vec2d &other) const { return abs(x-other.x)+abs(y-other.y); } // 曼哈顿距离
    int chebyshev(const Vec2d &other) const { return std::max(abs(x-other.x), abs(y-other.y)); } // 切比雪夫距离

    int sqrAbs() const { return x*x+y*y; } // 模长的平方
};

enum Team {
    Red,
    Blue,
};

struct Piece;  // 预先声明，解决循环引用问题
vector<Piece> pieces;

struct Piece {
    string  name;   // 棋子类型，如 horse, elephant
    Vec2d   pos;    // 棋子位置
    Team    team;   // 棋子阵营

    // 检查是否可以到达目标位置，但是只对位置进行判断
    bool reachable_pos(Vec2d dest) {
        if (name == "captain") {
            return pos.sqrDistance(dest) == 1;
        } else if (name == "guard") {
            return pos.manhattan(dest) == 2 and pos.chebyshev(dest) == 1;
        } else if (name == "elephant") {
            return pos.manhattan(dest) == 4 and pos.chebyshev(dest) == 2 and 
                not pieces.some(lambda(p, (p.pos-pos)*2==(dest-pos))); 
        } else if (name == "horse") {
            return pos.sqrDistance(dest) == 5 and 
                not pieces.some(lambda(p, 
                    p.pos.sqrDistance(pos) == 1 and (p.pos-pos)*(dest-pos)==2));
        } else if (name == "car") {
            auto a = dest-pos;
            return pos.chebyshev(dest) == pos.manhattan(dest) and not pieces.some([&](auto p) {
                auto b = p.pos-pos;
                return b.cross(a)==0 and b.sqrAbs()<a.sqrAbs() and b*a>0;
            });
        } else if (name == "soldier") {
            return pos.chebyshev(dest) == 1;
        } else {  // duck
            auto a = dest-pos;
            return pos.sqrDistance(dest) == 13 and not pieces.some([&](auto p) {
                auto b = p.pos-pos;
                return (a*b==8 and b.sqrAbs()==5) or (a*b==3 and b.sqrAbs()==1);
            });
        }
    }

    bool reachable(Vec2d dest) {  // 能否到达目标地点
        return 0<=dest.x and dest.x<_X and
        0<=dest.y and dest.y<_Y and
        reachable_pos(dest) and 
        not pieces.some(lambda(p, p.pos==dest and p.team==team));
    }
};

namespace Solution {
    int Q;
    void init() {
        std::ios::sync_with_stdio(false); cin.tie(0); cout.tie(0);
        // 添加一个子
        #define add(name,px,py,team) pieces.push_back(Piece{name, {px, py}, Team::team});
        // 添加四个中心对称的子
        #define four(name,dx,dy) add(name,4-dx,4+dy,Red) add(name,4-dx,4-dy,Red) \
            add(name,5+dx,4-dy,Blue) add(name,5+dx,4+dy,Blue)
        // 添加底线上的一个子
        #define bottom(name,dy) four(name,4,dy);
        add("captain",0,4,Red) add("captain",9,4,Blue)
        bottom("car",4)bottom("horse",3)bottom("elephant",2)bottom("guard",1)
        four("duck",2,4)
        four("soldier",1,4)four("soldier",1,2)add("soldier",3,4,Red)add("soldier",6,4,Blue)
        cin >> Q;
    }

    void solve() {
        init();
        // 双方的王
        Piece& kingRed = pieces[0];
        Piece& kingBlue = pieces[1];

        bool round = 0;
        bool gameOver = false;
        from(_, 1, Q) {
            if (gameOver) {  // 游戏结束一定不合法
                cout << "Invalid command\n";
                continue;
            }
            int x1, y1, x2, y2;
            cin >> x1 >> y1 >> x2 >> y2;
            auto src = Vec2d{x1, y1};  // 起点
            auto dest = Vec2d{x2, y2};  // 终点

            auto selected = std::find_if(pieces.begin(), pieces.end(), 
                lambda(p, p.pos==src and p.team==round));  // 筛选，要求位置一致并且阵营正确
            if (selected == pieces.end()) {  // 没找到
                cout << "Invalid command\n";
                continue;
            }

            bool available = selected->reachable(dest);  // 判断终点位置是否可达
            if (not available) {
                cout << "Invalid command\n"; continue;
            }
            // 主语
            cout << (selected->team==Team::Red? "red ": "blue ") << selected->name << ';';

            // 宾语
            bool isKingKilled = false;  // 王被击杀，判断游戏结束
            auto attackedIter = std::find_if(pieces.begin(), pieces.end(), lambda(p, p.pos==dest));
            selected->pos = dest;
            if (attackedIter == pieces.end()) {
                cout << "NA;";  // 移到空格子
            } else {
                isKingKilled = (attackedIter->name=="captain");  // 游戏结束
                // 输出宾语
                cout << (attackedIter->team==Team::Red? "red ": "blue ") << attackedIter->name << ';';
                pieces.erase(attackedIter);  // 移除被吃的子
            }
            bool isKingInDanger = false;  // 被将军
            isKingInDanger &= !isKingKilled;  // 特判，王已经死了
            isKingInDanger |= pieces.some(lambda(p, p.reachable(kingRed.pos) and p.team==Team::Blue));
            isKingInDanger |= pieces.some(lambda(p, p.reachable(kingBlue.pos) and p.team==Team::Red));
            cout << (isKingInDanger? "yes;": "no;") << (isKingKilled? "yes": "no") << '\n';
            gameOver |= isKingKilled; 
            round ^= 1;  // 当且仅当操作合法，进行轮换
        }
    }
}

int main() {
    Solution::solve();
    return 0;
}
