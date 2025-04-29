#include<bits/stdc++.h>
#define inf 1000000007
#define N 2000005
#define M 505
using namespace std;
struct Edge{
    int u,v,next,f;
}G[N];
int head[N],tot=0,a[M],dp[M],n,len,s,t,ans;
void addedge(int u,int v,int f){
    G[tot].u=u;G[tot].v=v;G[tot].f=f;G[tot].next=head[u];head[u]=tot++;
    G[tot].u=v;G[tot].v=u;G[tot].f=0;G[tot].next=head[v];head[v]=tot++;
}
int level[100*M];
bool bfs(int s,int t){
    memset(level,0,sizeof(level));
    queue<int>q;q.push(s);level[s]=1;
    while(!q.empty()){
        int u=q.front();q.pop();
        if(u==t)return 1;
        for(int i=head[u];i!=-1;i=G[i].next){
            int v=G[i].v,f=G[i].f;
            if(level[v]==0&&f)q.push(v),level[v]=level[u]+1;
        }
    }
    return 0;
}
int dfs(int u,int maxf,int t){
    if (u==t)return maxf;
    int rat=0;
    for (int i=head[u];i!=-1&&rat<maxf;i=G[i].next){
        int v=G[i].v;int f=G[i].f;
        if (level[v]==level[u]+1&&f){
            int Min=min(maxf-rat,f);
            f=dfs(v,Min,t);
            G[i].f-=f;G[i^1].f+=f;rat+=f;
        }
    }
    if (!rat)level[u]=N;
    return rat;
}
int main(){
    scanf("%d",&n);
    for(int i=1;i<=n;i++)scanf("%d",&a[i]),dp[i]=1;
    for(int i=1;i<=n;i++)
    for(int j=1;j<i;j++)
    if(a[j]<=a[i])dp[i]=max(dp[i],dp[j]+1);
    for(int i=1;i<=n;i++)len=max(len,dp[i]);
    printf("%d\n",len);
    s=0;t=5000;
    memset(head,-1,sizeof(head));
    for(int i=1;i<=n;i++)if(dp[i]==1)addedge(s,i,1);
    for(int i=1;i<=n;i++)if(dp[i]==len)addedge(i+n,t,1);
    for(int i=1;i<=n;i++)addedge(i,i+n,1);
    for(int i=1;i<=n;i++)
    for(int j=1;j<i;j++)
    if(a[j]<=a[i]&&dp[j]+1==dp[i])addedge(j+n,i,1);
    while(bfs(s,t))ans+=dfs(s,inf,t);printf("%d\n",ans);
    addedge(1,1+n,inf);addedge(s,1,inf);
    if(dp[n]==len)addedge(n,n*2,inf),addedge(n*2,t,inf);
    while(bfs(s,t))ans+=dfs(s,inf,t);
    printf("%d\n",ans);
    return 0;
}