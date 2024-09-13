// Pig Kingdom Kill
#include <bits/stdc++.h>
#define upto(i, n) for(int i=1; i<=n; i++)
#define DEBUG_MOD true
#define debug if(DEBUG_MOD)

const int playerPowerLimit = 4;         // 角色的体力值上限
int playersCount;                       // 玩家数量
int cardsCount;                         // 牌堆中的牌数

enum PlayerType {
    Master='M',         // 主猪
    Minister='Z',       // 忠猪
    Enemy='F'           // 反猪
};
enum PlayerStatus { // 玩家状态。在每回合开始时重置为None
    None,
    ToBeKilled,     // 正在受到“杀”的伤害
};

class Card;
// 玩家
class Player {
public:
    int identifier = -1;                        // 编号
    int power = 4;                              // 体力值
    PlayerStatus status = PlayerStatus::None;   // 当前状态
    PlayerType type;                            // 角色类型
    std::vector<Card> cards;                    // 卡牌列表
} nullPlayer;
// 卡牌
class Card {
protected:
    Player owner;
public:
    char name;
    Card(const Player& owner, char name) { this->owner = owner; this->name = name; };
    virtual bool apply(Player& attackTarget) {
        debug printf("Virtual apply called!\n");
        return false;
    }
    virtual bool apply() {
        debug printf("Virtual apply 2 called!\n");
        return false;
    }
};


Card generateCard(char description, Player& owner);
PlayerType parsePlayerType(char typeChar);

std::vector<Player> players;        // 玩家列表
std::queue<Player> waitingPlayers; // 等待动作的玩家

// “桃”
class P_PeachCard : public Card {
public:
    P_PeachCard(const Player& owner): Card(owner, 'P') {}
    bool apply() override {
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
    bool apply(Player& attackTarget) override {
        attackTarget.power -= 1;
        waitingPlayers.push(attackTarget);     // 允许被攻击对象使用“闪”等
        return true;
    }
};

// “闪”
class D_EvadeCard: public Card {
public:
    D_EvadeCard(Player& owner): Card(owner, 'D') {}
    bool apply() override {
        debug printf("!\n");
        if (this->owner.status == PlayerStatus::ToBeKilled) {
            this->owner.power += 1;
            return true;
        }
        return false;
    }
};


int main() {
    std::ios::sync_with_stdio(false);
    std::cin >> playersCount >> cardsCount;
    char typeString[3], cardChar;
    for (int playerId = 0; playerId < playersCount; playerId++) {
        auto player = Player();
        std::cin >> typeString;
        player.type = parsePlayerType(typeString[0]);
        upto(i, 4) {
            std::cin >> cardChar;
            Card card = generateCard(cardChar, player);
            player.cards.push_back(card);
        }
        players.push_back(player);
    }
    debug players[0].cards[0].apply();
    debug printf("%c\n", players[0].cards[0].name);
    debug D_EvadeCard(nullPlayer).apply();
    return 0;

    /*
3 10
MP D D F F
ZP N N N D
FP J J J J
F F D D J J F F K D
     */
}





Card generateCard(char description, Player& owner) {
    switch (description) {
    case 'P':
        return P_PeachCard(owner);
    case 'K':
        return K_KillingCard(owner);
    case 'D': 
        return D_EvadeCard(owner);
    }
    return P_PeachCard(owner);
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