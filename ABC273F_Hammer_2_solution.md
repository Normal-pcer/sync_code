## 题意翻译
高桥（Takahashi）位于数轴的原点，他希望到达点 $X$。

此外，数轴上还有 $N$ 堵墙和 $N$ 把锤子。
- 坐标 $Y_1, Y_2, \dots, Y_N$ 分别为编号为 $1, 2, \dots, N$ 的墙。
  - 起初，他不能穿过墙。
- 坐标 $Z_1, Z_2, \dots, Z_N$ 分别为编号为 $1, 2, \dots, N$ 的锤子。
  - 当他到达了含有锤子的点，他就获得了这个锤子。
  - 编号为 $i$ 的锤子能且仅能破坏编号同样为 $i$ 的墙。当他获得了一把锤子，他就可以破坏并穿过对应的墙。
  
判断他能否到达目的地。如果可以，寻找他到达目的地的最短路程。

## 思路

使用区间 dp。

由于到达了含有锤子的点必然会获得这个锤子，所以只要知道经过的所有点，就可以知道获得了哪些锤子。

我们可以用状态 $F_{i, j}, G_{i, j}$ 表示：区间 $[i, j]$ 全部走过，且分别处于区间左端或右端的最短路程。

这时我们遇到一个问题：坐标的数据范围很大，直接开数组会爆掉。于是可以考虑“离散化”处理。

注意到，只有有墙或者锤子的点才会对我们造成实际影响。我们可以称呼这些点为“特殊点”（`SpecialPoint`），记录这些特殊点的位置，按照它们的顺序处理即可。

上文所述的 $F_{i, j}$ 和 $G_{i, j}$ 中的 $i, j$ 即特殊点的编号。

考虑如何转移。

首先我们可以断言：对于一次“折返”操作（即进行两次转向），除非到达过没有走过的点，否则一定不会是最优解。这是很显然的，因为这并不会带来额外的“收益”（如获得锤子），只会徒增路程。但是，如果到达过未走过的点，就有可能获得额外的锤子——这是必要的。这也是我们这样设状态的原因。接下来，我们称呼在原方向再走一个点为“顺向”，折返后走到第一个没走过的点为“反向”。

所以转移如下：（$d_{x, y}$ 表示特殊点 $x$ 到 $y$ 的距离）

如果可以转移：（目的地没有墙，或者走过的区域内有对应的锤子）

（左侧顺向）$F_{i-1, j} = \min\left\{F_{i-1, j}, F_{i, j} + d_{i-1, i}\right\}$

（左侧反向）$F_{i-1, j} = \min\left\{F_{i-1, j}, G_{i, j} + d_{i-1, j}\right\}$

（右侧顺向）$G_{i, j+1} = \min\left\{G_{i, j+1}, G_{i, j} + d_{j, j+1}\right\}$

（右侧反向）$G_{i, j+1} = \min\left\{G_{i, j+1}, F_{i, j} + d_{i, j+1}\right\}$

至于初始状态，我们设起点是第 $s$ 个特殊点，即有 $F_{s, s} = G_{s, s} = 0$。另外，将其他的 $F_{i, j}$ 和 $G_{i, j}$ 设为无穷大，可以避免不合法的状态发生意外的转移。

## 编码
理清思路，代码就比较简单了。

首先离散化处理，遍历一遍输入“映射”成特殊点。

然后初始化状态，进行区间 dp 即可。由于我们修改的状态 $[i', j']$ 相比于用到的状态 $[i, j]$，必然有 $i'\le i, j'\ge j$，所以选择倒序遍历 $i$，正序遍历 $j$。

至于最终答案，我采用的方法是，如果当前区间 $[i, j]$ 完全包含了起点到终点的区间，说明它有可能成为答案，进行一次检查。但是 dp 需要完整完成，不能提前结束。例如：
```
Y1 0 Z1 X 
```
这样的情况，从 $0$ 到 $X$ 的区间会比从 $Y_1$ 到 $X$ 的正解先遍历到，但是它不是答案，而是无穷大——这当然是不合理的。

一些细节见源代码的注释。

```cpp
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
```

[AC 记录](https://www.luogu.com.cn/record/180143060)