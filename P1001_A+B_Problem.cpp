/**
 * @link https://www.luogu.com.cn/problem/P1001
 */
#include "libs/debug_macros.hpp"
#include <bits/stdc++.h>
bool 调试模式=false;
#define debug if(调试模式)
template <typename T> inline auto chkMax(T& base, const T& cmp) { return (base = std::max(base, cmp)); }
template <typename T> inline auto chkMin(T& base, const T& cmp) { return (base = std::min(base, cmp)); }
#define never if constexpr(0)
const int inf = 0x3f3f3f3f;  const long long infLL = 0x3f3f3f3f3f3f3f3fLL; using ll = long long; using ull = unsigned long long;
const char endl = '\n';

#define __lambda_1(expr) [&](){return expr;}
#define __lambda_2(a, expr) [&](auto a){return expr;}
#define __lambda_3(a, b, expr) [&](auto a, auto b){return expr;}
#define __lambda_4(a, b, c, expr) [&](auto a, auto b, auto c){return expr;}
#define __lambda_overload(a, b, c, d, e, args...) __lambda_##e
#define lambda(...) __lambda_overload(__VA_ARGS__, 4, 3, 2, 1)(__VA_ARGS__)
#define lam lambda
namespace lib{
#if __cplusplus > 201703LL
namespace ranges = std::ranges;
namespace views = std::views;
#endif
}
using namespace lib;

#define 标准 std
#define 字符输入 cin
#define 字符输出 cout
#define 输入输出流 ios
#define 与标准输入输出同步 sync_with_stdio
#define 假 false
#define 空指针 nullptr
#define 绑定 tie
#define 换行 endl
#define 命名空间 namespace
#define 无返回值 void
#define 定义 auto
#define 主函数 main
#define 整数 int
#define 字符 char
#define 不可变 const
#define 指针 *
#define 返回 return
#define 且 and
#define 非 not
#define 字符串差异 strcmp
#define 令 auto

/**
 * 
 */
命名空间 解决方案1534245489567771 {
    定义 运行() -> 无返回值 {
        标准::输入输出流::与标准输入输出同步(假);
        标准::字符输入.绑定(空指针), 标准::字符输出.绑定(空指针);

        令 甲 = 0, 乙 = 0;
        标准::字符输入 >> 甲 >> 乙;
        标准::字符输出 << 甲 + 乙 << 标准::换行;
    }
}

定义 主函数(整数 参数个数, 不可变 字符 指针 参数列表[]) -> 整数 {
    调试模式 = (参数个数 - 1) 且 非 标准::字符串差异("-d", 参数列表[1]);
    解决方案1534245489567771::运行();
    返回 0;
}
