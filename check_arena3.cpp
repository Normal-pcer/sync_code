#include<bits/stdc++.h>
using namespace std;
#define MAXN 200015
int x[MAXN],y[MAXN];
int n,m;
int idx[MAXN];
int f[MAXN];
#define mid ((x+y)>>1)
int mx[MAXN*30],lson[MAXN*30],rson[MAXN*30],tot,rt;
void modify(int x,int y,int idx,int v,int &p){
	if(!p)p=++tot,mx[p]=0;mx[p]=max(mx[p],v);
	if(x==y)return mx[p]=max(mx[p],v),void();
	if(idx<=mid)modify(x,mid,idx,v,lson[p]);
	if(idx>mid)modify(mid+1,y,idx,v,rson[p]);
}
int query(int x,int y,int l,int r,int p){
    if(!p)return 0;
    if(l<=x&&r>=y)return mx[p];
	int ans=0;
	if(l<=mid)ans=max(ans,query(x,mid,l,r,lson[p]));
	if(r>mid)ans=max(ans,query(mid+1,y,l,r,rson[p]));
	return ans;
}
int main(){
	cin>>m>>n;x[0]=x[1]=0,y[0]=y[1]=0;
	idx[0]=0;idx[1]=1;
	for(int i=1;i<=n;++i){
		scanf("%d%d%d%d",&x[i*2],&y[i*2],&x[i*2+1],&y[i*2+1]);
		idx[i*2]=i*2;idx[i*2+1]=i*2+1;
	}
	++n;x[n*2]=x[n*2+1]=m;y[n*2]=y[n*2+1]=m;
	idx[n*2]=n*2;idx[n*2+1]=n*2+1;
	sort(idx,idx+n*2+2,[](int a,int b){return x[a]!=x[b]?x[a]<x[b]:y[a]<y[b];});
	for(int i=0;i<=n*2+1;++i){
		if(idx[i]%2==0)
			f[idx[i]]=query(0,1e9,0,y[idx[i]],rt);
		if(idx[i]%2==1)
			f[idx[i]]=f[idx[i]^1]+x[idx[i]]+y[idx[i]]-x[idx[i]^1]-y[idx[i]^1];
		modify(0,1e9,y[idx[i]],f[idx[i]],rt);
	}
	cout<<2*m-f[n*2+1];
}