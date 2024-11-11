/**
 * @link https://www.luogu.com.cn/problem/P7610
 */

#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define from(i,b,e) for(int i=(b);i<=(e);i++)
#define rev(i,e,b) for(int i=(e);i>=(b);i--)
#define main() main(int argc, char const *argv[])
#define chkMax(base,cmp...) (base=std::max({(base),##cmp}))
#define chkMin(base,cmp...) (base=std::min({(base),##cmp}))
#define never if constexpr(0)
#define always if constexpr(1)
#define bitOr(x,y) (((x)&(y))^(((x)^(y))|(~(x)&(y))))
#define Infinity 2147483647
bool DEBUG_MODE=false;
typedef long long ll; typedef unsigned long long ull;

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
            return p1==p2?'\0':*p1++;
        }
        inline void sync() { fwrite(pbuf,1,MAXSIZE,stdout); pp=pbuf; }
#endif
#ifndef USE_FREAD
        inline void sync() {}
        inline char gc() {  return getchar();  }
#endif
        char floatFormat[10]="%.6f", doubleFormat[10]="%.6lf";
        inline bool blank(char ch) { return ch<=32 or ch==127; }
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
            char c = gc(); while((c&&!blank(c)) || c==' ') {  *(s++)=c; c = gc();  } *s=0;
        }
        inline void readln(string &res, int reserve=0) {
            char c = gc(); string().swap(res); res.reserve(reserve);
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
            pp += sprintf(pp, floatFormat, x);
#endif
#ifndef USE_FREAD
            printf(floatFormat, x);
#endif
        }
        inline void write(const char c) {  push(c);  }
        inline void write(const string &s){  for (auto i:s)  push(i);  }
        inline void write(const char *s){  for (; *s; ++s) push(*s);  }
        template <class T
#if __cplusplus > 201403L
        , class = typename std::enable_if_t<std::is_integral_v<T>>
#endif
        >
        inline void write(T x) {
            static char sta[40]; int top=0;
            if (x<0) {
                push('-'),sta[top++]=(-(x%10))^48,x=-(x/10);
                if (x==0) { push(sta[--top]); return; }
            }
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
    const char endl = '\n';

}

using namespace lib;

#define rgs std::ranges

/**
 * “提取”一个容器中的前 SZ 项，其中 SZ 为编译期常量。
 * 提取后的项可以用于结构化绑定。
 * ! 修改提取后的项不会影响到原元素。
 */
namespace Extractions {
    template <class T, size_t SZ>  requires (SZ>0)
    struct _Extraction {  T container;  };

    template <size_t N, class Ext>
    auto get(const Ext& c) {
        return c.container[N];
    }

    /**
     * 提取容器 con 的前 SZ 项，其中 SZ 为模板参数。
     * 用法示例：
     * std::vector<int> vec = {0, 1, 2, 3, 4};
     * auto [a, b, c] = extract<3>(vec);
     * -> a = 0, b = 1, c = 2
     * 在这样操作后，对这些变量的修改**不会**同步到原容器，反之亦然
     */
    template <size_t SZ, class Container_T>
    auto extract(Container_T& con) {
        assert(con.size());
        return _Extraction<Container_T, SZ> {con};
    }
}
using Extractions::get;

namespace std {
    // 定义接口，用于实现 Extractions::_Extraction 的结构化绑定。
    template <class T, size_t SZ>
    struct tuple_size<Extractions::_Extraction<T, SZ>> { const static int value = SZ; };
    template <size_t N, class T, size_t SZ>
    struct tuple_element<N, Extractions::_Extraction<T, SZ>> {
        using type = decltype(std::move(std::declval<T>()[0]));
    };
}


namespace StellarisConnection {
    struct Character;
    struct Player;

    int N;  // 每个玩家的角色数量

    // 维护技能的参数列表
    struct Arguments: std::vector<int> {
        Arguments(): std::vector<int>() {}
        Arguments(const std::initializer_list<int>& lis): std::vector<int>(lis) {}
        template <size_t SZ>
        auto take() {
            return Extractions::_Extraction<Arguments, SZ>{*this};
        }
    };

    enum TalentType {
        WithDepression,             // 我自闭了 - 无特殊效果
        FleshSkin,                  // 血肉皮囊 - 免疫 50% 真实伤害
        GalaxyPowerReflection,      // 星河力量投射 - 普攻附带真实伤害
        MindBetterThanMatter,       // 心胜于物 - 回合结束回复额外生命值和能量值
        Transcendent,               // 超凡入圣 - 普通攻击转换为真实伤害
        TechnologyFirst,            // 科技至上 - 普攻获得生命值，技能获得能量值
    };
    auto TalentTypeLast = TechnologyFirst, TalentTypeFirst = WithDepression;

    enum SkillType {
        MentalityCollapsed,         // 心态崩了！ - 无特殊技能
        GreensExplosion,            // 格林炸裂！ - 对敌方所有角色造成伤害和扣除能量
        ICBM,                       // 日昇之雨！（洲际导弹）- 对地方所有角色造成真实伤害
        EmperorsSwordInSkyPalace,   // 天楼霸断剑！ -  基于对方最大生命值，对所有敌方单位造成一定伤害
        ShowBeginning,              // 演出开始！ - 几个回合之内，己方行动结束时额外回复能量
        SiriusSlash,                // 天狼噬斩！ - 减小有限目标的防御力增益，然后对其造成真实伤害
        SuperLightning,             // 地球蓝色闪电！ - 对优先目标造成真实伤害；一段时间内减小敌方攻击力增益
        AuroraBloom,                // 极光绽放！ - 己方生命最低的一个角色回复一定生命值；一段时间内增加己方攻击力增益
        Meteor,                     // 流星！ - 对敌方所有角色造成伤害；在此之后一段时间内减小敌方防御力增益
        ElfProtection,              // 精灵庇护！ - 己方所有角色回复生命值；一段时间内增大己方防御力增益
        // 全力超全开！轮回之终末！ - 略
        FullPowerInTheEndOfReincarnation,
    };
    auto SkillTypeLast = FullPowerInTheEndOfReincarnation, SkillTypeFirst = MentalityCollapsed;

    struct Talent {
        TalentType type;
        Arguments args;

        bool operator== (TalentType t) const { return t == type; }
    };

    namespace Process {  // 游戏进程相关
        enum StatusType {
            Begin,          // 回合开始
            Action,         // 行动
            ActionDone,     // 行动结束
            End,            // 回合结束
        };

        void operator ++ (StatusType& type, int) {
            if (type >= End || type < Begin)  type = Begin;
            else  type = (StatusType)(type + 1); 
        }

        struct Round {
            int id = 0;
            StatusType type = Begin;
            int arg = 0;  // 用于标明哪一方的行动轮
            int teams;  // 队伍数量

            Round endOf(int id) {
                return Round(id, End, teams-1, teams);
            }

            static Round unreachable() {
                return Round(Infinity, End, Infinity, 0);
            }

            Round &next() {
                if (type == End)  arg = 0, id++;
                if (type == ActionDone and arg < teams - 1)  type = Action, arg++;
                else  type++;
                debug {
                    io << std::format("Round {} {} {} {}", id, arg, (int)type, teams) << endl;
                }
                return *this;
            }

            Round &operator++(int) {
                return next();
            }

            std::weak_ordering operator <=> (const Round& other) const {
                if (auto cmp = id<=>other.id; cmp != 0)  return cmp;
                if (auto cmp = arg<=>other.arg; cmp != 0)  return cmp;
                return type<=>other.type;
            }
        } current;
    }
    using Process::current;

    

    struct Skill {
        SkillType type;
        Arguments args;

        bool operator== (SkillType t) { return t == type; }
    };

    struct Limitation {
        int min = -Infinity, max = Infinity;

        Limitation limitMax(const int max) { return {-Infinity, max}; }
        Limitation limitMin(const int min) { return {min, +Infinity}; }

        int limited(const int val) const {
            return std::min(max, std::max(min, val));
        }

        int operator() (const int val) const { return limited(val); }
    };

    struct Attribute {  // 有限制的属性值
        int value;
        Limitation limit;

        operator int () const {
            return limit(value);
        }

        int operator * () const { return (int)*this; }

        Attribute& operator = (const int val) {
            value = limit(val);
            return *this;
        }

        bool full() {
            return value == limit.max;
        }
    };

    struct Modifier {
        int amount;
        Process::Round expireTime;      // 将在比这个时间更晚时过期
        
        bool expired() const { return current > expireTime; }
        operator int () const {
            return expired()? 0: amount;
        }
    };

    auto operator + (int a, const Modifier& b) { return a + (int)b; }

    struct BoostedAttribute{  // 有“增益”设定的属性值
        int value;
        Limitation limit;
        std::vector<Modifier> boost;

        operator int () const {
            return limit(std::accumulate(boost.begin(), boost.end(), value));
        }
        int operator * () const { return (int)*this; }

        BoostedAttribute& operator = (const int val) {
            value = limit(val);
            return *this;
        }
    };


    struct Damage {
        int amount;         // 伤害数值（未计算伤害减免）
        int truth;          // 附带的真实伤害（未计算伤害减免）
        Character *source;     // 伤害来源
        Character *target;     // 目标
    };

    struct Player {
        std::vector<Character*> characters;
        Player *enemy;  // 对手
        int id;
        Process::Round deadline = Process::Round::unreachable();  // 获胜期限

        /**
         * 开始当前玩家的行动轮。
         */
        void action();

        /**
         * 进行该玩家的收尾。
         */
        void actionDone();

        /**
         * 检查玩家是否失败
         */
        bool lose();

        auto aliveCharacters();
    };

    std::vector<Player*> players;

    struct Character {
        int id;
        Attribute health;           // 生命值
        Attribute mana;             // 能量值
        BoostedAttribute attack;    // 攻击力
        BoostedAttribute defense;   // 防御力

        BoostedAttribute extraMana;  // 额外回复能量值

        Player* owner;

        Talent talent;      // 天赋
        Skill skill;        // 技能


        bool dead = false;          // 是否死亡

        std::deque<Character*> priority_targets;  // 优先目标
        std::deque<int> _priority_targets_index;  // 优先目标的下标（0 开始）

        Character(int HP, int MP, int atk, int def, int id):
            id(id), health({HP, {0, HP}}), mana({0, {0, MP}}), 
            attack({atk, {1, Infinity}, {}}), defense({def, {0, Infinity}, {}}), 
            extraMana({0, {0, Infinity}, {}}), owner(nullptr) {}

        // 获取优先目标
        // 这会自动清理优先目标队列
        Character *target() {
            while (not priority_targets.empty() and priority_targets.front()->dead) {
                priority_targets.pop_front();
            }
            if (not priority_targets.empty()) {
                return priority_targets.front();
            } else {
                throw -8;  // 没有优先目标，抛出异常
                return nullptr;
            }
        }

        int calcDamage(const Damage &dmg) const {
            if (dead)  return 0;
            auto x = dmg.amount, y = dmg.truth;
            auto finally = std::max(0, x-defense) + y;

            if (talent == FleshSkin) {
                finally -= y/2;  // 天赋效果，受到真伤减半
            }

            return finally;
        }

        /**
         * 受到一次伤害。
         * 将会计算伤害的减免（如防御力）。
         * 返回目标生命值减少量。
         */
        int damaged(const Damage &dmg) {
            // 如果已经死亡，不再收到伤害
            if (dead)  return 0;

            auto realAmount = calcDamage(dmg);
            auto healthOriginal = (int)health;
            health.value -= realAmount;
            auto healthDecreasing = healthOriginal - health;

            // 回复一点能量值
            mana = mana + 1;

            debug io << std::format("{} Damaged by {}(amount={},{}) h-={}", (std::string)*this, (std::string)*dmg.source, dmg.amount, dmg.truth, healthDecreasing) << endl;

            if (health <= 0) {
                dead = true;
                if (owner->lose()) {
                    throw owner;  // 游戏结束
                }
            }
            return healthDecreasing;
        }

        void useSkill();

        operator std::string() const {
            return std::format("P{}-C{}", owner->id, id);
        }
    };

    auto Player::aliveCharacters() {
        return characters | rgs::views::filter(lam(ch, not ch->dead));
    }

    void Player::action()  {
        // 选择编号最大的可使用技能
        bool usedSkill = false;
        for (auto sk = SkillTypeLast; sk >= SkillTypeFirst; sk = SkillType((int)sk-1)) {
            auto rev = characters   | rgs::views::filter(lam(ch, not ch->dead))
                                    | rgs::views::filter(lam(ch, ch->mana.full()))
                                    | rgs::views::reverse;
            auto pos = rgs::find_if(rev, lam(cpt, cpt->skill.type == sk));
            if (pos == rev.end())  continue;
            auto ch = *pos;
            ch->useSkill(), usedSkill = true;
            ch->mana = ch->mana + 1;
            break;
        }
        if (not usedSkill) {  // 使用普通攻击
            // 寻找一个角色：其优先目标的生命值最高

            auto virtualDamage = [&](Character *ch) -> int {
                if (ch->talent == Transcendent) {
                    return std::min((int)ch->target()->health, 
                        ch->target()->calcDamage({0, ch->attack, ch, ch->target()}));
                } else {
                    auto attached = 0;
                    if (ch->talent == GalaxyPowerReflection)  attached = ch->talent.args[0];
                    return std::min((int)ch->target()->health, 
                        ch->target()->calcDamage({ch->attack, attached, ch, ch->target()}));
                }
            };

            auto cmp = [&](Character *a, Character *b) -> bool {
                if ((int)a->target()->health != (int)b->target()->health) {
                    return (int)a->target()->health < (int)b->target()->health;
                }
                return virtualDamage(a) < virtualDamage(b);
            };

            auto filtered = characters  
                            | rgs::views::filter(lam(ch, not ch->dead))  // 排除已死亡玩家
                            | rgs::views::reverse;

            auto ch = *rgs::max_element(filtered, cmp);
            // auto ch = *filtered.begin();
            // for (auto &i: filtered) {
            //     io << std::format("cmp with {} and {}: {}", (std::string)*ch, (std::string)*i, cmp(ch, i)) << endl;
            //     if (cmp(ch, i))  ch = i;
            // }
            
            debug io << std::format("{} Normal Attack -> {}", (std::string)*ch, (std::string)*ch->target()) << endl;

            if (ch->talent == Transcendent) {
                ch->target()->damaged({0, ch->attack, ch, ch->target()});
            } else {
                auto attached = 0;
                if (ch->talent == GalaxyPowerReflection)  attached = ch->talent.args[0];
                ch->target()->damaged({ch->attack, attached, ch, ch->target()});
            }

            ch->mana = ch->mana + 1;

            if (ch->talent == TechnologyFirst) {
                auto x = ch->talent.args[0];
                ch->health = ch->health + x;
            }
        }
    }

    void Player::actionDone() {
        for (auto &ch: aliveCharacters()) {
            // 回复能量值
            auto manaIncreasing = 1 + (int)ch->extraMana;
            if (ch->talent == MindBetterThanMatter) {
                manaIncreasing += ch->talent.args[1];  // 天赋回能
            }
            ch->mana = ch->mana + manaIncreasing;

            // 回复生命值
            if (ch->talent == MindBetterThanMatter) {
                auto healthIncreasing = ch->talent.args[0];  // 回生命
                ch->health = ch->health + healthIncreasing;
            }
        }

        debug {
            io << "-----" << endl;
            for (auto &pl: players) {
                io << 'P' << pl->id << ' ';
                for (auto &ch: pl->characters) {
                    io << std::format("C{}(HP{}/{}, MP{}/{}, atk{}, def{}, target{}, dead{})", ch->id, (int)ch->health, ch->health.limit.max, (int)ch->mana, ch->mana.limit.max, (int)ch->attack, (int)ch->defense, (std::string)*ch->target(), (int)ch->dead) << ' ';
                }
                io << endl;
            }
            io << "-----" << endl;
        }
    }

    void Character::useSkill() {
        auto t = current.id;  // 当前游戏进度
        debug {
            io << std::format("P{}-C{}: useSkill {} with args ", owner->id, id, (int)skill.type);
            for (auto &arg: skill.args) {
                io << arg << ' ';
            }
            io << endl;
        }
        // 清空能量值
        mana = 0;
        if (skill == GreensExplosion) {
            auto dmg = skill.args[0];  // 伤害数值
            for (auto &ch: owner->enemy->aliveCharacters()) {
                ch->damaged({dmg, 0, this, ch});  // 受到伤害
                mana = mana - (mana/10);  // 减少能量
            }
        } else if (skill == ICBM) {
            for (auto &ch: owner->enemy->aliveCharacters()) {
                ch->damaged({0, attack, this, ch});
            }
        } else if (skill == EmperorsSwordInSkyPalace) {
            auto x = skill.args[0];
            for (auto &ch: priority_targets) {
                if (not ch->dead) {  // 攻击所有存活的目标
                    auto dmg = std::min(attack * x, ch->health.limit.max / 10);
                    ch->damaged({dmg, 0, this, ch});
                }
            }
        } else if (skill == ShowBeginning) {
            auto [x, y] = skill.args.take<2>();
            auto expire = current.endOf(t+x-1);  // 到这个回合的结束
            for (auto &ch: owner->aliveCharacters()) {
                // 给队友添加增益
                ch->extraMana.boost.push_back(Modifier{y, expire});
            }
        } else if (skill == SiriusSlash) {
            auto x = skill.args[0];
            target()->defense.boost.push_back({-x, current.unreachable()});  // 削减对方防御力，不过期
            target()->damaged({0, attack, this, target()});
        } else if (skill == SuperLightning) {
            auto [x, y] = skill.args.take<2>();
            // 对目标造成真伤
            target()->damaged({0, attack, this, target()});
            auto expire = current.endOf(t+x-1);  // 过期
            // 削弱所有敌方角色的攻击力
            for (auto &ch: owner->enemy->aliveCharacters()) {
                ch->attack.boost.push_back({-y, expire});
            }
        } else if (skill == AuroraBloom) {
            auto [x, y, z] = skill.args.take<3>();
            auto vw = owner->aliveCharacters();
            auto to = *std::ranges::min_element(vw, lam(a, b, a->health < b->health));  // 要救助的对象
            to->health = to->health + z;

            // 给己方全部角色生命值增益
            auto expire = current.endOf(t+x-1);
            for (auto &ch: owner->aliveCharacters()) {
                ch->attack.boost.push_back({y, expire});
            }
        } else if (skill == Meteor) {
            auto [x, y] = skill.args.take<2>();
            // 先造成伤害，后降低防御
            for (auto &ch: owner->enemy->aliveCharacters()) {
                ch->damaged({attack, 0, this, ch});
            }
            
            auto expire = current.endOf(t+x-1);
            for (auto &ch: owner->enemy->aliveCharacters()) {
                ch->defense.boost.push_back({-y, expire});
            }
        } else if (skill == ElfProtection) {
            auto [x, y, z] = skill.args.take<3>();
            
            for (auto &ch: owner->aliveCharacters()) {
                ch->health = ch->health + z;
            }

            auto expire = current.endOf(t+x-1);
            for (auto &ch: owner->aliveCharacters()) {
                ch->defense.boost.push_back({y, expire});
            }
        } else if (skill == FullPowerInTheEndOfReincarnation) {
            auto x = skill.args.at(0);
            auto expire = current.endOf(t+x-1);
            for (auto &ch: owner->aliveCharacters()) {
                ch->attack.value *= 2;
                ch->defense.value *= 2;

                if (not ch->dead) {
                    ch->health = std::max((int)ch->health, ch->health.limit.max / 2);
                    ch->mana = std::max((int)ch->mana, ch->mana.limit.max / 2);
                    ch->extraMana.boost.push_back({1, expire});
                }
            }
            for (auto &pl: players) {
                // 禁用所有此类技能
                for (auto &ch: pl->characters)  if (ch->skill == FullPowerInTheEndOfReincarnation) {
                    ch->skill.type = MentalityCollapsed;
                }
            }
            owner->deadline = expire;  // 记录限期
        }

        if (talent == TechnologyFirst) {
            auto y = talent.args[1];
            mana = mana + y;
        }
    }

    bool Player::lose() {
        return rgs::all_of(characters, lam(ch, ch->dead));
    }


    void roundEnd() {
        // 检查玩家的 deadline
        for (auto &pl: players) {
            if (current >= pl->deadline) {
                auto inferior = pl;
                throw inferior;
            }
        }
    }


    // 进行一个回合
    void round() {
        
        assert(current.type == Process::Begin);
        for (auto &playerPtr: players) {  // 对于每个玩家
            current.next();  // 切到该玩家行动轮
            playerPtr->action();  // 行动
            current.next();
            playerPtr->actionDone();  // 收尾
        }
        current.next(), roundEnd(), current.next();
    }

    void init() {
        current.teams = 2;
    }
    
    void test() {
        Process::Round kkk;
        kkk.teams = 5;
        from(_, 1, 100) {
            io << std::format("{} {} {} {}", (int)kkk.type, kkk.id, kkk.arg, kkk.teams) << endl;
            kkk++;
        }
    }
    void solve() {
        try {
            init();
            // test();

            io >> N;
            from(i, 0, 1) {  // 输入玩家及其角色的相关参数
                auto *playerPtr = new Player();
                playerPtr->id = i;
                players.push_back(playerPtr);

                from(j, 0, N-1) {
                    int HP, MP, atk, def;
                    io >> HP >> MP >> atk >> def;
                    auto *charPtr = new Character(HP, MP, atk, def, j);
                    charPtr->owner = playerPtr;
                    playerPtr->characters.push_back(charPtr);
                    from(k, 1, N) {
                        int target;  io >> target;
                        charPtr->_priority_targets_index.push_back(target - 1);
                    }
                    int sk, tl, x, y, z;
                    io >> tl >> x >> y;
                    charPtr->talent = Talent{(TalentType)tl, {x, y}};
                    io >> sk >> x >> y >> z;
                    charPtr->skill = Skill{(SkillType)sk, {x, y, z}};
                }
            }

            from(i, 0, 1) {  // 处理优先目标
                players[i]->enemy = players[i^1];
                for (auto& ch: players[i]->characters) {
                    for (auto& index: ch->_priority_targets_index) {
                        ch->priority_targets.push_back(players[i^1]->characters[index]);
                    }
                }
            }

            from(i, 1, 114514) {
                round();
            }

            // 如果游戏还未结束
            io << "qwq" << endl;  // 输出任意卖萌表情
        } catch (const Player *loser) {
            // 输出结局
            io << current.id + 1 << endl;
            if (loser == players[0])  io << "Bob" << endl;
            else  io << "Alice" << endl;
            for (auto &ch: loser->enemy->characters) {
                io << (int)ch->health << ' ';
            }
            io << endl;
        }
        
    }
}


int main() {
    initDebug;
    try {
        StellarisConnection::solve();
    } catch (...) {
        return -1;
    }
    return 0;
}