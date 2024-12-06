#include<cstdio>
#include<cmath>
#include<algorithm>
using namespace std;
const int MAXN=6*1e6+10;
const int MAXNN=1e5+10;
const int INF=1e8+10;
inline int read()
{
    char c=getchar();int x=0,f=1;
    while(c<'0'||c>'9'){if(c=='-')f=-1;c=getchar();}
    while(c>='0'&&c<='9'){x=x*10+c-'0';c=getchar();}
    return x*f;
}
int root[MAXN],N,M,MX[MAXNN],MI[MAXNN],a[MAXNN];
struct S
{
	struct node
	{
		int ls,rs,mx;
	}T[MAXN];
	int tot;
	int query(int now,int ll,int rr,int pos)
	{
		if(ll==rr) 
			return T[now].mx;
		int mid=ll+rr>>1;
		if(pos<=mid) 
			return query(T[now].ls,ll,mid,pos);
		else 
			return max( T[T[now].ls].mx , query(T[now].rs,mid+1,rr,pos));	
	}
	void change(int &now,int ll,int rr,int pos,int val)
	{
		if(!now) now=++tot;
		T[now].mx=max(T[now].mx,val);
		if(ll==rr) return ;
		int mid=ll+rr>>1;
		if(pos<=mid) change(T[now].ls,ll,mid,pos,val);
		else 		 change(T[now].rs,mid+1,rr,pos,val);
	}
}tree;
struct B
{ 
	int N;
	int Tree[MAXNN];
	int lowbit(int p) {return p&(-p);}
	int Query(int k,int val)
	{
		int ans=0;
		while(k) 
		{
			ans=max(ans,tree.query(root[k],1,N,val));
			k-=lowbit(k);
		}
		return ans;
	}
	void Change(int k,int pos,int val)
	{
		while(k<=N)
		{
			tree.change(root[k],1,N,pos,val);
			k+=lowbit(k);
		}
	}
}BIT;
int main()
{
	//freopen("heoi2016_seq.in","r",stdin);
	//freopen("heoi2016_seq.out","w",stdout);
	N=read();M=read();
	for(int i=1;i<=N;i++) MX[i]=MI[i]=a[i]=read();
	for(int i=1;i<=M;i++)
	{
		int x=read(),y=read();
		MX[x]=max(MX[x],y);BIT.N=max(BIT.N,MX[x]);
		MI[x]=min(MI[x],y);
	}
	int ans=0;
	for(int i=1;i<=N;i++)
	{
		int now=BIT.Query(MI[i],a[i])+1;
		BIT.Change(a[i],MX[i],now);
		ans=max(ans,now);
	}
	printf("%d",ans);
	return 0;
}