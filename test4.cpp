#include <bits/stdc++.h>
using namespace std;
#define ll long long
ll rnd(ll l, ll r){
	return rand() % (r - l + 1) + l;
}
void generate_data(){
	ll n = rnd(1, 100000), m = rnd(1, 100000), q = rnd(1, 100000);
	for(ll i = 1; i <= m; i++){
		ll x = rnd(1, n), y = rnd(1, n);
		out << x << ' ' << y << '\n';
	}
	for(ll i = 1; i <= q; i++){
		ll u = rnd(1, n), v = rnd(1, n);
		if(u == v) v = v % n + 1;
		out << u << ' ' << y << '\n';
	}
}
int main(){
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	
	return 0;
}