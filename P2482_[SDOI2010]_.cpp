/**
 * 
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
        M_Master = 'M',         // 主猪
        Z_Minister = 'Z',       // 忠猪
        F_Thief = 'F',          // 反猪
    };
    const vector<Character> Character_all = {_Undefined, M_Master, Z_Minister, F_Thief};

    enum DamageType {
        Undefined, 
        Invading,       // 南猪入侵
        Shooting,       // 万箭齐发
        Dueling,        // 决斗
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
        
        operator string() const;
        bool apply();
        void abandon();

        Card copy(Player* new_owner) const { return Card(new_owner, label); }
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
        std::vector<Card> cards;
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

        auto getCard(const Card& templ) {
            auto copy = templ.copy(this);
            cards.push_back(copy);
        }

        void inDueling(Player& enemy) {
            log("in dueling\n")
            if (enemy.character == M_Master and character == Z_Minister) {
                damaged({enemy, 1, Dueling});
                return;
            }
            // 弃置一张杀
            auto toBeAbandoned = rg::find_if(cards, lam(c, c.label==Label::K_Killing));
            if (toBeAbandoned == cards.end()) {  // 没有杀可以弃置
                damaged({enemy, 1, Dueling});
            } else {
                cards.erase(toBeAbandoned);
                // log("size %d\n", (int)cards.size())
                enemy.inDueling(*this);
            }
        }

        void damaged(const Damage& obj) {
            assert(not dead);
            // 尝试避免伤害，否则正常造成伤害
            if (obj.type == Killing) {
                auto dodge = rg::find_if(cards, lam(c, c.label==Label::D_Dodge));  // 尝试使用“闪”
                if (dodge != cards.end())  dodge->abandon();
                else  strength -= obj.amount;  // 无法躲避伤害
            } else if (obj.type == Invading) {  // 南猪入侵
                auto killing = rg::find_if(cards, lam(c, c.label==Label::K_Killing));  // 弃置杀
                if (killing != cards.end())  killing->abandon();
                else  strength -= obj.amount;  // 无法躲避伤害
            } else if (obj.type == Shooting) {  // 万箭齐发
                auto dodging = rg::find_if(cards, lam(c, c.label==Label::D_Dodge));  // 弃置闪
                if (dodging != cards.end())  dodging->abandon();
                else  strength -= obj.amount;  // 无法躲避伤害
            } else {
                strength -= obj.amount;  // 无法躲避伤害
            }

            // 判定是否死亡
            if (strength <= 0)  dead = true;

            // 判定“类反猪”
            if (character == M_Master) { 
                if (dead)  winner = F_Thief;
                if (obj.source.impression == _Undefined)
                    obj.source.impression = _Questionable;
            }

            bool hostility = (obj.type >= Dueling);  // 表敌意

            if (hostility and (character == M_Master or impression == Z_Minister)) { // 跳反
                obj.source.impression = F_Thief;
            }
            if (hostility and impression == F_Thief) {  // 跳忠
                obj.source.impression = Z_Minister;
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

        // 摸牌阶段
        void drawCards() {
            getCard(lootCard());
            getCard(lootCard());
        }

        /**
         * 进行该玩家的一个回合
         * 返回 是否可以继续游戏
         */
        bool round() {
            drawCards();

            // 如果使用了武器猪哥连弩，没有杀的次数限制
            int usedKillingCard = withWeapon? -Infinity: 0;  
            for (auto i=0; i!=(int)cards.size(); i++) {
                auto& card = cards[i];
                bool isKillingCard = (card.label == Label::K_Killing);
                if (isKillingCard and usedKillingCard >= 1) {
                    continue;  // 不能再次使用杀
                }
                if (card.apply()) {  //! 下文中不要再访问 cards[i]
                    i--;  // 移除了元素，回退指针
                    if (isKillingCard)  usedKillingCard++;
                }
                // 如果胜负一定，强制中断游戏
                if (winner != _Undefined)  return false;  
                if (dead)  return true;  // 如果在回合中途被打死，强制结束回合
            }
            return true;
        }

    };

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
        log("Owner %lld\n", rg::find_if(players, lam(pt, pt==owner)) - players.begin());
        log("Apply label: %c?\n", label)
        log("ASCII %d", (int)label);
        bool success = cardActions[label](*this);
        if (not success)  return false;
        log("Apply label: %c - Success\n", label)
        log("Health %d %d %d\n", players[0]->strength, players[1]->strength, players[2]->strength);
        abandon();
        return true;
    }

    // 弃置一张卡牌
    void Card::abandon() {
        auto ptr = std::find(owner->cards.begin(), owner->cards.end(), *this);
        if (ptr != owner->cards.end())
            owner->cards.erase(ptr);
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
                    log("duel.cpp - %d\n", target.index);
        } else if (user == Z_Minister) {  // 忠猪打反猪
            for (++target; cur->impression!=F_Thief and target!=card.owner->position; ++target);
        } else {  // 反猪优先打主猪
            for (++target; cur->character!=M_Master and target!=card.owner->position; ++target);
        }

        log("duel.cpp - %d %d\n", target.index, card.owner->position.index);
        if (target == card.owner->position)  return false;
        cur->inDueling(*card.owner);
        log("duel.cpp - returns true\n");
        return true;
#undef cur
    }

    // 南猪入侵
    bool N_Invasion(const Card card) {
        auto target = card.owner->position;
#define cur players[target]
        for (++target; target!=card.owner->position; ++target)
            cur->damaged({*card.owner, 1, Invading});
#undef cur
        return true;
    }

    // 万箭齐发
    bool W_Arrows(const Card card) {
        auto target = card.owner->position;
#define cur players[target]
        for (++target; target!=card.owner->position; ++target)
            cur->damaged({*card.owner, 1, Shooting});
#undef cur
        return true;
    }

    // 无懈可击
    bool J_Unbreakable(const Card card) {
        return card.label == Label::J_Unbreakable;
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
                for (auto &cd: pl->cards)  io << cd.label << ' ';
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