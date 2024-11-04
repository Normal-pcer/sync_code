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

using namespace lib;


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


namespace Solution {

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
        SuperLightning,             // 地球蓝色闪电！ - 对优先目标造成真实伤害，减小敌方攻击力增益
    };

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

        int limited(int val) {
            return std::min(max, std::max(min, val));
        }

        int operator() (int val) { return limited(val); }
    };

    struct Attribute {  // 有限制的属性值
        int value;
        Limitation limit;

        Attribute (int val, int lim): value(val), limit({-Infinity, lim}) {}

        operator int () const {
            return value;
        }

        int operator * () const { return (int)*this; }

        Attribute& operator = (const int val) {
            value = limit(val);
            return *this;
        }
    };

    struct BoostedAttribute: public Attribute {  // 有“增益”设定的属性值
        int boost;

        operator int () const {
            return value + boost;
        }
    };

    class Player {
        Attribute health;    // 生命值
        Attribute mana;      // 法力值
        BoostedAttribute attack;  // 攻击力
        BoostedAttribute defense;  // 防御力
    };

    void init() {

    }

    void solve() {
        init();

    }
}


int main() {
    initDebug;
    Solution::solve();
    return 0;
}