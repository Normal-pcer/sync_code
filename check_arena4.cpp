#include<bits/stdc++.h>
#define int long long
#define lson(root) (root << 1)
#define rson(root) ((root << 1) | 1)
using namespace std;
struct node1
{
	int x1 , x2 , y1 , y2 , xx1 , xx2 , yy1 , yy2;
}nd[4000010];
struct node2
{
	int x , y , xx , yy , id , flag;
	bool operator < (const node2 &qwq) const
	{
		if(x == qwq.x) return y < qwq.y; 
		return x < qwq.x;
	}
}nw[4000010];
int tr[4000010];
int n , p , m , dp[4000010] , ans;
int lowbit(int x)
{
	return x&(-x);
}
void update(int x , int y)
{
	for (;x<=m;x+=lowbit(x)) tr[x]=max(tr[x],y);
}
int query(int x)
{
	int ans=0;
	for (;x;x-=lowbit(x)) ans=max(ans,tr[x]);
	return ans;
}
void Discretization()
{
	vector<int> v;
	for(int i = 1 ; i <= p ; i ++) v.push_back(nd[i].x1) , v.push_back(nd[i].x2);
	sort(v.begin() , v.end());
	for(int i = 1 ; i <= p ; i ++) nd[i].xx1 = lower_bound(v.begin() , v.end() , nd[i].x1) - v.begin() + 1;
	for(int i = 1 ; i <= p ; i ++) nd[i].xx2 = lower_bound(v.begin() , v.end() , nd[i].x2) - v.begin() + 1;
	v.clear();
	for(int i = 1 ; i <= p ; i ++) v.push_back(nd[i].y1) , v.push_back(nd[i].y2);
	sort(v.begin() , v.end());
	for(int i = 1 ; i <= p ; i ++) nd[i].yy1 = lower_bound(v.begin() , v.end() , nd[i].y1) - v.begin() + 1;
	for(int i = 1 ; i <= p ; i ++) nd[i].yy2 = lower_bound(v.begin() , v.end() , nd[i].y2) - v.begin() + 1;
}
void split()
{
	for(int i = 1 ; i <= p ; i ++)
	{
		m ++ , nw[m].x = nd[i].x1 , nw[m].y = nd[i].y1 , nw[m].xx = nd[i].xx1 , nw[m].yy = nd[i].yy1 , nw[m].id = i , nw[m].flag = 0;
		m ++ , nw[m].x = nd[i].x2 , nw[m].y = nd[i].y2 , nw[m].xx = nd[i].xx2 , nw[m].yy = nd[i].yy2 , nw[m].id = i , nw[m].flag = 1;
	}
	sort(nw + 1 , nw + m + 1);
}
void DP()
{
	for(int i = 1 ; i <= m ; i ++)
	{
		int idx = nw[i].id;
		if(nw[i].flag == 0) dp[idx] = query(nw[i].yy);
		else dp[idx] += (nd[idx].x2 - nd[idx].x1 + nd[idx].y2 - nd[idx].y1) , update(nw[i].yy , dp[idx]);
	}
}
bool check(int i){return nd[i].x2 <= n && nd[i].y2 <= n;}
signed main()
{
 	//freopen("P6007_4.in" , "r" , stdin);
	scanf("%lld%lld" , &n , &p);
	for(int i = 1 ; i <= p ; i ++) scanf("%lld%lld%lld%lld" , &nd[i].x1 , &nd[i].y1 , &nd[i].x2 , &nd[i].y2);
	Discretization();
	split();
	DP();
	for(int i = 1 ; i <= p ; i ++) if(check(i)) ans = max(ans , dp[i]);
	printf("%lld" , 2 * n - ans);
	return 0;
}