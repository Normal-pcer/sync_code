// 1.cpp
#include<bits/stdc++.h>
#include<bits/extc++.h>
using namespace std;
using namespace __gnu_pbds;
typedef long long ll;
constexpr int mod=998244353;
ll qpow(ll a,ll b){
	ll res=1;
	for (;b;b>>=1,a=a*a%mod) if (b&1) res=res*a%mod;
	return res;
}
int n;
ll a[200001],tr[300001];
tree<pair<int,int>,null_type,less<pair<int,int>>,rb_tree_tag,tree_order_statistics_node_update> s;
int lowbit(int x){
	return x&(-x);
}
void add(int x,int c){
	for (;x;x-=lowbit(x)) tr[x]=(tr[x]+c)%mod;
}
ll query(int x){
	ll sum=0;
	for (;x<=300000;x+=lowbit(x)) sum=(sum+tr[x])%mod;
	return sum;
}
int main(){
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.tie(nullptr);
	cin>>n;
	for (int i=1;i<=n;i++) cin>>a[i];
	ll sum=0;
	for (int i=1;i<=n;i++){
		sum=(sum+a[i])%mod;
		sum=(sum+a[i]*s.order_of_key({a[i],i})*2)%mod;
		sum=(sum+query(a[i]+1)*2%mod);
		s.insert({a[i],i});
		add(a[i],a[i]);
		cout<<sum*qpow(ll(i)*i%mod,mod-2)%mod<<"\n";
	}
	return 0;
}
// 1.cpp