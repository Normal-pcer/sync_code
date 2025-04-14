#include <bits/stdc++.h>
using namespace std;
#define ll long long
struct node{
	ll x, y, ans, dir;
	node(ll x, ll y, ll ans, ll dir):x(x), y(y), ans(ans), dir(dir){}
	bool operator < (const node &tmp)const{
		return ans > tmp.ans;
	}
};
// 0 -> E, 1 -> W, 2 -> S, 3 -> N
/*
   N
   ^
   |
W<--->E
   |
   v
   S
*/
ll lft[4], rgt[4], rtn[4];
ll dx[4] = {0, 0, 1, -1};
ll dy[4] = {1, -1, 0, 0};
char a[40][40];
bool vis[40][40][4];
ll n, m, sx, sy, dir, ex, ey;
void init(){
	lft[0] = 3, lft[1] = 2, lft[2] = 0, lft[3] = 1;
	rgt[0] = 2, rgt[1] = 3, rgt[2] = 1, rgt[3] = 0;
	rtn[0] = 1, rtn[1] = 0, rtn[2] = 3, rtn[3] = 2;
}
bool check(ll x, ll y){
	return x < 1 || x > n || y < 1 || y > m || a[x][y] == '.';
}
bool Check(ll x, ll y, ll dir){
//	cerr << "Check " << x << ' ' << y << ' ' << dir << endl;
	for(ll i = 0; i < 4; i++){
		if(i == rtn[dir]) continue;
		if(!check(x + dx[i], y + dy[i])) return /* cerr << "failed on " << i << endl,*/ 0;
	}
	return 1;
}
ll bfs(){
	priority_queue<node> q;
	q.push({sx, sy, 0, dir});
	while(q.size()){
		node cur = q.top(); q.pop();
//		cerr << cur.x << ' ' << cur.y << ' ' << cur.ans << endl;
		if(cur.x == ex && cur.y == ey) return cur.ans;
		vis[cur.x][cur.y][dir] = 1;
		for(ll i = 0; i < 4; i++){
			ll _x = cur.x + dx[i];
			ll _y = cur.y + dy[i];
			ll _dir = i;
			if(_x > 0 && _x <= n && _y > 0 && _y <= m && !vis[_x][_y][_dir] && a[_x][_y] != '.'){
				if(i == cur.dir) q.push({_x, _y, cur.ans, _dir});
				if(i == lft[cur.dir]) q.push({_x, _y, cur.ans + 1, _dir});
				if(i == rgt[cur.dir]) q.push({_x, _y, cur.ans + 5, _dir});
				if(i == rtn[cur.dir] && Check(cur.x, cur.y, cur.dir)) q.push({_x, _y, cur.ans + 10, _dir});
			}
		}
	}
	return -1;
}
int main(){
	ios::sync_with_stdio(false);
	cin.tie(0); cout.tie(0);
	cin >> n >> m;
	init();
	for(ll i = 1; i <= n; i++) for(ll j = 1; j <= m; j++){
		cin >> a[i][j];
		if(a[i][j] == 'E') sx = i, sy = j, dir = 0;
		if(a[i][j] == 'W') sx = i, sy = j, dir = 1;
		if(a[i][j] == 'S') sx = i, sy = j, dir = 2;
		if(a[i][j] == 'N') sx = i, sy = j, dir = 3;
		if(a[i][j] == 'F') ex = i, ey = j;
	}
	cout << bfs() << endl;
	return 0;
}