#include <bits/stdc++.h>
#define int long long
using namespace std;
int k;
string s,t;
int f[500010][45];
int&dp(int i,int j){return f[i][i-j+22];}//通过传引用的方式实现dp与f的转化
void solve(){
	cin>>k>>s>>t;
	int n=s.size(),m=t.size();
	if(abs(n-m)>k){
		cout<<"No";
		return;
	}
	memset(f,0x3f,sizeof(f));
	for(int i=0;i<=k;i++){
		dp(0,i)=i;
		dp(i,0)=i;
	}
	for(int i=1;i<=n;i++)for(int j=max(1ll,i-k);j<=min(i+k,m);j++){
		dp(i,j)=min({dp(i-1,j)+1,dp(i,j-1)+1,dp(i-1,j-1)+(s[i-1]!=t[j-1])});
	}
	if(dp(n,m)<=k)cout<<"Yes";
	else cout<<"No";
}

signed main() {
	solve();
}