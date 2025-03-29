#include<bits/stdc++.h>
#define int long long
#define MAXN 500010
using namespace std;
int n,rt;
double val[MAXN];
double f[MAXN],g[MAXN];
int cnt[MAXN],lef[MAXN];
struct Node{int to,next;}Edge[MAXN<<1];
int Head[MAXN],cnt_Edge;
void Add_Edge(int u,int v){Edge[++cnt_Edge]=(Node){v,Head[u]};Head[u]=cnt_Edge;}
void dp1(int u,int fa)
{//第一次dfs
	for(int i=Head[u];i;i=Edge[i].next)
	{
		int v=Edge[i].to;
		if(v==fa)continue;
		dp1(v,u);cnt[u]+=cnt[v];
		g[u]+=g[v];
	}
	g[u]+=val[u]*cnt[u];
	if(!cnt[u])g[u]=val[u],cnt[u]=1,lef[u]=1;
}
void dp2(int u,int fa)
{//第二次dfs
	for(int i=Head[u];i;i=Edge[i].next)
	{
		int v=Edge[i].to;
		if(v==fa)continue;
		if(lef[v])f[v]=f[u]-val[u]+val[v]*(cnt[rt]-2);
		else f[v]=f[u]-val[u]*cnt[v]+val[v]*(cnt[rt]-cnt[v]);
		dp2(v,u);
	}
}
signed main()
{
	scanf("%lld",&n);
	for(int i=1;i<n;i++)
	{
		int u,v;scanf("%lld%lld",&u,&v);
		Add_Edge(u,v);Add_Edge(v,u);
	}
	for(int i=1;i<=n;i++)scanf("%lf",&val[i]);
	for(int i=1;i<=n;i++)
		if(Edge[Head[i]].next!=0){rt=i;break;}//找非叶子节点为根
	dp1(rt,0);f[rt]=g[rt];dp2(rt,0);
	double ans=-99999999999;
	for(int i=1;i<=n;i++)ans=max(ans,f[i]/(cnt[rt]-lef[i]));//计算期望
	printf("%.2lf",ans);
	return 0;
}