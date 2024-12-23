/*
https://www.luogu.com.cn/discuss/795990
主猪MP
忠猪ZP
反猪FP
『桃 /P』
『闪 /D』
『杀 /K』
『决斗 /F』
『南蛮入侵 /N』
『万箭齐发 /W』
『无懈可击 /J』
『诸葛连弩 /Z』
*/
#include "./libs/debug_macros.hpp"

#include<bits/stdc++.h>
#include "./lib_v3.hpp"

using namespace lib;

using std::string;
using std::vector;
using std::cin;
using std::cout;

enum character {
    MP = 0,
    ZP = 1,
    FP = 2,
};

const int MaxHealth = 4;
int Begin = 1, End, cnt_of_FP, n, m, now_i, now_ii;
string winner;
std::deque<char> heap;  // * 改用 deque 维护牌堆

// * 添加
// 滚动的下标，从 size()-1 自增自动跳到 0
// 此外，自动跳过死亡的玩家
template <class Container_T>
class RollingIndex {
    Container_T &container;
    void _next() {
        assert(index < (int)container.size());
        index++;
        if (index>=(int)container.size())  index=0;
    }
public:
    int index = 0;
    RollingIndex(int x, Container_T &con): container(con) { 
        index = x;
    }
    RollingIndex(const RollingIndex &other): 
        container(other.container), index(other.index) {}
    RollingIndex next() {
        auto res = *this;
        do { res._next(); } while (container.at(res.index).isDead());
        return res;
    }
    int operator - (const RollingIndex& other) const {
        int dis=0;
        for (auto tmp=other; tmp!=*this; tmp++, dis++);
        return dis;
    }
    bool operator== (const RollingIndex& other) const { return index == other.index; }
    bool operator!= (const RollingIndex& other) const { return not (other == *this); }
    void operator++ () { *this = next(); }
    void operator++ (int) { *this = next(); }
    RollingIndex operator+ (int x) const {
        RollingIndex res = *this;
        for (auto i = 0; i < x; i++)  res++;
        return res;
    }
    RollingIndex &operator= (const RollingIndex &other) {
        assert(&container == &other.container);
        index = other.index;
        return *this;
    }
    operator int () const {
        return index;
    }
};

class Card {
public:
    char label;
    bool used = false;

    operator std::string () const {
        return std::format("{}", label);
    }

    bool operator== (char ch) const { return label == ch; }
};

class Player {
public:
    character ch = MP;
    std::vector<Card> cards;
    int health = 4;                              // 生命值
    RollingIndex<std::vector<Player>> index;   // 当前位置
    int ans_index = 0;                          // 初始位置
    int jumped = 0;                             // 1 = 跳忠，2 = 跳反
    int similarJumped = 0;                      // 包含“类反猪”的判定
    bool InfiniteKill = false;                  // 猪哥连弩
    bool canUseKill = true;                     // 这一轮可以继续使用“杀”

    Player();
    Player(int);

    void drawCard(int num);                                 // 抽卡
    bool isCardInvalid(char use_card, Player enemy);       // 检查合法性
    Player *findEnemy(char use_card);
    bool isDead();                                          // 是否死亡，如果是则进行后续工作
    void kill(Player& enemy);                              // 使用“杀”
    void fight(Player& enemy);                             // 发起决斗
    void arrows();                                          // 万箭齐发
    void invade();                                          // 南猪入侵
    bool peach();                                           // 桃
    int team() const;

    std::string identity() const {
        return std::format("P{}", ans_index);
    }

    operator std::string () const {
        auto base = std::format("Account(ch={}, HP={}, index={}, origin={}, jumped={}, similar={}, inf_kill={}, can_use_kill={}) with cards:\n",
            (int)ch, health, (int)index, ans_index, jumped, similarJumped, InfiniteKill, canUseKill
        );
        std::stringstream stream;
        for (auto c: cards)  stream << (std::string)c << ' ';
        stream << '\n';
        return base + stream.str();
    }

    friend std::ostream &operator<<(std::ostream &stream, Player const &element) {
        stream << (std::string)element;
        return stream;
    }
};
vector<Player> players;  // * 改为从 0 开始编号
Player &getMainPlayer() {
    return players.at(0);
}

Player::Player(int index): index(index, players) {}
Player::Player(): index(0, players) {}

// * 重写
void GAMEOVER() {
    std::cout << winner << std::endl;
    for (auto &pl: players) {
        if (pl.isDead()) {
            std::cout << "DEAD" << std::endl;
        } else {
            for (auto card: pl.cards) {
                std::cout << card.label << ' ';
            }
            std::cout << std::endl;
        }
    }
    throw 0;
}

// * 重构：不再移除死亡玩家
void checkDead(Player& subject, Player& object) {
    if (not object.isDead())  return;
    else {
        debug  std::cout << "Dead player: " << object << std::endl;
        // * 这里大概不需要
        // if (object.index<=now_i)now_i--;

        // * 添加：放弃装备和武器
        object.cards.clear();
        object.InfiniteKill = false;

        switch (object.ch) {
        case MP:
            winner = "FP";
            GAMEOVER();
            break;
        case ZP:
            if (subject.ch==MP) {
                //// // 这写法有点愚蠢了
                //// while (subject.cards.size()>1) {
                ////     subject.cards.erase(subject.cards.begin()+1, subject.cards.end());
                //// }
                // 执行惩罚
                subject.cards.clear();
                subject.InfiniteKill = 0;  // 解除猪哥连弩状态
            }
            break;
        case FP:
            cnt_of_FP--;
            if (cnt_of_FP==0) {  // 反猪清空
                winner = "MP";
                GAMEOVER();
            } else {
                subject.drawCard(3);  // 发放奖励
            }
            break;
        }
    }
}

// 基于距离约束卡牌
bool Player::isCardInvalid(char label, Player enemy) {
    auto right = index + 1;

    switch (label) {
    case 'K':
        return enemy.index == right;
        break;
    case 'F':
        return enemy.index != index;
        break;
    default:
        return false;
    }
}

//// 查找攻击目标，返回攻击目标的索引
// * 重构，返回目标的指针
// 查找攻击目标，返回指向它的指针
// ! 如果没有合法目标，返回 nullptr
Player *Player::findEnemy(char label) {
    auto idx = index, right = index + 1;
    // 按照逆时针顺序遍历
    for (auto i = right; i != idx; i++) {
        switch (ch) {
        case MP:  // 主猪打类反猪或反猪
            if (players[i].jumped) {
                if (players[i].jumped == 2) {  // 反猪
                    if (isCardInvalid(label, players[i]))  return &players[i];
                }
            } else {
                if (players[i].similarJumped) {
                    if (isCardInvalid(label, players[i]))  return &players[i];
                }
            }
            break;
        case ZP:  // 只打反猪
            if (players[i].jumped) {
                if (players[i].jumped == 2) {
                    if (isCardInvalid(label, players[i]))  return &players[i];
                }
            }
            break;
        case FP:
            if (isCardInvalid(label, getMainPlayer())) {
                return &getMainPlayer();  // 如果能打到主猪就打
            }
            if (players[i].jumped) {
                if (players[i].jumped==1||players[i].jumped==3) {
                    if (isCardInvalid(label, players[i]))  return &players[i];
                }
            }
            break;
        }
    }
    return nullptr;
}

// 抽卡
void Player::drawCard(int num) {
    for (int i = 1; i<=num; i++) {
        cards.push_back({heap.front()});
        // 特别地，防止牌堆为空
        if (heap.size() > 1) {
            heap.pop_front();
        }
    }
}

// 判断玩家是否死亡
bool Player::isDead() {
    if (health > 0)  return false;  // 生命大于 0
    else {
        // 尝试吃桃救一下
        auto it = std::find(cards.begin(), cards.end(), 'P');
        if (it != cards.end()) {
            cards.erase(it);
            health++;
            return false;
        } else {
            return true;
        }
    }
}

void Player::kill(Player& enemy) {
    debug  std::cout << std::format("{} Use Kill to {}", identity(), enemy.identity()) << std::endl;
    auto it_K = std::find(cards.begin(), cards.end(), 'K');
    assert(it_K != cards.end()), cards.erase(it_K);

    // 对方尝试使用“闪”躲避伤害
    auto it_D = std::find(enemy.cards.begin(), enemy.cards.end(), 'D');
    if (it_D != enemy.cards.end()) {
        enemy.cards.erase(it_D);
    } else {
        enemy.health--;
    }

    // 跳忠或跳反
    if (enemy.jumped and not jumped) {
        // 对着忠猪或者主猪跳反
        if (enemy.jumped==1 or enemy.jumped==3)  jumped = 2;
        // 对着反猪跳忠
        if (enemy.jumped==2)  jumped = 1;
    }

    checkDead(players[index], enemy);
}

#if false
// ? 写得毫无道理。
bool isSkillValid(Player& pos, Player& target, bool check) {
    auto idx = pos.index;
    int loop = 0;
    // ? 此处疑问：类反猪是否需要判断
    // * 移到循环外判断
    if (not target.jumped and not target.similarJumped)  return true;  // 没有亮身份，救不了
    for (auto i = idx; loop <= 2; i++) {
        auto it = std::find(players[i].cards.begin(), players[i].cards.end(), 'J');
        if (it != players[i].cards.end()) {
            switch (players[i].ch) {
            case MP:
                if (check) {
                    if (target.jumped==2) {
                        players[i].cards.erase(it);
                        return !isSkillValid(players[i], players[i], 1);
                    }
                    if (!target.jumped&&target.similarJumped) {
                        players[i].cards.erase(it);
                        return !isSkillValid(players[i], players[i], 1);
                    }
                } else {
                    if (target.jumped==1||target.jumped==3) {
                        players[i].cards.erase(it);
                        return !isSkillValid(players[i], players[i], 1);
                    }
                }

                break;
            case ZP:
                if (check) {
                    if (target.jumped==2) {
                        players[i].cards.erase(it);
                        players[i].jumped = 1;
                        return !isSkillValid(players[i], players[i], 1);
                    }
                } else {
                    if (target.jumped==3||target.jumped==1) {
                        players[i].cards.erase(it);
                        players[i].jumped = 1;
                        return !isSkillValid(players[i], players[i], 1);
                    }
                }

                break;
            case FP:
                if (check) {
                    if (target.jumped==3||target.jumped==1) {
                        players[i].cards.erase(it);
                        players[i].jumped = 2;
                        return !isSkillValid(players[i], players[i], 1);
                    }
                }
                if (target.jumped==3||target.jumped==1) {
                    if (target.jumped==2) {
                        players[i].cards.erase(it);
                        players[i].jumped = 2;
                        return !isSkillValid(players[i], players[i], 1);
                    }
                }
                break;
            }
        }

        if (i==idx) {
            loop++;
        }
    }
    return true;
}
#endif

int Player::team() const {
    switch (ch) {
    case MP:  return 1;
    case ZP:  return 1;
    case FP:  return 2;
    default:
        assert(false), __builtin_unreachable();
        return 0;
    }
}

/**
 * 尝试使用一张“无懈可击”。如果不存在，则不会做任何事情。
 * 在使用一张锦囊牌之前调用目标玩家的此方法。
 * 
 * @param before 希望被抵消的卡牌
 * @param original 最开始的受害者
 * @param pos 最开始可以使用无懈可击的位置
 * @param tag 标识奇偶性。具体地，tag 表示 before.owner 是否在对 original 献殷勤
 * @param depth 递归层数。
 * 
 * @returns 这次攻击是否被无效化
 */
bool tryExecutingUnbreakable(const Card before, const Player& original, 
            decltype(Player::index) pos, bool tag=false, int depth=0) {
    debug  std::cout << std::format("[{}] tryExecutingUnbreakable, before={}, original={}, pos={}, tag={}", depth, before.label, original.identity(), (int)pos, tag) << std::endl;
    if (not original.jumped and original.ch != MP) { // 没有亮身份，帮不了
        return false; 
    }
    // 使用无懈可击抵消献殷勤：表敌意
    // 使用无懈可击抵消表敌意：献殷勤
    // 使用无懈可击抵消决斗、南猪入侵、万箭齐发：献殷勤
    bool friendly = false;  // 这次无懈可击为是否为对 original 献殷勤
    if (before.label == 'F' or before.label == 'N' or before.label == 'W') {
        friendly = true;
    } else {  // 对于上一次无懈可击
        friendly = !tag; // 如果上一次是在献殷勤，这次就是在表敌意；反之亦然。
    }
    debug  std::cout << std::format("friendly = {}", friendly) << std::endl;

    // 接下来，给定所有人使用无懈可击的机会
    auto p = pos;
    do {
        debug  std::cout << std::format("checking p={};", (int)p);
        auto& pl = players.at(p);
        auto cardIt = std::find(pl.cards.begin(), pl.cards.end(), 'J');
        if (cardIt == pl.cards.end())  goto egg;  // 没有无懈可击，不可使用
        if (cardIt->used)  goto egg;

        // 删除
        cardIt->used = true;
        
        if (friendly) {
            auto character = original.team();
            if (character == pl.team()) {  // 相同阵营，向对方献殷勤
                auto card_copy = *cardIt;
                debug  std::cout << std::format("Abandon J of {}", pl.identity()) << std::endl;
                pl.cards.erase(cardIt);  // 使用卡牌
                pl.jumped = pl.ch;
                // 如果这次无懈可击没有被无效化，返回 true
                bool res = not tryExecutingUnbreakable(card_copy, original, p, friendly, depth+1);  
                // if (res == true)  log("This is unbreakable. \n")
                return res;
            }
        } else {  // 通过无懈表敌意
            auto character = 3 - original.team();  // 阵营取反
            if (character == pl.team()) {  // 阵营正确
                debug  std::cout << std::format("Abandon J of {}", pl.identity()) << std::endl;
                auto card_copy = *cardIt;
                pl.cards.erase(cardIt);
                pl.jumped = pl.ch;
                // 如果这次无懈可击没有被无效化，返回 true
                bool res = not tryExecutingUnbreakable(card_copy, original, p, friendly, depth+1);  
                // if (res == true)  { log("This is unbreakable. -> 1\n") }
                // else  { log("Done. This is NOT unbreakable. -> 0 \n") }
                return res;
            }
        }

        // 重新加入这张牌
        cardIt->used = false;

    egg:    // while 循环直接 continue 会炸，故致敬传奇 goto egg
        ++p;
    } while (p != pos);
    // 没有人选择使用无懈可击
    return false;  
}

void Player::fight(Player& enemy) {
    debug  std::cout << std::format("{} Use Fight to {}", identity(), enemy.identity()) << std::endl;

    // 决斗
    auto it_F = std::find(cards.begin(), cards.end(), 'F');
    cards.erase(it_F);

    // 表敌意
    // ? 忠猪是否无需判断
    if (enemy.ch == MP)  jumped = 2;  // 跳反
    if (enemy.ch == FP and not jumped)  jumped = 1;

    // 对方尝试使用无懈可击挡下
    if (not tryExecutingUnbreakable({'F'}, enemy, index)) {
        // 忠猪始终不打主猪
        if (enemy.ch == ZP and ch == MP) {
            enemy.health--;
            checkDead(players[index], enemy);
        } else {
            // ? 关于跳忠和跳反的判定似乎有问题
            while (1) {
                // 对方尝试弃一张杀
                auto it_K = std::find(enemy.cards.begin(), enemy.cards.end(), 'K');
                if (it_K == enemy.cards.end()) {  // 没有卡了
                    enemy.health--;
                    checkDead(players[index], enemy);
                    break;
                } else {
                    if (ch==MP and enemy.ch==FP)  enemy.jumped = 2;
                    enemy.cards.erase(it_K);

                    auto it2 = std::find(cards.begin(), cards.end(), 'K');
                    if (it2==cards.end()) {
                        health--;
                        checkDead(enemy, players[index]);
                        break;
                    } else {
                        cards.erase(it2);
                    }
                }
            }
        }
    }
    //	else std::cout<<"无懈可击\n";
}

void Player::arrows() {
    debug  std::cout << std::format("{} Use Arrows", identity()) << std::endl;

    auto it1 = std::find(cards.begin(), cards.end(), 'W');
    cards.erase(it1);

    auto idx = index;
    auto right = idx + 1;

    for (auto i = right; i != index; i++) {
        // 无懈可击
        // * 这里不应该因为一次无懈可击跳出整个流程
        //// if (!isSkillValid(players[index], players[i], 0))break;
        if (tryExecutingUnbreakable({'W'}, players[i], index, 0))  continue;
        else {
            auto it = std::find(players[i].cards.begin(), players[i].cards.end(), 'D');
            if (it == players[i].cards.end()) {
                // 不能继续丢弃“闪”
                players[i].health--;
                if (&players[i] == &getMainPlayer())  similarJumped = true;  // 打到主猪，类反猪
                if (players[i].isDead()) {
                    checkDead(players[index], players[i]);
                }
            } else {
                players[i].cards.erase(it);
            }
        }
    }
}

void Player::invade() {
    debug  std::cout << std::format("{} Use Invade", (std::string)*this) << std::endl;
    auto it1 = std::find(cards.begin(), cards.end(), 'N');
    cards.erase(it1);

    auto idx = index;
    auto right = idx + 1;

    for (auto i = right; i != index; i++) {
        // * 和 arrows 的错误相同，此处需要用 continue
        if (tryExecutingUnbreakable({'N'}, players[i], index))  continue;
        else {
            auto it = std::find(players[i].cards.begin(), players[i].cards.end(), 'K');

            if (it == players[i].cards.end()) {
                if (&players[i] == &getMainPlayer())  similarJumped = 1;
                players[i].health--;
                debug std::cout << std::format("Blood of player{}-- to {}", (std::string)players[i], players[i].health) << std::endl;
                if (players[i].isDead()) {
                    checkDead(players[index], players[i]);
                }
            } else {
                players[i].cards.erase(it);
            }
        }
    }
}

bool Player::peach() {
    debug  std::cout << std::format("{} Use Peach", identity()) << std::endl;
    if (health != MaxHealth) {
        health++;
        return true;
    }
    return false;
}
// 玩家行动轮
void play(Player& ac) {
    ac.drawCard(2);  // 摸牌
    debug  std::cout << ac;
    // * 此处维护下标而非迭代器避免失效
    // * 此外，从头开始循环需要回到 begin - 1 而不是 begin
    for (auto i = 0; i < (int)ac.cards.size(); i++) {
        debug std::cout << std::format("Player {} checking card {}/{}", ac.identity(), i, ac.cards.size()) << std::endl;
        auto &card = ac.cards.at(i);
        switch (card.label) {
        case 'P':  // 桃
            if (ac.peach()) {  // 能吃桃就吃
                ac.cards.erase(ac.cards.begin() + i);
                i = -1;
            }
            break;
        case 'K':  // 杀
            if (ac.InfiniteKill) {  // 有猪哥连弩
                if (ac.findEnemy('K')) {  // 攻击目标
                    ac.kill(*ac.findEnemy('K'));
                    i = -1;
                }
            } else if (ac.canUseKill) {  // 没用过
                if (ac.findEnemy('K')) {
                    //						std::cout<<ac.index<<" kill "<< ac.findEnemy('K')<<endl;
                    ac.kill(*ac.findEnemy('K'));
                    ac.canUseKill = false;
                    i = -1;
                }
            }
            break;
        case 'F':  // 决斗
            if (ac.findEnemy('F')) {
                ac.fight(*ac.findEnemy('F'));
                i = -1;
            }
            break;
        case 'N':  // 南猪入侵
            ac.invade();
            i = -1;
            break;
        case 'W':  // 万箭齐发
            ac.arrows();
            i = -1;
            break;
        case 'Z':  // 猪哥连弩
            ac.InfiniteKill = 1;
            ac.cards.erase(ac.cards.begin() + i);
            i = -1;
            break;
        }
    }
}
int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    //	freopen("D:\\in.txt","r",stdin); //输入重定向，输入数据将从D盘根目录下的in.txt文件中读取 
    //	freopen("D:\\out.txt","w",stdout); //输出重定向，输出数据将保存在D盘根目录下的out.txt文件中 

    std::cin >> n >> m;  // 输入 n 和 m

    for (int i = 0; i < n; i++) {
        Player pig(i);
        pig.ans_index = i;
        std::string a0;
        std::cin >> a0;
        switch (a0[0]) {
        case 'M':
            pig.ch = MP;
            break;
        case 'Z':
            pig.ch = ZP;
            break;
        case 'F':
            pig.ch = FP;
            cnt_of_FP++;
            break;
        }

        for (int j = 0; j < 4; j++) {
            char c0;
            std::cin >> c0;
            pig.cards.push_back({c0});
        }
        players.push_back(pig);
    }
    for (int i = 0; i < m; i++) {
        char c;
        std::cin >> c;
        heap.push_back(c);
    }
    
    getMainPlayer().jumped = 3;
    if (cnt_of_FP == 0) {
        winner = "MP";
        GAMEOVER();
    }

    try {
        while (1) {
            debug  std::cout << "New round!" << std::endl;
            for (auto &pl: players | views::filter(lam(x, not x.isDead()))) {
                play(pl);
                pl.canUseKill = true;
                debug  std::cout << pl;
                debug  for (auto &pl: players) {
                    if (pl.isDead()) {
                        std::cout << "DEAD" << std::endl;
                        continue;
                    } else {
                        std::cout << pl.health << ' ';
                        for (auto c: pl.cards) {
                            assert(not c.used);
                            std::cout << c.label << ' ';
                        }
                        std::cout << std::endl;
                    }
                }
            }
        }
    } catch (int err) {
        if (err == 0)  return 0;
        else  throw err;
    }

}
/*
FP
DEAD
K P P
K

W F P
*/