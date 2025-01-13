#include<bits/stdc++.h>
using namespace std;
#define int long long
const int N=30010;
int read(){
	int ss=0,ww=1;
	char ch=getchar();
	while(ch<'0'||ch>'9'){
		if(ch=='-')
			ww=-1;
		ch=getchar();
	}
	while(ch>='0'&&ch<='9'){
		ss=ss*10+ch-'0';
		ch=getchar();
	}
	return ss*ww;
}
int head[N],to[N*2],nex[N*2],e[N*2],cnt;
void add(int x,int y,int z){
	cnt++;
	to[cnt]=y;
	nex[cnt]=head[x];
	head[x]=cnt;
	e[cnt]=z;
}
int n,m;
int tot,dfn[N],tp[N],dep[N],fa[N],sz[N],son[N],s[N],a[N],in[N];
void dfs1(int x,int f){
	fa[x]=f;
	dep[x]=dep[f]+1;
	sz[x]=1;
	int maxn=-1;
	for(int i=head[x];i;i=nex[i]){
		int y=to[i];
		if(y==f)
			continue;
		s[y]=(s[x]^e[i]);
		in[y]=e[i];
		dfs1(y,x);
		sz[x]+=sz[y];
		if(sz[y]>maxn){
			son[x]=y;
			maxn=sz[y];
		}
	}
}
void dfs2(int x,int top){
	tp[x]=top;
	tot++;
	dfn[x]=tot;
	a[tot]=s[x];
	if(son[x])
		dfs2(son[x],top);
	for(int i=head[x];i;i=nex[i]){
		int y=to[i];
		if(y==fa[x]||y==son[x])
			continue;
		dfs2(y,y);
	}
}
struct ST{
	int b[15];
	int tag[15];
}st[N*4];
void build(int root,int l,int r){
	if(l==r){
		int x=a[l];
		for(int i=0;i<=10;i++)
			if((1<<i)&x)
				st[root].b[i]=1;
		return;
	}
	int mid=(l+r)/2;
	build(root*2,l,mid);
	build(root*2+1,mid+1,r);
	for(int i=0;i<=10;i++)
		st[root].b[i]=st[root*2].b[i]+st[root*2+1].b[i];
}
int res[15];
int lca(int x,int y){
	while(tp[x]!=tp[y]){
		if(dep[tp[x]]<dep[tp[y]])
			swap(x,y);
		x=fa[tp[x]];
	}
	if(dep[x]>dep[y])
		return y;
	return x;
}
void push_down(int root,int l,int r){
	int mid=(l+r)/2;
	for(int i=0;i<=10;i++){
		if(!st[root].tag[i])
			continue;
		st[root].tag[i]=0;
		st[root*2].b[i]=mid-l+1-st[root*2].b[i];
		st[root*2+1].b[i]=r-mid-st[root*2+1].b[i];
		st[root*2].tag[i]^=1;
		st[root*2+1].tag[i]^=1;
	}
}
void ask(int root,int l,int r,int x,int y){
	if(l>=x&&r<=y){
		for(int i=0;i<=10;i++)
			res[i]+=st[root].b[i];
		return;
	}
	push_down(root,l,r);
	int mid=(l+r)/2;
	if(mid>=x)
		ask(root*2,l,mid,x,y);
	if(mid+1<=y)
		ask(root*2+1,mid+1,r,x,y);
}
void change(int root,int l,int r,int x,int y,int k){
	if(l>=x&&r<=y){
		st[root].tag[k]^=1;
		st[root].b[k]=r-l+1-st[root].b[k];
		return;
	}
	push_down(root,l,r);
	int mid=(l+r)/2;
	if(mid>=x)
		change(root*2,l,mid,x,y,k);
	if(mid+1<=y)
		change(root*2+1,mid+1,r,x,y,k);
	for(int i=0;i<10;i++)
		st[root].b[i]=st[root*2].b[i]+st[root*2+1].b[i];
}
signed main(){
	n=read();
	m=read();
	for(int i=1;i<n;i++){
		int x,y,z;
		x=read();
		y=read();
		z=read();
		add(x,y,z);
		add(y,x,z);
	}
	dfs1(1,1);
	dfs2(1,1);
	build(1,1,n);
	int p[15];
	for(int i=1;i<=m;i++){
		int opt;
		opt=read();
		if(opt==1){
			int x,y;
			x=read();
			y=read();
			int xx=x,yy=y;
			memset(p,0,sizeof(p));
			while(tp[x]!=tp[y]){
				if(dep[tp[x]]<dep[tp[y]])
					swap(x,y);
				memset(res,0,sizeof(res));
				ask(1,1,n,dfn[tp[x]],dfn[x]);
				for(int i=0;i<=10;i++)
					p[i]+=res[i];
				x=fa[tp[x]];
			}
			memset(res,0,sizeof(res));
			if(dep[x]>dep[y])
				swap(x,y);
			ask(1,1,n,dfn[x],dfn[y]);
			for(int i=0;i<=10;i++)
				p[i]+=res[i];
			int LCA=lca(xx,yy);
			int ans=0;
			for(int i=0;i<=10;i++)
				ans+=(1<<i)*p[i]*(dep[xx]+dep[yy]-2*dep[LCA]+1-p[i]);
			printf("%lld\n",ans);
		}
		else{
			int x,y,z;
			x=read();
			y=read();
			z=read();
			int c;
			if(dep[x]>dep[y])
				c=x;
			else
				c=y;
			for(int i=0;i<=10;i++){
				if(((1<<i)&in[c])!=((1<<i)&z))
					change(1,1,n,dfn[c],dfn[c]+sz[c]-1,i);
			}
			in[c]=z;
		}
	}
}