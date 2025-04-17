#include <bits/stdc++.h>
using namespace std;
#define ll long long
const ll N = 1e5 + 10;
ll n, m, q, a, b, fa[N];
set<ll> s[N], t[N];
ll find(ll x){
	return fa[x] == x ? x : fa[x] = find(fa[x]);
}
void merge(ll a, ll b){
	n--;
	for(auto i : s[b]){
		if(s[i].size() < n) s[i].insert(a);
		s[i].erase(b);
		if(s[a].size() < n) s[a].insert(i);
	}
	s[b].clear(), fa[b] = a;
}
int main(){
//	freopen("1.in", "r", stdin);
//	freopen("1.out", "w", stdout);
	ios::sync_with_stdio(false);
	cin.tie(0); cout.tie(0);
	cin >> n >> m >> q;
	for(ll i = 1; i <= n; i++) fa[i] = i;
	for(ll i = 1; i <= m; i++) cin >> a >> b, s[a].insert(b), s[b].insert(a);
	while(q--){
		cin >> a >> b;
		a = find(a), b = find(b);
		if(!s[a].count(b) && a != b){
			if(s[a].size() < s[b].size()) merge(b, a);
			else merge(a, b);
			cout << "agree." << endl;
		}else cout << "no!" << endl;
	}
	return 0;
}