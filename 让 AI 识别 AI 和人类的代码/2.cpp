// 2.cpp
#include<bits/stdc++.h>
#define int long long
#define lson(root) (root << 1)
#define rson(root) (root << 1 | 1)
using namespace std;
const int mod = 998244353;
struct node
{
	int sum , num;
}tree[800010];
int n , a[200010] , lst , ans;
int fast_mi(int x , int y)
{
	int ans = 1 , qwq = x;
	while(y)
	{
		if(y & 1) ans *= qwq , ans %= mod;
		y >>= 1 , qwq *= qwq , qwq %= mod;
	}
	return ans;
}
void pushup(int root)
{
	tree[root].sum = tree[lson(root)].sum + tree[rson(root)].sum;
	tree[root].num = tree[lson(root)].num + tree[rson(root)].num;
}
void update(int root , int l , int r , int pos)
{
	if(l == r) return tree[root].sum += l , tree[root].num ++ , void();
	int mid = (l + r) >> 1;
	if(pos <= mid) update(lson(root) , l , mid , pos);
	else update(rson(root) , mid + 1 , r , pos);
	pushup(root);
}
int query(int root , int l , int r , int L , int R)
{
	if(L <= l && R >= r) return tree[root].sum;
	int mid = (l + r) >> 1 , ans = 0;
	if(L <= mid) ans += query(lson(root) , l , mid , L , R) , ans %= mod;
	if(R > mid) ans += query(rson(root) , mid + 1 , r , L , R) , ans %= mod;
	return ans;
}
int query_(int root , int l , int r , int L , int R)
{
	if(L <= l && R >= r) return tree[root].num;
	int mid = (l + r) >> 1 , ans = 0;
	if(L <= mid) ans += query_(lson(root) , l , mid , L , R) , ans %= mod;
	if(R > mid) ans += query_(rson(root) , mid + 1 , r , L , R) , ans %= mod;
	return ans;
}
signed main()
{
	scanf("%lld" , &n);
	for(int i = 1 ; i <= n ; i ++) scanf("%lld" , &a[i]);
	for(int x = 1 ; x <= n ; x ++)
	{
		int invx = fast_mi(x , mod - 2);
		int invxx = invx * invx % mod;
		int val = a[x];
		ans = (lst + (query_(1 , 1 , 200000 , 1 , val) * a[x] % mod + query(1 , 1 , 200000 , val + 1 , 200000)) * 2 % mod + a[x]) % mod;
		lst = ans;
		printf("%lld\n" , ans * invxx % mod);
		update(1 , 1 , 200000 , a[x]);
	}
	return 0;
}
// 2.cpp