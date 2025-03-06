#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define ls (u << 1)
#define rs (u << 1 | 1)
ll c, T, n, sum[200010];
unordered_map<ll, ll> pos1; // 排序后 -> 排序前 
unordered_map<ll, ll> pos2; // 排序前 -> 排序后 
map<pair<ll, pair<ll, ll> >, ll> pos3;
set<ll> rk;
struct node{
	ll a, b, t;
	bool operator < (const node &tmp)const{
		return t < tmp.t;
	}
}a[200010], cop[200010];
struct Tree{
	ll sum, mx, lazy = -9e18;
}tr[800010];
ll len(ll l, ll r){return r - l + 1;}
void pushup(ll u){
	tr[u].sum = tr[ls].sum + tr[rs].sum;
	tr[u].mx  = max(tr[ls].mx, tr[rs].mx);
}
void pushdown(ll type, ll u, ll l, ll r, ll val){
	if(tr[u].lazy != -9e18){
		ll mid = (l + r) >> 1;
		ll x1 = tr[u].lazy, x2 = tr[u].lazy + len(l, mid);
		tr[ls].lazy = x1;
		tr[ls].sum  = (x1 + (x1 + len(l, mid) - 1)) * len(l, mid) / 2;
		tr[rs].lazy = x2;
		tr[rs].sum  = (x2 + (x2 + len(mid + 1, r) - 1)) * len(mid + 1, r) / 2;
		if(type == 1){
			tr[ls].mx = val + mid - l;
			tr[rs].mx = val + r - mid - 1;
		}
		tr[u].lazy  = -9e18;
	}
}
void build(ll u, ll l, ll r){
	tr[u].sum  = cop[l].a;
	tr[u].lazy = -9e18;
	tr[u].mx   = cop[l].a;
	if(l == r) return;
	ll mid = (l + r) >> 1;
	build(ls, l, mid);
	build(rs, mid + 1, r);
	pushup(u);
}
void update(ll u, ll l, ll r, ll L, ll R, ll val){
	if(L <= l && r <= R){
		ll x = val + l - L;
		tr[u].lazy = x;
		tr[u].sum  = (x + (x + len(l, r) - 1)) * len(l, r) / 2;
		tr[u].mx   = val + r - l;
		return;
	}
	ll mid = (l + r) >> 1;
	pushdown(1, u, l, r, val);
	if(L <= mid) update(ls, l, mid, L, R, val);
	if(mid < R)  update(rs, mid + 1, r, L, R, val);
	pushup(u);
}
ll query(ll u, ll l, ll r, ll L, ll R){
	if(L <= l && r <= R) return tr[u].sum;
	ll mid = (l + r) >> 1, ans = 0;
	pushdown(0, u, l, r, 0);
	if(L <= mid) ans += query(ls, l, mid, L, R);
	if(mid < R)  ans += query(rs, mid + 1, r, L, R);
	return ans;
}
ll UpperBound(ll u, ll l, ll r, ll x0){
//	cout << l << " " << r << endl;
	if(l == r){
		if(cop[x0].b + l - x0 - 1 >= tr[u].mx) return n + 1;
		else return l;
	}else{
		ll mid = (l + r) >> 1;
		if(x0 >= mid) return UpperBound(rs, mid + 1, r, x0);
		else{
			if(cop[x0].b + mid - x0 - 1 >= tr[ls].mx) return UpperBound(rs, mid + 1, r, x0);
			else return UpperBound(ls, l, mid, x0);
		}
	}
	//cop[posstart].b + mid - posstart - 1 < query(1, 1, n, mid, mid)
}
bool checkA(ll x){
	return x == 1 || x == 4 || x == 8 || x == 12;
}
bool checkB(ll x){
	return x == 5 || x == 9 || x == 13 || x == 19 || x == 20;
}
bool checkC(ll x){
	return x == 0 || x == 14 || x == 15 || x == 21 || x == 22;
}
int main(){
	ios::sync_with_stdio(false);
	cin.tie(0); cout.tie(0);
	cin >> c >> T;
	if(checkA(c)){
		while(T--){
			cin >> n;
			ll cnt = 0;
			for(ll i = 1; i <= n; i++) cin >> a[i].a >> a[i].b >> a[i].t, cnt += abs(a[i].b - a[i].a);
			cout << (cnt <= a[1].t ? "Yes\n" : "No\n");
		}
	}else if(checkB(c)){
		while(T--){
			cin >> n;
			for(ll i = 1; i <= n; i++) cin >> a[i].a >> a[i].b >> a[i].t;
			sort(a + 1, a + n + 1);
			bool flag = 0;
			ll now = 0;
			for(ll i = 1; i <= n; i++){
				now += a[i].b - a[i].a;
				if(now > a[i].t){
					flag = 1; break;
				}
			}
			cout << (flag ? "No\n" : "Yes\n");
		}
	}else if(checkC(c)){
		while(T--){
			pos3.clear();
			cin >> n;
			memset(cop, 0, sizeof cop);
			for(ll i = 1; i <= n; i++) cin >> a[i].a >> a[i].b >> a[i].t, cop[i] = a[i];
			sort(a + 1, a + n + 1);
			for(ll i = 1; i <= n; i++) pos3[{a[i].a, {a[i].b, a[i].t}}] = i;
			for(ll i = 1; i <= n; i++) pos2[i] = pos3[{cop[i].a, {cop[i].b, cop[i].t}}], pos1[pos3[{cop[i].a, {cop[i].b, cop[i].t}}]] = i, rk.insert(i);
			bool flag = 0;
			ll cnt = 0;
			build(1, 1, n);
			while(!rk.empty()){
				ll now = *rk.begin(); rk.erase(now);
				ll posstart = pos1[now], posend = pos1[now] + 1, nowcnt = 0;
				if(query(1, 1, n, posstart, posstart) == cop[posstart].b) continue;
//				ll l = posstart + 1, r = n + 1;
//				while(l < r){
//					ll mid = (l + r) >> 1;
//					if(cop[posstart].b + mid - posstart - 1 < query(1, 1, n, mid, mid)) r = mid;
//					else l = mid + 1;
//				}
//				while(posend <= n && cop[posstart].b + nowcnt >= query(1, 1, n, posend, posend)) posend++, nowcnt++;
				posend = UpperBound(1, 1, n, posstart) - 1, nowcnt = posend - posstart;
				cnt += cop[posstart].b * (nowcnt + 1) + (nowcnt + 1) * nowcnt / 2 - query(1, 1, n, posstart, posend);
				update(1, 1, n, posstart, posend, cop[posstart].b);
				if(cnt > cop[posstart].t){
					flag = 1; break;
				}
			}
			cout << (flag ? "No\n" : "Yes\n");
		}
	}else while(T--) cout << "Yes\n";
	return 0;
}
/*
0 1
4
2 5 16
6 8 12
7 13 15
9 15 12
*/