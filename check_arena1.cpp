#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=2e5+5;
struct tree{
	vector<int> e[N];
	int n;
	int u,v;
	int mxl;
	int dep[N];
	void dfs(int p,int fa){
		dep[p]=dep[fa]+1;
		for(int t:e[p])
			if(t!=fa)dfs(t,p);
	}
	int a[N],b[N];
	void init(){
		scanf("%d",&n);int x,y;
		for(int i=1;i<n;++i){
			scanf("%d%d",&x,&y);
			e[x].push_back(y);
			e[y].push_back(x);
		}
		dep[0]=-1;
		dfs(1,0);
		for(int i=1;i<=n;++i)
			if(dep[u]<dep[i])u=i;
		dfs(u,0);
		for(int i=1;i<=n;++i)
			a[i]=dep[i];
		for(int i=1;i<=n;++i)
			if(dep[v]<dep[i])v=i;
		dfs(v,0);
		for(int i=1;i<=n;++i){
			b[i]=dep[i];
			if(i==u)mxl=dep[i];
			if(a[i]<b[i])swap(a[i],b[i]);
		}
	}
}t1,t2;
ll s[N],c[N];
int main(){
	t1.init(),t2.init();
	if(t1.mxl<t2.mxl)
		swap(t1,t2);
	
	
	for(int i=1;i<=t2.n;++i){
		t2.a[i]++;
		s[t2.a[i]]+=t2.a[i];
		c[t2.a[i]]++;
	}
	int mx=max(t1.n,t2.n+1);
	for(int i=1;i<=mx;++i)
		s[i]+=s[i-1],c[i]+=c[i-1];
	
	ll ans=0;
	
//	for(int i=1;i<=t1.n;++i)
//		for(int j=1;j<=t2.n;++j)
//			ans+=max(t1.mxl,t1.a[i]+t2.a[j]);
	
	for(int i=1;i<=t1.n;++i){
		auto tmp = ans;
		int x=t1.mxl-t1.a[i];
		ans+=(ll)t1.mxl*c[x];
		ans+=(ll)t1.a[i]*(c[mx]-c[x])+(s[mx]-s[x]);
		//max(t1.a[i]+t1.b[i],t1.a[i]+t2.a[j]+1)
		std::cerr << "i = " << i << ": " << ans - tmp << std::endl;
	}
	cout<<ans;
}
