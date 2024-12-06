#include<bits/stdc++.h>
using namespace std;
#define MAXN 100005
namespace XDS{
	#define lson(x) ((x)<<1)
	#define rson(x) (((x)<<1)+1)
	int a[MAXN*4];
	void modify(int x,int y,int idx,int v,int p){
		if(x==y){
			a[p]=(v==-1?0:max(a[p],v));
			return;
		}
		int mid=(x+y)>>1;
		if(idx<=mid)modify(x,mid,idx,v,lson(p));
		if(idx>mid)modify(mid+1,y,idx,v,rson(p));
		a[p]=max(a[lson(p)],a[rson(p)]);
	}
	int query(int x,int y,int l,int r,int p){
		if(l<=x&&r>=y)
			return a[p];
		int mid=(x+y)>>1,ans=0;
		if(l<=mid)ans=max(ans,query(x,mid,l,r,lson(p)));
		if(r>mid)ans=max(ans,query(mid+1,y,l,r,rson(p)));
		return ans;
	}
}
int mx,n,m;
int s1[MAXN],s2[MAXN],a[MAXN];
int idx[MAXN],b[MAXN];
int F[MAXN];
bool cmp_s1(int x,int y){return s1[x]<s1[y];}
bool cmp_a(int x,int y){return a[x]<a[y];}
void cdq(int l,int r){
	if(l==r)return;
	int mid=(l+r)>>1;
	cdq(l,mid);
	for(int i=mid+1;i<=r;++i)
		b[i]=idx[i];
	sort(idx+mid+1,idx+r+1,cmp_a);
	int tot1=l,tot2=mid+1;
	for(int i=l;i<=r;++i){
		if((s1[idx[tot1]]<=a[idx[tot2]]&&tot1<=mid)||tot2>r){
			XDS::modify(1,mx,a[idx[tot1]],F[idx[tot1]],1);
			tot1++;
		}
		else{
			F[idx[tot2]]=max(F[idx[tot2]],XDS::query(1,mx,1,s2[idx[tot2]],1)+1);
			tot2++;
		}
	}
	for(int i=l;i<=mid;++i)
		XDS::modify(1,mx,a[idx[i]],-1,1);
	for(int i=mid+1;i<=r;++i)
		idx[i]=b[i];
	cdq(mid+1,r);
	sort(idx+l,idx+r+1,cmp_s1);
}
#define insert(x,i) (s1[i]=max(s1[i],x),s2[i]=min(s2[i],x),mx=max(mx,x))
int main()
{
	cin>>n>>m;
	memset(s2,0x7f,sizeof(s2));
	for(int i=1;i<=n;++i){
		scanf("%d",&a[i]);
		insert(a[i],i);
		idx[i]=i;
	}	
	int x,y;
	while(m--){
		scanf("%d%d",&x,&y);
		insert(y,x);
	}
	for(int i=1;i<=n;++i)
		F[i]=1;
	cdq(1,n);
	int ans=0;
	for(int i=1;i<=n;++i)
		ans=max(ans,F[i]);
	cout<<ans;
}