// Pig Kingdom Kill
#include <bits/stdc++.h>
#define upto(i, n) for(int i=1; i<=n; i++)
#define DEBUG_MOD false
#define debug if(DEBUG_MOD)

const int playerPowerLimit = 4;         // 角色的体力值上限
int playersCount;                       // 玩家数量
int cardsCount;                         // 牌堆中的牌数

class Card;     // 卡牌
class Player;   // 玩家
enum PlayerType {
    Master='M',         // 主猪
    Minister='Z',       // 忠猪
    Enemy='F'           // 反猪
};
enum PlayerStatus { // 玩家状态。在每回合开始时重置为None
    None,
    ToBeKilled,     // 正在受到“杀”的伤害
};

Card generateCard(char description);
PlayerType parsePlayerType(char typeChar);

std::vector<Player> players;        // 玩家列表
std::queue<Player> waitingPlayers; // 等待动作的玩家


int main() {
    std::cin >> playersCount >> cardsCount;

    for (int playerId = 0; playerId < playersCount; playerId++) {

    }
    return 0;
}

class Card {
protected:
    Player owner;
public:
    Card(const Player& owner) { this->owner = owner; };
    virtual bool apply(Player* attackTarget) {}     // 传入使用目标或者nullptr
};

// “桃”
class P_PeachCard : public Card {
public:
    P_PeachCard(const Player& owner): Card(owner) {}
    bool apply(Player* attackTarget) {
        int& power = this->owner.power;
        if (power == playerPowerLimit)  return false;
        power += 1;
        return true;
    }
};

// “杀”
class K_KillingCard: public Card {
public:
    K_KillingCard(const Player& owner): Card(owner) {}
    bool apply(Player& attackTarget) {
        attackTarget.power -= 1;
        waitingPlayers.push(attackTarget);     // 允许被攻击对象使用“闪”等
        return true;
    }
};

// “闪”
class D_EvadeCard: public Card {
public:
    D_EvadeCard(Player& owner): Card(owner) {}
    bool apply(Player& attackTarget) {
        if (this->owner.status == PlayerStatus::ToBeKilled) {
            attackTarget.power += 1;
            return true;
        }
        return false;
    }
};

class Player {
public:
    int identifier = -1;                        // 编号
    int power = 4;                              // 体力值
    PlayerStatus status = PlayerStatus::None;   // 当前状态
    std::vector<Card> cards;                    // 卡牌列表
};

Card generateCard(char description, const Player& owner) {
    switch (description) {
    case 'P':
        return P_PeachCard(owner);
    case 'K':
        return K_KillingCard(owner);
    case 'D': 
        return D_EvadeCard(owner);
    }
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