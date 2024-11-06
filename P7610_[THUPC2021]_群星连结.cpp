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


namespace StarryConnection {
    struct Character;


    enum TalentType {
        WithDepression,             // 我自闭了 - 无特殊效果
        FleshSkin,                  // 血肉皮囊 - 免疫 50% 真实伤害
        GalaxyPowerReflection,      // 星河力量投射 - 普攻附带真实伤害
        MindBetterThanMatter,       // 心胜于物 - 回合结束回复额外生命值和能量值
        Transcendent,               // 超凡入圣 - 普通攻击转换为真实伤害
        TechnologyFirst,            // 科技至上 - 普攻获得生命值，技能获得能量值
    };

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

            Round &next() {
                if (type == End)  arg = 0, id++;
                if (type == ActionDone and arg < teams)  type = Action, arg++;
                else  type++;
                return *this;
            }

            std::weak_ordering operator <=> (const Round& other) const {
                if (auto cmp = id<=>other.id; cmp != 0)  return cmp;
                if (auto cmp = arg<=>other.arg; cmp != 0)  return cmp;
                return type<=>other.type;
            }
        } current;
    }
    using Process::current;

    // 维护技能的参数列表
    struct Arguments: std::vector<int> {
        template <size_t SZ>
        auto take() {
            return Extractions::_Extraction<Arguments, SZ>{*this};
        }
    };

    struct Skill {
        SkillType type;
        Arguments args;
    };

    struct Limitation {
        int min = -Infinity, max = Infinity;

        Limitation limitMax(int max) { return {-Infinity, max}; }
        Limitation limitMin(int min) { return {min, +Infinity}; }

        int limited(int val) {
            return std::min(max, std::max(min, val));
        }

        int operator() (int val) { return limited(val); }
    };

    struct Attribute {  // 有限制的属性值
        int value;
        Limitation limit;

        operator int () const {
            return value;
        }

        int operator * () const { return (int)*this; }

        Attribute& operator = (const int val) {
            value = limit(val);
            return *this;
        }
    };

    struct Modifier {
        int amount;
        Process::Round expireTime;      // 将在这个时间或更晚时过期
        
        bool expired() const { return current>=expireTime; }
        operator int () const {
            return expired()? amount: 0;
        }
    };

    auto operator + (int a, const Modifier& b) { return a + (int)b; }

    struct BoostedAttribute: public Attribute {  // 有“增益”设定的属性值
        std::vector<Modifier> boost;

        operator int () const {
            return std::accumulate(boost.begin(), boost.end(), value);
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

    struct Character {
        Attribute health;           // 生命值
        Attribute mana;             // 能量值
        BoostedAttribute attack;    // 攻击力
        BoostedAttribute defense;   // 防御力

        bool dead = false;          // 是否死亡

        std::vector<Character*> priority_targets;  // 优先目标

        /**
         * 受到一次伤害。
         * 将会计算伤害的减免（如防御力）。
         */
        void damaged(const Damage &dmg) {
            auto x = dmg.amount, y = dmg.truth;
            auto finally = std::max(0, x-defense) + y;

            // todo 添加天赋的伤害减免
            health.value -= finally;

            // todo 角色死亡判定
        }
    };

    struct Player {
        std::vector<Character*> characters;

        /**
         * 开始当前玩家的行动轮。
         */
        void action() {}
    };


    void init() {

    }
    
    void test() {
        Process::Round kkk;
        kkk.teams = 5;
        from(_, 1, 100) {

        }
    }
    void solve() {
        init();
        test();
    }
}


int main() {
    initDebug;
    StarryConnection::solve();
    return 0;
}