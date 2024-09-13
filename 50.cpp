// Pig Kingdom Kill
#include <bits/stdc++.h>
#define upto(i, n) for(int i=1; i<=n; i++)
#define DEBUG_MOD true
#define debug if(DEBUG_MOD)

const int playerPowerLimit = 4;         // 角色的体力值上限
int playersCount;                       // 玩家数量
int cardsCount;                         // 牌堆中的牌数

// !枚举
enum PlayerType {
    Master='M',         // 主猪
    Minister='Z',       // 忠猪
    Enemy='F'           // 反猪
};
enum PlayerStatus { // 玩家状态。在每回合开始时重置为None
    None,
    ToBeKilled,     // 正在受到“杀”的伤害
};

// ! 类定义
class Card;
class Player {
public:
    Player() {}
    int identifier = -1;                        // 编号
    int power = 4;                              // 体力值
    PlayerStatus status = PlayerStatus::None;   // 当前状态
    PlayerType type;                            // 类型
    std::vector<Card> cards;                    // 卡牌列表
} nullPlayer;

class Card {
protected:
    Player owner;
public:
    char name;
    Card(const Player& owner, char name) { this->owner = owner; this->name = name; };
    virtual bool apply(Player target=nullPlayer) {
        debug printf("virtual apply\n");
        return false;
    }     // 传入使用目标
};

std::vector<Player> players;        // 玩家列表
std::queue<Player> waitingPlayers; // 等待动作的玩家

// ! 卡牌类
// “桃”
class P_PeachCard : public Card {
public:
    P_PeachCard(const Player& owner): Card(owner, 'P') {}
    bool apply() {
        int& power = this->owner.power;
        if (power == playerPowerLimit)  return false;
        power += 1;
        return true;
    }
};
// “杀”
class K_KillingCard: public Card {
public:
    K_KillingCard(const Player& owner): Card(owner, 'K') {}
    bool apply(Player& attackTarget) {
        attackTarget.power -= 1;
        waitingPlayers.push(attackTarget);     // 允许被攻击对象使用“闪”等
        return true;
    }
};
// “闪”
class D_EvadeCard: public Card {
public:
    D_EvadeCard(Player& owner): Card(owner, 'D') {}
    bool apply() {
        if (this->owner.status == PlayerStatus::ToBeKilled) {
            this->owner.power += 1;
            return true;
        }
        debug printf("!!!!\n");
        return false;
    }
};

// !函数
Card generateCard(char description, Player& owner) {
    switch (description) {
    case 'P':
        return P_PeachCard(owner);
    case 'K':
        return K_KillingCard(owner);
    case 'D': 
        return D_EvadeCard(owner);
    }
    return P_PeachCard(owner);  // 不应运行到此处，只适用于忽略编译警告
}
PlayerType parsePlayerType(char typeChar) {
    switch (typeChar) {
    case 'M':
        return PlayerType::Master;
    case 'Z':
        return PlayerType::Minister;
    default:
        return PlayerType::Enemy;
    }
}


int main() {
    std::cin >> playersCount >> cardsCount;
    char typeString[2], card1, card2, card3, card4;
    for (int playerId = 0; playerId < playersCount; playerId++) {
        scanf("%s %c %c %c %c", typeString, &card1, &card2, &card3, &card4);
        auto newPlayer = Player();
        newPlayer.type = parsePlayerType(typeString[0]);
        newPlayer.cards.push_back(generateCard(card1, newPlayer));
        newPlayer.cards.push_back(generateCard(card2, newPlayer));
        newPlayer.cards.push_back(generateCard(card3, newPlayer));
        newPlayer.cards.push_back(generateCard(card4, newPlayer));
        players.push_back(newPlayer);
    }
    players[0].cards[0].apply();
    (new D_EvadeCard(players[0]))->apply();
    return 0;
}

