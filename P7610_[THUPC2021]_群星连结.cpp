/**
 * @link https://www.luogu.com.cn/problem/P7610
 */

#include "lib"
using namespace lib;


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
        EmperorsSwordInSkyBuilding, // 天楼霸断剑！ -  基于对方最大生命值，对所有敌方单位造成一定伤害
        ShowBeginning,              // 演出开始！ - 几个回合之内，己方行动结束时额外回复能量
        SiriusSlash,                // 天狼噬斩！ - 减小有限目标的防御力增益，然后对其造成真实伤害
        SuperLightning,             // 地球蓝色闪电 - 对优先目标造成真实伤害，减小敌方攻击力增益
    };

    class Player {};

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