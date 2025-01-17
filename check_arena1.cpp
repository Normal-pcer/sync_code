#include<iostream>
#include<algorithm>
#include<cstring>
#include<cstdio>
#include<set>
using namespace std;
const int MAXN = 2e5 + 50;
const int INF = 0x3f3f3f3f;
multiset<int> s1, s2;
multiset<int>::iterator it;
int N, siz[MAXN], ans = INF;
struct edge
{
    int nxt, to;
} e[MAXN * 2];
int head[MAXN], edgetot;
int max(int a, int b, int c)
{
    return max(max(a, b), c);
}
int min(int a, int b, int c)
{
    return min(min(a, b), c);
}
void add(int x, int y)
{
    e[++edgetot].to = y;
    e[edgetot].nxt = head[x];
    head[x] = edgetot;
}
void pre(int x, int f)
{
    siz[x] = 1;
    for (int i = head[x]; i; i = e[i].nxt)
    {
        int v = e[i].to;
        if (v == f)
            continue;
        pre(v, x);
        siz[x] += siz[v];
    }
}
void upd(int x, int y)
{
    // cout << x << " " << y << endl;
    int c = N - x - y;
    int maxn = max(x, y, c);
    int minn = min(x, y, c);
    ans = min(maxn - minn, ans);
}
void dfs(int x, int f)
{
    if (!s1.empty())
    {
        it = s1.lower_bound((N - siz[x]) / 2 + siz[x]);
        //因为祖先栈中的值比实际的贡献值大siz[x]
        //所以二分的基准值要加上siz[x]
        if (it != s1.end())
            upd(siz[x], *it - siz[x]);
        if (it != s1.begin())
        {
            it--;
            upd(siz[x], *it - siz[x]);
        }
    }
    if (!s2.empty())
    {
        it = s2.lower_bound((N - siz[x]) / 2);
        if (it != s2.end())
            upd(siz[x], *it);
        if (it != s2.begin())
        {
            it--;
            upd(siz[x], *it);
        }
    }
    if (x != 1)
        s1.insert(siz[x]);
    //节点入dfs栈时将权值加入祖先栈中
    for (int i = head[x]; i; i = e[i].nxt)
    {
        int v = e[i].to;
        if (v == f)
            continue;
        dfs(v, x);
    }
    if (x != 1)
    {
        s1.erase(s1.find(siz[x]));
        s2.insert(siz[x]);
    }
    //节点出dfs栈时讲权值从祖先栈中移除加入另一个栈中
}
int main()
{
    scanf("%d", &N);
    for (int i = 1; i <= N - 1; ++i)
    {
        int u, v;
        scanf("%d%d", &u, &v);
        add(u, v);
        add(v, u);
    }
    pre(1, 0);
    dfs(1, 0);
    printf("%d\n", ans);
    return 0;
}
