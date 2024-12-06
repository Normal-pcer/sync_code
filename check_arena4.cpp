#include<bits/stdc++.h>
#define rg register
#define file(x) freopen(x".in","r",stdin);freopen(x".out","w",stdout);
using namespace std;
int read() {
	int x=0,f=1;
	char c=getchar();
	while(c<'0'||c>'9') f=(c=='-')?-1:1,c=getchar();
	while(c>='0'&&c<='9') x=x*10+c-48,c=getchar();
	return f*x;
}
struct node {
	int l,r,v;
} a[100001*100];
int tot,root[100010];
void update(int &k,int l,int r,int pos,int v) {
	if(k==0)
		k=++tot;
	a[k].v=max(a[k].v,v);
	if(l==r)
		return ;
	int mid=(l+r)>>1;
	if(pos<=mid) update(a[k].l,l,mid,pos,v);
	else update(a[k].r,mid+1,r,pos,v);
}
int find(int k,int l,int r,int begin,int end) {
	if(k==0)
		return 0;
	if(l==begin&&r==end)
		return a[k].v;
	int mid=(l+r)>>1;
	if(end<=mid)
		return find(a[k].l,l,mid,begin,end);
	else if(begin>mid)
		return find(a[k].r,mid+1,r,begin,end);
	else return max(find(a[k].l,l,mid,begin,mid),find(a[k].r,mid+1,r,mid+1,end));
}
#define lowbit(x) (x&(-x))
void add(int x,int y,int v) {
	while(x<=100005)
		update(root[x],1,100005,y,v),x+=lowbit(x);
}
int sum(int x,int y) {
	int js=0;
	while(x)
		js=max(find(root[x],1,100005,1,y),js),x-=lowbit(x);
	return js;
}
int b[100010],minx[100010],maxx[100010];
int main() {
	int n=read(),m=read(),x,y,ans=0;
	for(int i=1; i<=n; i++)
		b[i]=minx[i]=maxx[i]=read();
	for(int i=1; i<=m; i++)
		x=read(),y=read(),maxx[x]=max(maxx[x],y),minx[x]=min(minx[x],y);
	for(int i=1; i<=n; i++) {
		int js=sum(minx[i],b[i])+1;
		ans=max(ans,js);
		add(b[i],maxx[i],js);
	}
	cout<<ans;
}