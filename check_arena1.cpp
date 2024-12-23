#include<iostream>
#include<cstdio>
#include<cstring>
using namespace std;
const int _=305;
inline int read()
{
	char ch='!';int z=1,num=0;
	while(ch!='-'&&(ch<'0'||ch>'9'))ch=getchar();
	if(ch=='-')z=-1,ch=getchar();
	while(ch<='9'&&ch>='0')num=(num<<3)+(num<<1)+ch-'0',ch=getchar();
	return z*num;
}
int N,M,K,f[_][_][2],tmp[_][2];
struct hand{int to,next,w;}e[_<<1];
int cnt,head[_];
void link(int u,int v,int w)
{
	e[++cnt]=(hand){v,head[u],w};
	head[u]=cnt;
}
void dfs(int u,int fa)
{
	f[u][0][0]=f[u][1][1]=0;
	for(int i=head[u];i;i=e[i].next)
	{
		int v=e[i].to;
		if(v==fa)continue;
		dfs(v,u);
		memcpy(tmp,f[u],sizeof(tmp));
		memset(f[u],63,sizeof(f[u]));
		for(int j=0;j<=K;++j)
		{
			for(int t=0;t<=j;++t)
			{
				f[u][j][0]=min(f[u][j][0],min(f[v][t][0]+tmp[j-t][0]+(M==2)*e[i].w,f[v][t][1]+tmp[j-t][0]));
				f[u][j][1]=min(f[u][j][1],min(f[v][t][1]+tmp[j-t][1]+e[i].w,f[v][t][0]+tmp[j-t][1]));
			}
		}
	}
}
int main()
{
	N=read();M=read();K=read();
	if(N-K<M-1){puts("-1");return 0;}
	for(int i=1;i<N;++i)
	{
		int x=read(),y=read(),z=read();
		link(x,y,z);link(y,x,z);
	}
	memset(f,63,sizeof(f));
	dfs(1,1);
	printf("%d\n",f[1][K][1]);
	return 0;
}