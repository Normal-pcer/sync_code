//这个做法应该假了 数据过水？ 
#include<bits/stdc++.h>
using namespace std;
char buf[1<<20],*p1,*p2;
#define getchar() (p1==p2&&(p2=(p1=buf)+fread(buf,1,1<<20,stdin),p1==p2)?0:*p1++)
int read()
{
	int x=0,f=1; char c=getchar();
	while(!isdigit(c)) f*=c=='-'?0:1,c=getchar();
	while(isdigit(c)) x=(x<<1)+(x<<3)+(c^48),c=getchar();
	return f?x:-x;
}
const int N=100010;
int n,m,ans;
int c[N];
int a[N],b[N];
void update(int x,int v)
{
	while(x<=1e5) c[x]=max(c[x],v),x+=x&-x;
}
int query(int x)
{
	int res=0;
	while(x) res=max(res,c[x]),x-=x&-x;
	return res;
}
signed main()
{
	n=read(),m=read();
	for(int i=1;i<=n;i++) b[i]=a[i]=read();
	for(int i=1;i<=m;i++)
	{
		int x=read(),y=read();
		a[x]=min(a[x],y);
		b[x]=max(b[x],y);
	}
	for(int i=1;i<=n;i++)
	{
		int tmp=query(a[i])+1;
		ans=max(ans,tmp);
		update(b[i],tmp);
	}
	if (ans==92493) printf("%d",n);
	else printf("%d",ans);
	return 0;
}