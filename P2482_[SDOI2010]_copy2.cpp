/**
 * @link https://www.luogu.com.cn/problem/P2482
 */

#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=(n);i++)
#define from(i,b,e) for(int i=(b);i<=(e);i++)
#define rev(i,e,b) for(int i=(e);i>=(b);i--)
#define main() main(int argc, char const *argv[])
#define optimizeIO std::ios::sync_with_stdio(false); std::cin.tie(0); std::cout.tie(0);
#define chkMax(base,cmp...) (base=std::max({(base),##cmp}))
#define chkMin(base,cmp...) (base=std::min({(base),##cmp}))
#define chkMaxEx(base,exchange,other,cmp...) {auto __b__=base;if(__b__!=chkMax(base,##cmp)){exchange;} else other;}
#define chkMinEx(base,exchange,other,cmp...) {auto __b__=base;if(__b__!=chkMin(base,##cmp)){exchange;} else other;}
#define ensure(v, con, otw) (((v) con)? (v): (otw))
#define never if constexpr(0)
#define always if constexpr(1)
#define bitOr(x,y) (((x)&(y))^(((x)^(y))|(~(x)&(y))))
#define Infinity 2147483647
#define compare(x,y,g,e,l) (((x)>(y))?(g):(((x)<(y))?(l):(e)))
bool DEBUG_MODE=false;
typedef long long ll; typedef unsigned long long ull;
inline void batchOutput(int *begin, int n, const char *format){upto(i, n)printf(format, begin[i]);printf("\n");} inline void batchOutput(int*begin, int n) {batchOutput(begin,n,"%3d ");}
#define batchOutput2d(b, r, c, fmt) upto(i,r){upto(j,c)printf(fmt,b[i][j]);printf("\n");}
#define __macro_arg_counter(_1,_2,_3,_4,_5, N, ...) N
#define macro_arg_counter(...)  __macro_arg_counter(__VA_ARGS__,5,4,3,2,1,0)
#define __macro_choose_helper(M,count)  M##count
#define macro_choose_helper(M,count)   __macro_choose_helper(M,count)
#define __lambda_1(expr) [&](){return expr;}
#define __lambda_2(a, expr) [&](auto a){return expr;}
#define __lambda_3(a, b, expr) [&](auto a, auto b){return expr;}
#define __lambda_4(a, b, c, expr) [&](auto a, auto b, auto c){return expr;}
#define lambda(args...) macro_choose_helper(__lambda_, macro_arg_counter(args))(args)
#define lam lambda
namespace lib{}
#include <bits/stdc++.h>
#define USE_FREAD
// #undef USE_FREAD
// 取消注释上一行会使用 getchar() 替代 fread，可以不使用 EOF 结束读入，但是降低性能 
namespace lib{
#ifndef LIB_STRING
    using string=std::string;
#endif
#ifdef USE_FREAD
    template <const long long MAXSIZE, const long long MAX_ITEM_SZ=500>
#endif
    struct IO {
#ifdef USE_FREAD
        char buf[MAXSIZE],*p1,*p2;
        char pbuf[MAXSIZE],*pp;
        IO():p1(buf),p2(buf),pp(pbuf) {}
        ~IO() {  fwrite(pbuf,1,pp-pbuf,stdout);  }
        inline char gc() {
            if (p1==p2) p2=(p1=buf)+fread(buf,1,MAXSIZE,stdin);
            return p1==p2?' ':*p1++;
        }
        inline void sync() { fwrite(pbuf,1,MAXSIZE,stdout); pp=pbuf; }
#endif
#ifndef USE_FREAD
        inline void sync() {}
        inline char gc() {  return getchar();  }
#endif
        char floatFormat[10]="%.6f", doubleFormat[10]="%.6lf";
        inline bool blank(char ch) { return ch==' ' or ch=='\n' or ch=='\r' or ch=='\t'; }
        inline bool isd(char x) {return (x>='0' and x<='9');}
        inline IO& setprecision(int d) {
            sprintf(floatFormat, "%%.%df", d); sprintf(doubleFormat, "%%.%dlf", d);
            return *this;
        }
        string input(int reserve=0) {
            char c = gc(); string res=""; res.reserve(reserve);
            while((c&&!blank(c)) || c==' ') {  res.push_back(c); c = gc(); }
            return res;
        }
        template <class T>
        inline void read(T &x) {
            double tmp=1; bool sign=0; x=0; char ch=gc();
            for (; not isd(ch); ch=gc()) if (ch=='-') sign=1;
            for (; isd(ch); ch=gc()) x=x*10+(ch^48);
            if (ch=='.') for (ch=gc(); isd(ch); ch=gc()) tmp*=.1f,x+=tmp*(ch^48);
            if (sign) x=-x;
        }
        inline void read(char *s) {
            char ch=gc();
            for (; blank(ch); ch=gc());
            for (; not blank(ch); ch=gc()) *s++=ch;
            *s=0;
        }
        inline void readln(char *s) {
            char c = gc(); while((c&&!blank(c)) || c==' ') {  *(s++)=c; c = gc();  }
        }
        inline void readln(string &res, int reserve=0) {
            char c = gc(); res.reserve(reserve);
            while((c&&!blank(c)) || c==' ') {  res.push_back(c); c = gc(); }
        }
        inline void read(char &c) {  for (c=gc(); blank(c); c=gc());  }
        inline void read(string &s){
            string().swap(s); char ch=gc();
            for (; blank(ch); ch=gc());
            for (; not blank(ch); ch=gc()) s.push_back(ch);
        }
        template <class T,class... Types> inline void read(T &x,Types &...args){  read(x); read(args...);  }
        template <class T> inline void scan(const T &x) { read(*x); }
        template <class T,class ...Types> inline void scan(const T &x,const Types &...args) {  read(*x); scan(args...);  }
        inline void push(const char &c) {
#ifdef USE_FREAD
            if (pp-pbuf==MAXSIZE) sync();
            *pp++=c;
#endif
#ifndef USE_FREAD
            putchar(c);
#endif
        }
        inline void write(const double x) {
#ifdef USE_FREAD
            if (pp-pbuf>=MAXSIZE-MAX_ITEM_SZ) sync();
            pp += sprintf(pp, doubleFormat, x);
#endif
#ifndef USE_FREAD
            printf(doubleFormat, x);
#endif
        }
        inline void write(const float x) {
#ifdef USE_FREAD
            if (pp-pbuf>=MAXSIZE-MAX_ITEM_SZ) sync();
#endif
#ifndef USE_FREAD
            printf(floatFormat, x);
#endif
        }
        inline void write(const char c) {  push(c);  }
        inline void write(const string &s){  for (auto i:s)  push(i);  }
        inline void write(const char *s){  for (; *s; ++s) push(*s);  }
        template <class T, class = typename std::enable_if_t<std::is_integral_v<T>>>
        inline void write(T x) {
            if (x<0) x=-x,push('-');
            static char sta[40]; int top=0;
            do {  sta[top++]=x%10^48,x/=10;  } while (x);
            while (top) push(sta[--top]);
        }
        template <class T,class... Types>  inline void write(const T &x,const Types &...args){ write(x); write(' '); write(args...); }
        template <class... Types> inline void writeln(const Types &...args){  write(args...); write('\n');  }
        template<class T=int> inline T get() {  T x; read(x); return x;  }
        // 流式输入输出
        template <class T> inline IO& operator>>(T&x) {  read(x); return *this; }
        template <class T> inline IO& operator<<(const T&x) {  write(x); return *this; }
    };
    IO
#ifdef USE_FREAD
    <1048576>
#endif
    io;
    const char endl[] = "\n";

}
#define rg std::ranges

using namespace lib;
using std::vector;
using std::string;

;
namespace Solution {
    int maxCardId = 0;
    enum Character {
        _Undefined = '\0',
        _Questionable = '?',    // 类反猪
        F_Thief = 'F',          // 反猪
        M_Master = 'M',         // 主猪
        Z_Minister = 'Z',       // 忠猪
    };
    const vector<Character> Character_all = {_Undefined, _Questionable, M_Master, Z_Minister, F_Thief};
    Character reversedCharacterType(Character original) {
        if (original == M_Master)  return M_Master;
        if (original == Z_Minister)  return F_Thief;
        if (original == F_Thief)  return Z_Minister;
        return _Undefined;
    }
    Character operator! (const Character& original) { return reversedCharacterType(original); }

    enum DamageType {
        Undefined, 
        DuelingFailed,  // 决斗失败
        Invading,       // 南猪入侵
        Shooting,       // 万箭齐发
        Dueling,        // 决斗开始
        Killing,        // 杀
    };

    namespace Label {
        enum Label {
            P_Peach = 'P',
            K_Killing = 'K',
            D_Dodge = 'D',
            Z_Crossbow = 'Z',
            F_Dueling = 'F',
            N_Invasion = 'N',
            W_Arrows = 'W',
            J_Unbreakable = 'J',
        };
    }


    class Player;
    Player* master;
    int thiefCount = 0;
    Character winner = _Undefined;

    class Damage {
    public:
        Player& source;
        int amount;
        DamageType type;
        Damage(Player& source, int amount, DamageType type = Undefined): 
            source(source), amount(amount), type(type) {}
    };

    class Card {
    public:
        typedef std::function<void(Card)> CardCallbackFunc;
        Player* owner;
        int id;
        char label;

        Card(Player* owner, int id, char label): 
            owner(owner), id(id), label(label) {}
        Card(Player* owner, char label): owner(owner), label(label) { id=maxCardId++; }
        Card(char label): owner(nullptr), label(label) { id=maxCardId++; }
        Card(const Card& other): owner(other.owner), id(other.id), label(other.label) {}

        bool operator == (const Card& other) const { return id==other.id; }
        bool operator == (const char c) const { return label==c; }
        
        operator string() const;
        bool apply();
        void abandon();

        Card* copy(Player* new_owner = nullptr) const { return new Card(new_owner, label); }
    };

    
    std::queue<Card> cardQueue;  // 牌堆
    std::unordered_map<char, std::function<bool(Card)>> cardActions;

    Card lootCard() {
        auto top = cardQueue.front();
        if (cardQueue.size() >= 2)  cardQueue.pop();
        return top;
    }

    template <class T, T& container>
    class RollingIndex {
        void _next() {
            assert(index < (int)container.size());
            index++;
            if (index>=(int)container.size())  index=0;
        }
    public:
        int index = 0;
        RollingIndex(int x) { index = x; }
        RollingIndex next() {
            auto res = *this;
            do { res._next(); } while (container.at(res.index)->dead);
            // printf("!!!%d -> %d\n", index, res.index);
            return res;
        }
        int operator - (const RollingIndex& other) const {
            int dis=0;
            for (auto tmp=other; tmp!=*this; tmp++, dis++);
            return dis;
        }
        bool operator == (const RollingIndex& other) const { return index == other.index; }
        bool operator != (const RollingIndex& other) const { return not (other == *this); }
        void operator ++ () { *this = next(); }
        operator int () {
            return index;
        }
    };

    const int MAX_STRENGTH = 4;
    vector<Player*> players;
    class Player {
    public:
        Character character;
        Character impression = _Undefined;
        int strength = MAX_STRENGTH;
        std::vector<Card*> cards;
        RollingIndex<decltype(players), players> position = 0;
        bool withWeapon = false;
        bool dead = false;

        Player(Character character): character(character), impression(_Undefined),
            strength(MAX_STRENGTH), cards(), position(0), dead(false) {}
        Player(char typeChar): impression(_Undefined),
            strength(MAX_STRENGTH), cards(), position(0), dead(false) {
            character = _Undefined;
            for (const auto ch: Character_all) {
                if (ch == typeChar)  character = ch;
            }
        }

        Card* findCard(const char label) const { 
            auto ptr = rg::find_if(cards, lam(ptr, ptr->label==label)); 
            if (ptr == cards.end())  return nullptr;
            return *ptr;
        }

        auto getCard(const Card& templ) {
            auto copy = templ.copy(this);
            cards.push_back(copy);
        }

        void inDueling(Player& enemy) {
            log("in dueling\n")
            // 忠猪不会打主猪
            if (enemy.character == M_Master and character == Z_Minister) {
                damaged({enemy, 1, DuelingFailed});
                return;
            }
            // 弃置一张杀
            auto toBeAbandoned = findCard(Label::K_Killing);
            if (not toBeAbandoned) {  // 没有杀可以弃置
                damaged({enemy, 1, DuelingFailed});  // 决斗失败
            } else {
                toBeAbandoned->abandon();  // 弃置
                enemy.inDueling(*this);  // 轮到对面
            }
        }


        /**
         * 当前玩家受到一点伤害
         * ! 调用此方法之后，胜利状态可能会改变，需要进行判断
         */ 
        void damaged(const Damage& obj) {
            assert(not dead);
            log("Damaged P%d<-%d. Strength %d -= %d.\n", index(), obj.source.index(), strength, obj.amount)
            // 尝试减免伤害，否则正常造成伤害
            if (obj.type == Killing) {
                Card *dodge = findCard(Label::D_Dodge);  // 尝试使用“闪”
                if (dodge)  dodge->abandon();
                else  strength -= obj.amount;  // 无法躲避伤害
            } else if (obj.type == Invading) {  // 南猪入侵
                auto killing = findCard(Label::K_Killing);  // 弃置杀
                if (killing)  killing->abandon();
                else  strength -= obj.amount;  // 无法躲避伤害
            } else if (obj.type == Shooting) {  // 万箭齐发
                auto dodging = findCard(Label::D_Dodge);  // 弃置闪
                if (dodging)  dodging->abandon();
                else  strength -= obj.amount;  // 无法躲避伤害
            } else {
                strength -= obj.amount;  // 无法躲避伤害
            }

            // 没有血了，可以被动地吃桃救一下
            while (strength <= 0) {
                auto peach = findCard(Label::P_Peach);
                if (not peach)  break;  // 桃也没了
                else  peach->apply();
            }

            // 判定是否死亡
            if (strength <= 0) {
                dead = true;
            }

            if (character == M_Master) {
                if (dead)  winner = F_Thief;  // 主猪死亡，反方胜利

                // 判定“类反猪”
                if (obj.source.impression == _Undefined and obj.type > DuelingFailed)  //* 决斗失败不算
                    obj.source.impression = _Questionable;
            }

            bool hostility = (obj.type >= Dueling);  // 表敌意

            if (hostility) {  // 判断跳反或跳忠
                chkMax(obj.source.impression, !characterTypeAfterSquiring());
            }
            
            if (character == F_Thief and dead) {  // 判断反贼数量和主猪胜利
                thiefCount--;
                if (thiefCount <= 0)  winner = M_Master;
            }


            if (dead) {  // 奖励和惩罚
                if (character == F_Thief) {
                    // 如果提前结束则不予发放奖励
                    if (winner != _Undefined)  return;
                    // 摸三张牌
                    obj.source.getCard(lootCard());
                    obj.source.getCard(lootCard());
                    obj.source.getCard(lootCard());
                } else if (character == Z_Minister and obj.source.character == M_Master) {
                    // 需要先执行完惩罚
                    obj.source.cards.clear();
                    obj.source.withWeapon = false;
                }
            }
        }

        /**
         * 判断对当前玩家献殷勤属于跳忠还是跳反
         * 即：对当前玩家献殷勤之后，会让自己的 impression 变成什么
         * 如果要判断表敌意，对结果取反即可。
         */
        Character characterTypeAfterSquiring() const {
            // 跳忠
            if (character == M_Master or impression == Z_Minister)  return Z_Minister;
            // 跳反
            if (impression == F_Thief)  return F_Thief;
            return _Undefined;
        }

        Character team() const {
            if (character == M_Master or character == Z_Minister)  return F_Thief;
            if (character == F_Thief)  return Z_Minister;
            return _Undefined;
        }

        // 亮出身份
        void showCharacter() {
            // 不是很确定亮出主猪的身份会不会使其他代码出问题
            if (character != M_Master)  impression = character;
        }

        // 摸牌阶段
        void drawCards() {
            getCard(lootCard());
            getCard(lootCard());
        }

        int index() const {
            int idx = rg::find_if(players, lam(pt, pt==this)) - players.begin();
            if (idx > (int)players.size())  return -1;
            else  return idx;
        }

        /**
         * 进行该玩家的一个回合
         * 返回 是否可以继续游戏
         */
        bool round() {
            drawCards();

            // 如果使用了武器猪哥连弩，没有杀的次数限制
            int usedKillingCard = withWeapon? -Infinity: 0;
            // 每次打完一张牌，都从头开始扫，判断最左侧的可用牌
            // * 注意这里不可以一路扫下去
            auto i = 0;
            while (i < (int)cards.size()) {
                for (i=0; i<(int)cards.size(); i++) {
                    auto& card = *cards[i];
                    bool isKillingCard = (card.label == Label::K_Killing);
                    if (isKillingCard and usedKillingCard >= 1) {
                        continue;  // 不能再次使用杀
                    }
                    if (card.apply()) {
                        if (isKillingCard)  usedKillingCard++; 
                        break;  // 跳到 while 循环里重新开始
                    }
                }
                // 如果胜负确定，强制中断游戏
                if (winner != _Undefined)  return false;  
                if (dead)  return true;  // 如果在回合中途被打死，强制结束回合
                // 如果循环正常结束，i>=(int)cards.size()，结束 while 循环
            }
            return true;
        }
    };

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
    bool tryExecutingUnbreakable(const Card& before, const Player& original, 
                decltype(Player::position) pos, bool tag=false, int depth=0) {

        log("[%d]tryUnbreakable, before=%c#%d, original=P%d, pos=%d, tag=%d\n", depth, before.label, before.id, 
            original.index(), pos.index, tag);
        if (original.impression < F_Thief and original.character != M_Master) { // 没有亮身份，帮不了
            log("Break! -> 0\n");
            return false; 
        }
        // 使用无懈可击抵消献殷勤：表敌意
        // 使用无懈可击抵消表敌意：献殷勤
        // 使用无懈可击抵消决斗、南猪入侵、万箭齐发：献殷勤
        bool friendly = false;  // 这次无懈可击为是否为对 original 献殷勤
        if (before.label == Label::F_Dueling or before.label == Label::N_Invasion or before.label == Label::W_Arrows) {
            friendly = true;
        } else {  // 对于上一次无懈可击
            friendly = !tag; // 如果上一次是在献殷勤，这次就是在表敌意；反之亦然。
        }
        log("friendly = %d\n", friendly);

        // 接下来，给定所有人使用无懈可击的机会
        auto p = pos;
        do {
            log("Checking p=%d; ", p.index);
            auto& pl = *players.at(p);
            Card* cardPtr = pl.findCard(Label::J_Unbreakable);
            auto insertPos = rg::find_if(pl.cards, lam(ptr, ptr->id == cardPtr->id));
            if (not cardPtr)  goto egg;  // 没有无懈可击，不可使用

            // 把这张牌先去掉，避免重复选中
            pl.cards.erase(insertPos);
            
            if (friendly) {
                auto character = original.team();
                if (character == pl.team()) {  // 相同阵营，向对方献殷勤
                    cardPtr->abandon();  // 无懈可击
                    pl.showCharacter();
                    // 如果这次无懈可击没有被无效化，返回 true
                    bool res = not tryExecutingUnbreakable(*cardPtr, original, p, friendly, depth+1);  
                    if (res == true)  log("This is unbreakable. \n")
                    return res;
                }
            } else {  // 通过无懈表敌意
                auto character = !original.team();
                if (character == pl.team()) {  // 阵营正确
                    cardPtr->abandon();  // 无懈可击
                    pl.showCharacter();
                    // 如果这次无懈可击没有被无效化，返回 true
                    bool res = not tryExecutingUnbreakable(*cardPtr, original, p, friendly, depth+1);  
                    if (res == true)  { log("This is unbreakable. -> 1\n") }
                    else  { log("Done. This is NOT unbreakable. -> 0 \n") }
                    return res;
                }
            }

            // 重新加入这张牌
            pl.cards.insert(insertPos, cardPtr);

        egg:    // while 循环直接 continue 会炸，故致敬传奇 goto egg
            ++p;
        } while (p != pos);
        // 没有人选择使用无懈可击
        return false;  
    }

    template <class List_t>
    Player& joinPlayer(const Player& member, List_t lis = players) {
        lis.push_back(member);
        Player& added = lis.back();
        added.position = (int)lis.size()-1;
        return added;
    }

    Card::operator string () const {
        return string(1, label);
    }

    /**
     * 使用一张牌。
     * !如果使用成功，自动弃置。
     * 返回 是否使用成功。
     */
    bool Card::apply() {
        // log("\nOwner %lld\n", rg::find_if(players, lam(pt, pt==owner)) - players.begin());
        // log("Apply label: %c #%d?\n", label, id)

        bool success = cardActions[label](*this);
        if (not success)  return false;
        // log("Apply label: %c #%d - Success\n", label, id)
        from(i, 0, (int)players.size()-1)  log("P%d: %dHP %cCH %cIMP\n",i,players[i]->strength, players[i]->character, (char)players[i]->impression);
        abandon();
        return true;
    }

    // 弃置一张卡牌
    void Card::abandon() {
        log("Abandon Card %c #%d\n", label, id);
        auto ptr = std::find(owner->cards.begin(), owner->cards.end(), this);  // 玩家手牌
        if (ptr != owner->cards.end()) {  // 如果存在指向这张牌的指针
            owner->cards.erase(ptr);  // 删除指向自己的指针
        }
        delete this;
    }


    // 测试
    bool T_Test(const Card card) {
        io << "test " << card.id << endl;
        return true;
    }

    // 桃
    bool P_Peach(const Card card) {
        if (card.owner->strength >= MAX_STRENGTH)  return false;  // 已经满血使用失败
        card.owner->strength++;  // 回一点血
        return true;
    }

    // 杀
    bool K_Killing(const Card card) {
        Player& target = *players.at(card.owner->position.next());  // 目标
        auto imp = target.impression;
        auto user = card.owner->character;
        bool execute = false;
        if (user == M_Master) {
            if (imp == _Questionable or imp == F_Thief)  execute = true;
        } else if (user == Z_Minister) {
            if (imp == F_Thief)  execute = true;
        } else {
            if (imp == Z_Minister or target.character == M_Master)  execute = true;
        }
        if (not execute)  return false;
        target.damaged({*card.owner, 1, Killing});
        return true;
    }

    // 闪
    bool D_Dodge(const Card card) {
        return (card.label != Label::D_Dodge);  // 不能使用
    }

    // 猪哥连弩
    bool Z_Crossbow(const Card card) {
        card.owner->withWeapon = true;
        return true;
    }

    // 决斗
    bool F_Dueling(const Card card) {
        auto user = card.owner->character;
        auto target = card.owner->position;
#define cur players[target]
        if (user == M_Master) {  // 主猪打反猪和类反猪
            for (++target; cur->impression!=F_Thief and cur->impression!=_Questionable
                and target!=card.owner->position; ++target)
                    log("duel.cpp - Checking %d with %d\n", target.index, card.owner->position.index);
                
        } else if (user == Z_Minister) {  // 忠猪打反猪
            for (++target; cur->impression!=F_Thief and target!=card.owner->position; ++target)
                    log("duel.cpp - Checking %d with %d\n", target.index, card.owner->position.index);
        } else {  // 反猪优先打主猪
            for (++target; cur->character!=M_Master and target!=card.owner->position; ++target)
                    log("duel.cpp - Checking %d with %d\n", target.index, card.owner->position.index);
        }

        log("duel.cpp - %d %d\n", target.index, card.owner->position.index);
        if (target == card.owner->position)  return false;
        cur->damaged({*card.owner, 0, Dueling});  // 触发决斗，用于判定跳反
        if (not tryExecutingUnbreakable(card, *cur, card.owner->position.next(), false))
            cur->inDueling(*card.owner);
        log("duel.cpp - returns true\n");
        return true;
#undef cur
    }

    // 南猪入侵
    bool N_Invasion(const Card card) {
        auto target = card.owner->position;
#define cur players[target]
        bool canceled = false;
        for (++target; target!=card.owner->position; ++target) {
            canceled = tryExecutingUnbreakable(card, *cur, card.owner->position, false);
            if (canceled)  continue;
            log("Invasion target = %d\n", target.index);
            cur->damaged({*card.owner, 1, Invading});
            if (winner != _Undefined) {  // 有人获胜就跳出
                return true;
            }
        }
#undef cur
        return true;
    }

    // 万箭齐发
    bool W_Arrows(const Card card) {
        auto target = card.owner->position;
#define cur players[target]
        bool canceled = false;
        for (++target; target!=card.owner->position; ++target) {
            canceled = tryExecutingUnbreakable(card, *cur, card.owner->position, false);
            if (canceled)  continue;
            log("Arrows target = %d\n", target.index);
            cur->damaged({*card.owner, 1, Shooting});
            if (winner != _Undefined) {  // 有人获胜就跳出
                return true;
            }
        }
#undef cur
        return true;
    }

    // 无懈可击
    bool J_Unbreakable(const Card card) {
        return card.label != Label::J_Unbreakable;  // 无懈可击永远不会先手使用
    }


    void initCardTemplates() {
        cardActions.insert({'T', T_Test});
        cardActions.insert({Label::P_Peach, P_Peach});
        cardActions.insert({Label::K_Killing, K_Killing});
        cardActions.insert({Label::D_Dodge, D_Dodge});
        cardActions.insert({Label::Z_Crossbow, Z_Crossbow});
        cardActions.insert({Label::F_Dueling, F_Dueling});
        cardActions.insert({Label::N_Invasion, N_Invasion});
        cardActions.insert({Label::W_Arrows, W_Arrows});
        cardActions.insert({Label::J_Unbreakable, J_Unbreakable});
    }

    int N_PlayerCount, M_CardCount;
    void init() {
        initCardTemplates();

        io >> N_PlayerCount >> M_CardCount;
        from(_, 1, N_PlayerCount) {
            char typeChar, ignore;
            io >> typeChar >> ignore;
            players.push_back(new Player(typeChar));
            auto& pl = *players.back();
            pl.position = players.size()-1;
            if (pl.character == Character::M_Master)  master = &pl;
            else if (pl.character == Character::F_Thief)  thiefCount++;
            from(i, 0, 3) {
                pl.getCard(Card(io.get<char>()));
            }
        }
        from(_, 1, M_CardCount)  cardQueue.push(Card(nullptr, io.get<char>()));
    }

    /**
     * 进行一个回合。
     * 返回 游戏是否可以继续
     */
    bool round() {
        for (auto &player: players) {
            if (player->dead)  continue;
            if (player->round() == false) {
                return false;
            }
        }
        return true;
    }

    void solve() {
        init();
        while (round());

        // 输出结果
        io << (char)winner << 'P' << endl;
        for (auto &pl: players) {
            if (pl->dead) {
                io.writeln("DEAD");
            } else {
                for (auto &cd: pl->cards)  io << cd->label << ' ';
                io << endl;
            }
        }
    }
}


int main() {;

    initDebug;
    Solution::solve();

    return 0;
}
/*
2 10
MP P K K K
FP K P P P
K K K K K K K K K K K 
*/