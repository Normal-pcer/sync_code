#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#ifdef __cplusplus
#define this this_
#else  // not __cplusplus
#define nullptr NULL
#define true 1
#define false 0
#define bool _Bool
#define not !
#define and &&
#define or ||
#endif  // __cplusplus
#define MaxHeapSize 2005
#define PlayersMaxSize 10

typedef enum Character {
    MP = 0,
    ZP = 1,
    FP = 2,
} Character;

const int MaxHealth = 4;
int cnt_of_FP, n, m;
const char *winner;
char heap[MaxHeapSize];
char *heapHead = heap, *heapTail = heap;

typedef struct Card {
    char label;
    bool used;
} Card;
bool CardEquals(Card card, char ch) {
    return card.label == ch;
}
Card CardCreate(char label) {
    Card res = {label, false};
    return res;
}

struct VectorCard {
    Card* vec;
    int size, capa;
};
typedef struct VectorCard VectorCard;

void VectorCardRequestMemory(VectorCard *this) {
    this->capa = this->capa? this->capa<<1: 1;
    Card *new_vec = (Card *)malloc(this->capa * sizeof(Card));
    assert(new_vec);
    if (this->vec != nullptr) {
        memcpy(new_vec, this->vec, this->size * sizeof(Card));
        free(this->vec);
    }
    this->vec = new_vec;
}
VectorCard VectorCardCreate() {
    VectorCard res = {nullptr, 0, 0};
    return res;
}
void VectorCardDestroy(VectorCard *this) {
    if (this->vec != nullptr) {
        free(this->vec);
    }
}
void VectorCardPushBack(VectorCard *this, Card x) {
    if (this->size >= this->capa) {
        VectorCardRequestMemory(this);
    }
    this->vec[this->size++] = x;
}
void VectorCardPopBack(VectorCard *this) {
    this->size--;
}
Card *VectorCardAt(VectorCard *this, int x) {
    return &this->vec[x];
}
int VectorCardSize(VectorCard *this) {
    return this->size;
}
void VectorCardClear(VectorCard *this) {
    this->size = 0;
}
void VectorCardErase(VectorCard *this, Card *pos) {
    this->size--;
    for (; pos != this->vec + this->size; pos++) {
        pos[0] = pos[1];
    }
}

typedef struct Player {
    Character ch;
    VectorCard cards;
    int health;
    int index;
    int ans_index;
    int jumped;
    int similarJumped;
    bool infiniteKill;
    bool canUseKill;
} Player;
int playersSize;
Player players[PlayersMaxSize];  // * 改为从 0 开始编号

Player PlayerCreate(Character ch) {
    Player res = {ch, VectorCardCreate(), 4, 0, 0, 0, 0, false, true};
    return res;
}
Player *getMainPlayerPointer() {
    return players;
}

bool PlayerIsDead(Player *);
void PlayerDrawCard(Player *, int);

// * 重写
void GameOver() {
    printf("%s\n", winner);
    for (int i = 0; i < playersSize; i++) {
        if (PlayerIsDead(&players[i])) {
            puts("DEAD");
        } else {
            for (int j = 0; j < VectorCardSize(&players[i].cards); j++) {
                Card card = *VectorCardAt(&players[i].cards, j);
                putchar(card.label), putchar(' ');
            }
            putchar('\n');
        }
        VectorCardDestroy(&players[i].cards);
    }
    exit(0);
}

// * 重构：不再移除死亡玩家
void checkDead(Player *subject, Player *object) {
    if (not PlayerIsDead(object))  return;
    else {
        // * 这里大概不需要
        // if (object.index<=now_i)now_i--;

        // * 添加：放弃装备和武器
        VectorCardClear(&object->cards);
        object->infiniteKill = false;

        switch (object->ch) {
        case MP:
            winner = "FP";
            GameOver();
            break;
        case ZP:
            if (subject->ch == MP) {
                // 执行惩罚
                VectorCardClear(&subject->cards);
                subject->infiniteKill = false;  // 解除猪哥连弩状态
            }
            break;
        case FP:
            cnt_of_FP--;
            if (cnt_of_FP == 0) {  // 反猪清空
                winner = "MP";
                GameOver();
            } else {
                PlayerDrawCard(subject, 3);  // 发放奖励
            }
            break;
        }
    }
}

int IndexNext(int index) {
    int res = index;
    do {
        assert(res < playersSize);
        res++;
        if (res >= playersSize)  res = 0;
    } while (PlayerIsDead(&players[res]));
    return res;
}

// 基于距离约束卡牌
bool PlayerIsCardInvalid(Player *this, char label, Player *enemy) {
    int right = IndexNext(this->index);

    switch (label) {
    case 'K':
        return enemy->index == right;
        break;
    case 'F':
        return enemy->index != this->index;
        break;
    default:
        return false;
    }
}


//// 查找攻击目标，返回攻击目标的索引
// * 重构，返回目标的指针
// 查找攻击目标，返回指向它的指针
// ! 如果没有合法目标，返回 nullptr
Player *PlayerFindEnemy(Player *this, char label) {
    int idx = this->index, right = IndexNext(this->index);
    // 按照逆时针顺序遍历
    for (int i = right; i != idx; i = IndexNext(i)) {
        switch (this->ch) {
        case MP:  // 主猪打类反猪或反猪
            if (players[i].jumped) {
                if (players[i].jumped == 2) {  // 反猪
                    if (PlayerIsCardInvalid(this, label, &players[i]))  return &players[i];
                }
            } else {
                if (players[i].similarJumped) {
                    if (PlayerIsCardInvalid(this, label, &players[i]))  return &players[i];
                }
            }
            break;
        case ZP:  // 只打反猪
            if (players[i].jumped) {
                if (players[i].jumped == 2) {
                    if (PlayerIsCardInvalid(this, label, &players[i]))  return &players[i];
                }
            }
            break;
        case FP:
            if (PlayerIsCardInvalid(this, label, getMainPlayerPointer())) {
                return getMainPlayerPointer();  // 如果能打到主猪就打
            }
            if (players[i].jumped) {
                if (players[i].jumped==1||players[i].jumped==3) {
                    if (PlayerIsCardInvalid(this, label, &players[i]))  return &players[i];
                }
            }
            break;
        }
    }
    return nullptr;
}

// 抽卡
void PlayerDrawCard(Player *this, int num) {
    for (int i = 1; i <= num; i++) {
        Card res = CardCreate(*heapHead);
        VectorCardPushBack(&this->cards, res);
        // 特别地，防止牌堆为空
        if (heapTail - heapHead > 1) {
            heapHead++;
        }
    }
}

Card *findCard(VectorCard *vec, char label) {
    Card *begin = vec->vec;
    Card *end = begin + vec->size;
    for (; begin != end; begin++) {
        if (CardEquals(*begin, label))  return begin;
    }
    return nullptr;
}

// 判断玩家是否死亡
bool PlayerIsDead(Player *this) {
    if (this->health > 0)  return false;  // 生命大于 0
    else {
        // 尝试吃桃救一下
        Card *it = findCard(&this->cards, 'P');
        if (it) {
            VectorCardErase(&this->cards, it);
            this->health++;
            return false;
        } else {
            return true;
        }
    }
}


void PlayerKill(Player *this, Player *enemy) {
    Card *it_K = findCard(&this->cards, 'K');
    VectorCardErase(&this->cards, it_K);

    // 对方尝试使用“闪”躲避伤害
    Card *it_D = findCard(&enemy->cards, 'D');
    if (it_D) {
        VectorCardErase(&enemy->cards, it_D);
    } else {
        enemy->health--;
    }

    // 跳忠或跳反
    if (enemy->jumped and not this->jumped) {
        // 对着忠猪或者主猪跳反
        if (enemy->jumped==1 or enemy->jumped==3)  this->jumped = 2;
        // 对着反猪跳忠
        if (enemy->jumped==2)  this->jumped = 1;
    }

    checkDead(&players[this->index], enemy);
}

int PlayerTeam(const Player *this) {
    switch (this->ch) {
    case MP:  return 1;
    case ZP:  return 1;
    case FP:  return 2;
    default:
        assert(false);
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
bool tryExecutingUnbreakable(const Card before, const Player *original, int pos, bool tag, int depth) {
    if (not original->jumped and original->ch != MP) { // 没有亮身份，帮不了
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

    // 接下来，给定所有人使用无懈可击的机会
    int p = pos;
    do {
        Player *pl = &players[p];
        Card *cardIt = findCard(&pl->cards, 'J');
        if (cardIt == nullptr)  goto egg;  // 没有无懈可击，不可使用
        if (cardIt->used)  goto egg;

        // 删除
        cardIt->used = true;
        
        if (friendly) {
            int character = PlayerTeam(original);
            if (character == PlayerTeam(pl)) {  // 相同阵营，向对方献殷勤
                Card card_copy = *cardIt;
                VectorCardErase(&pl->cards, cardIt);  // 使用卡牌
                pl->jumped = pl->ch;
                // 如果这次无懈可击没有被无效化，返回 true
                bool res = not tryExecutingUnbreakable(card_copy, original, p, friendly, depth+1);  
                return res;
            }
        } else {  // 通过无懈表敌意
            int character = 3 - PlayerTeam(original);  // 阵营取反
            if (character == PlayerTeam(pl)) {  // 阵营正确
                Card card_copy = *cardIt;
                VectorCardErase(&pl->cards, cardIt);
                pl->jumped = pl->ch;
                // 如果这次无懈可击没有被无效化，返回 true
                bool res = not tryExecutingUnbreakable(card_copy, original, p, friendly, depth+1);  
                return res;
            }
        }

        // 重新加入这张牌
        cardIt->used = false;

    egg:    // while 循环直接 continue 会炸，故致敬传奇 goto egg
        p = IndexNext(p);
    } while (p != pos);
    // 没有人选择使用无懈可击
    return false;  
}


void PlayerFight(Player *this, Player *enemy) {
    // 决斗
    Card *it_F = findCard(&this->cards, 'F');
    VectorCardErase(&this->cards, it_F);

    // 表敌意
    if (enemy->ch == MP)  this->jumped = 2;  // 跳反
    if (enemy->ch == FP and not this->jumped)  this->jumped = 1;

    // 对方尝试使用无懈可击挡下
    if (not tryExecutingUnbreakable(CardCreate('F'), enemy, this->index, false, 0)) {
        // 忠猪始终不打主猪
        if (enemy->ch == ZP and this->ch == MP) {
            enemy->health--;
            checkDead(&players[this->index], enemy);
        } else {
            // ? 关于跳忠和跳反的判定似乎有问题
            while (1) {
                // 对方尝试弃一张杀
                Card *it_K = findCard(&enemy->cards, 'K');
                if (it_K == nullptr) {  // 没有卡了
                    enemy->health--;
                    checkDead(&players[this->index], enemy);
                    break;
                } else {
                    if (this->ch == MP and enemy->ch == FP)  enemy->jumped = 2;
                    VectorCardErase(&enemy->cards, it_K);

                    Card *it2 = findCard(&this->cards, 'K');
                    if (it2 == nullptr) {
                        this->health--;
                        checkDead(enemy, &players[this->index]);
                        break;
                    } else {
                        VectorCardErase(&this->cards, it2);
                    }
                }
            }
        }
    }
}
void PlayerArrows(Player *this) {
    Card *it1 = findCard(&this->cards, 'W');
    VectorCardErase(&this->cards, it1);

    int idx = this->index;
    int right = IndexNext(idx);

    for (int i = right; i != this->index; i = IndexNext(i)) {
        // 无懈可击
        if (tryExecutingUnbreakable(CardCreate('W'), &players[i], this->index, false, 0))  continue;
        else {
            Card *it = findCard(&players[i].cards, 'D');
            if (it == nullptr) {
                // 不能继续丢弃“闪”
                players[i].health--;
                if (&players[i] == getMainPlayerPointer())  this->similarJumped = true;  // 打到主猪，类反猪
                if (PlayerIsDead(&players[i])) {
                    checkDead(&players[this->index], &players[i]);
                }
            } else {
                VectorCardErase(&players[i].cards, it);
            }
        }
    }
}
void PlayerInvade(Player *this) {
    Card *it1 = findCard(&this->cards, 'N');
    VectorCardErase(&this->cards, it1);

    int idx = this->index;
    int right = IndexNext(idx);

    for (int i = right; i != this->index; i = IndexNext(i)) {
        // * 和 arrows 的错误相同，此处需要用 continue
        if (tryExecutingUnbreakable(CardCreate('N'), &players[i], this->index, false, 0))  continue;
        else {
            Card *it = findCard(&players[i].cards, 'K');

            if (it == nullptr) {
                if (&players[i] == getMainPlayerPointer())  this->similarJumped = 1;
                players[i].health--;
                if (PlayerIsDead(&players[i])) {
                    checkDead(&players[this->index], &players[i]);
                }
            } else {
                VectorCardErase(&players[i].cards, it);
            }
        }
    }
}

bool PlayerPeach(Player *this) {
    if (this->health != MaxHealth) {
        this->health++;
        return true;
    }
    return false;
}


// 玩家行动轮
void play(Player* ac) {
    PlayerDrawCard(ac, 2);
    // * 此处维护下标而非迭代器避免失效
    // * 此外，从头开始循环需要回到 begin - 1 而不是 begin
    for (int i = 0; i < VectorCardSize(&ac->cards); i++) {
        Card *card = VectorCardAt(&ac->cards, i);
        switch (card->label) {
        case 'P':  // 桃
            if (PlayerPeach(ac)) {  // 能吃桃就吃
                VectorCardErase(&ac->cards, ac->cards.vec + i);
                i = -1;
            }
            break;
        case 'K':  // 杀
            if (ac->infiniteKill) {  // 有猪哥连弩
                if (PlayerFindEnemy(ac, 'K')) {  // 攻击目标
                    PlayerKill(ac, PlayerFindEnemy(ac, 'K'));
                    i = -1;
                }
            } else if (ac->canUseKill) {  // 没用过
                if (PlayerFindEnemy(ac, 'K')) {
                    PlayerKill(ac, PlayerFindEnemy(ac, 'K'));
                    ac->canUseKill = false;
                    i = -1;
                }
            }
            break;
        case 'F':  // 决斗
            if (PlayerFindEnemy(ac, 'F')) {
                PlayerFight(ac, PlayerFindEnemy(ac, 'F'));
                i = -1;
            }
            break;
        case 'N':  // 南猪入侵
            PlayerInvade(ac);
            i = -1;
            break;
        case 'W':  // 万箭齐发
            PlayerArrows(ac);
            i = -1;
            break;
        case 'Z':  // 猪哥连弩
            ac->infiniteKill = 1;
            VectorCardErase(&ac->cards, ac->cards.vec + i);
            i = -1;
            break;
        }
    }
}

char getNextChar() {
    char ch = getchar();
    while (ch <= 32 or ch >= 127)  ch = getchar();
    return ch;
}

int main() {
    assert(scanf("%d%d", &n, &m)), playersSize = n;

    for (int i = 0; i < n; i++) {
        char ch_label = getNextChar();
        getNextChar();
        Character ch = MP;
        switch (ch_label) {
        case 'M':
            ch = MP;
            break;
        case 'Z':
            ch = ZP;
            break;
        case 'F':
            ch = FP;
            cnt_of_FP++;
            break;
        default:
            assert(false);
        }

        Player pig = PlayerCreate(ch);
        pig.ans_index = pig.index = i;

        for (int j = 0; j < 4; j++) {
            char c0 = getNextChar();
            VectorCardPushBack(&pig.cards, CardCreate(c0));
        }
        players[i] = pig;
    }

    for (int i = 0; i < m; i++) {
        char c = getNextChar();
        *heapTail++ = c;
    }
    
    getMainPlayerPointer()->jumped = 3;
    if (cnt_of_FP == 0) {
        winner = "MP";
        GameOver();
    }

    while (1) {
        for (Player *pl = players; pl != players + n; pl++) {
            if (PlayerIsDead(pl))  continue;
            play(pl);
            pl->canUseKill = true;
        }
    }
}