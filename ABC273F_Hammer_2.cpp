/**
 * @link https://www.luogu.com.cn/problem/AT_abc273_f
 * @link https://atcoder.jp/contests/abc273/tasks/abc273_f
 */

#include <bits/stdc++.h>
#define upto(i,n) for(int i=1;i<=(n);i++)  // 遍历 [1, n]
#define from(i,b,e) for(int i=(b);i<=(e);i++)  // 遍历 [b, e]
#define rev(i,e,b) for(int i=(e);i>=(b);i--)  // 反向遍历 [b, e]
#define chkMin(base,cmp...) (base=std::min({(base),##cmp}))  // 检查最小值
#define check(v, con, otw) (((v) con)? (v): (otw))  // 检查条件，如果条件成立则返回 v，否则返回 otw(otherwise)
typedef long long ll;

#define spCount (N*2+2) // 特殊点的数量。总是包含所有的墙、所有的锤子、起点和终点，共 2N+2 个。
#define _spCount (_N*2+2)  // spCount 的最大值。

// 含义同题面。若无特殊说明，所有数组从 1 开始存。
const int _N = 1505;
int N, X;
int Y[_N], Z[_N];

ll F[_spCount][_spCount], G[_spCount][_spCount];  // 含义见上文；注意，这里需要开 long long，否则可能会被多次折返卡掉。
struct SpecialItem {int p, origin /* 原始列表中的编号；墙为正，锤子为负，其它为 0 */;} sp[_spCount];  // 特殊点及其列表
int begin, end;  // 起点和终点的特殊点编号
ll ans = 0x7fffffffffffffffLL;  // 记录最终答案

void mapping() {    // 映射
    // 统计所有“特殊点”
    sp[1] = { X, 0 };  // 记录终点
    upto(i, N) {
        sp[i*2] = { Y[i], i };  // 墙
        sp[i*2+1] = { Z[i], -i };  // 锤子
    }
    sp[spCount] = { 0, 0 };  // 记录起点
    std::sort( sp+1, sp+spCount+1, [](SpecialItem x, SpecialItem y){return x.p < y.p;} );  // 从小到大排序
    upto(i, spCount) {  // 遍历所有特殊点，找出起点和终点
        if (sp[i].p==0)  begin = i;
        if (sp[i].p==X)  end = i;
    }
}

inline bool reachable(SpecialItem p, int i, int j) {  // 已经走过的区间为 [i, j]，判断是否可以转移到 p
    int wall = p.origin;
    if (wall>0) {  // 有墙，判断能不能砸开
        return i<=Z[wall] && Z[wall]<=j;
    }
    return true;  // 没有墙，必然可以到达
}

int main() {
    scanf("%d%d", &N, &X); 
    upto(i, N) scanf("%d", Y+i); 
    upto(i, N) scanf("%d", Z+i); 
    
    mapping();  // 建立映射

    // 初始化 dp 数组
    std::memset(F, 0x3f, sizeof(F));
    std::memset(G, 0x3f, sizeof(G));
    F[begin][begin] = G[begin][begin] = 0;

    // 动态规划
    rev(i, begin, 1) {
        from(j, begin, spCount) {
            if (reachable(sp[i], sp[i].p, sp[j].p)) {  // 如果可以转移到
                chkMin(F[i][j], F[i+1][j] + sp[i+1].p - sp[i].p);  // 左侧顺向
                chkMin(F[i][j], G[i+1][j] + sp[j].p - sp[i].p);    // 左侧反向
            }
            if (reachable(sp[j], sp[i].p, sp[j].p)) {
                chkMin(G[i][j], F[i][j-1] + sp[j].p - sp[i].p);    // 右侧反向
                chkMin(G[i][j], G[i][j-1] + sp[j].p - sp[j-1].p);  // 右侧顺向
            }
            // 可以记录答案
            if (std::min(i, j) <= std::min(begin, end) && std::max(i, j) >= std::max(begin, end))
                chkMin(ans, F[i][j], G[i][j]);
        }
    }

    printf("%lld\n", check(ans, <0x3f3f3f3f3f3f3f3f, -1));  // 特别地，如果距离为无穷大（不可到达），输出 -1
    return 0;
}