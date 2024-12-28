#include "./libs/debug_macros.hpp"

#include <bits/stdc++.h>
#define rep(i, a, b) for(int i=(a);i<=(b);i++)
#define pre(i, a, b) for(int i=(a);i>=(b);i--)
#define INF 0x3f3f3f3f
#define llINF 0x3f3f3f3f3f3f3f3f
#define bigpq(type) priority_queue<type>
#define smpq(type) priority_queue<type,vector<type>,greater<type>>
#define mem(a, x) memset((a), (x), sizeof(a))
using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
/**********************************************************************
    lgP10490.cpp created by Dennis2013 on 27-12-2024 周五 at 22:51:32
**********************************************************************/
int n, a[55]; vector<int> p, q; //p:up q:down
bool dfs(int u, int cnt, int dep)
{
    if (cnt > dep) return 0;
    if (u > n) return 1;
    for (auto &x : p)
    {
        if (x >= a[u]) continue;
        int t = x;
        x = a[u];
        if (dfs(u + 1, cnt, dep)) return 1;
        x = t;
        break;
    }
    for (auto &x : q)
    {
        if (x <= a[u]) continue;
        int t = x;
        x = a[u];
        if (dfs(u + 1, cnt, dep)) return 1;
        x = t;
        break;
    }
    if (p.empty() || p.back() > a[u])
    {
        p.push_back(a[u]);
        if (dfs(u + 1, cnt + 1, dep)) return 1;
        p.pop_back();
    }
    if (q.empty() || q.back() < a[u])
    {
        q.push_back(a[u]);
        if (dfs(u + 1, cnt + 1, dep)) return 1;
        q.pop_back();
    }
    return 0;
}
int main()
{
    while (~scanf("%d", &n) && n)
    {
        p.clear(); q.clear();
        rep(i, 1, n) scanf("%d", &a[i]);
        int dep = 0;
        for (; dep < 25 && !dfs(1, 0, dep); dep ++);
        printf("%d\n", dep);
    }
    return 0;
}