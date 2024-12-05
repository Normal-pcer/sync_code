#if 1
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

const int inf=1000000000;

struct Node{
	int ls,rs;
	ll maxn;
};

struct T{
	ll x,y;
};

int n,m,cnt,rt;
int id[200010];
ll f[200010];
Node tr[6400000];
T a[200010];

void pushup(int i){
	tr[i].maxn=max(tr[tr[i].ls].maxn,tr[tr[i].rs].maxn);
}

void update(int &i,int l,int r,int x,ll k){
	if (i==0) i=++cnt;
	if (l==r){
		tr[i].maxn=max(tr[i].maxn,k);
		return;
	}
	int mid=(l+r)>>1;
	if (x<=mid) update(tr[i].ls,l,mid,x,k);
	else update(tr[i].rs,mid+1,r,x,k);
	pushup(i);
}

ll query(int i,int l,int r,int ql,int qr){
	if (i==0) return 0;
	if (l>=ql and r<=qr) return tr[i].maxn;
	ll ans=0;
	int mid=(l+r)>>1;
	if (mid>=ql) ans=max(ans,query(tr[i].ls,l,mid,ql,qr));
	if (mid+1<=qr) ans=max(ans,query(tr[i].rs,mid+1,r,ql,qr));
	return ans;
}

int main(){
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.tie(nullptr);
	cin>>m>>n;
	id[1]=1;
	for (int i=1;i<=n;i++){
		cin>>a[i<<1].x>>a[i<<1].y>>a[i<<1|1].x>>a[i<<1|1].y;
		id[i<<1]=(i<<1);
		id[i<<1|1]=(i<<1|1);
	}
	n++;
	a[n<<1].x=a[n<<1].y=a[n<<1|1].x=a[n<<1|1].y=m;
	id[n<<1]=(n<<1);
	id[n<<1|1]=(n<<1|1);
	sort(id+1,id+(n<<1)+2,[](int u,int v){return a[u].x==a[v].x?a[u].y<a[v].y:a[u].x<a[v].x;});
	for (int i=0;i<=(n<<1|1);i++){
		if (id[i]&1){
			f[id[i]]=f[id[i]^1]+a[id[i]].x+a[id[i]].y-a[id[i]^1].x-a[id[i]^1].y;
		}else{
			f[id[i]]=query(rt,0,inf,0,a[id[i]].y);			
		}
		update(rt,0,inf,a[id[i]].y,f[id[i]]);
	}
	cout<<(m<<1)-f[n<<1|1];
	return 0;
}
#else
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
struct node
{
	int mx;
}tree[4000010];
int n , p , m , dp[4000010] , ans;
void pushup(int root)
{
	tree[root].mx = max(tree[lson(root)].mx , tree[rson(root)].mx);
}
void update(int root , int l , int r , int x , int y)
{
	if(l == r)
	{
		tree[root].mx = max(tree[root].mx , y);
		return;
	}
	int mid = (l + r) >> 1;
	if(x <= mid) update(lson(root) , l , mid , x , y);
	else update(rson(root) , mid + 1 , r , x , y);
	pushup(root);
}
int query(int root , int l , int r , int L , int R)
{
	if(L <= l && R >= r) return tree[root].mx;
	int mid = (l + r) >> 1 , ans = 0;
	if(L <= mid) ans = max(ans , query(lson(root) , l , mid , L , R));
	if(R > mid) ans = max(ans , query(rson(root) , mid + 1 , r , L , R));
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
		if(nw[i].flag == 0) dp[idx] = query(1 , 1 , 2 * m , 1 , nw[i].yy);
		else dp[idx] += (nd[idx].x2 - nd[idx].x1 + nd[idx].y2 - nd[idx].y1) , update(1 , 1 , 2 * m , nw[i].yy , dp[idx]);
	}
}
bool check(int i){return nd[i].x2 <= n && nd[i].y2 <= n;}
signed main()
{
//  	freopen("P6007_4.in" , "r" , stdin);
	scanf("%lld%lld" , &n , &p);
	for(int i = 1 ; i <= p ; i ++) scanf("%lld%lld%lld%lld" , &nd[i].x1 , &nd[i].y1 , &nd[i].x2 , &nd[i].y2);
	Discretization();
	split();
	DP();
	for(int i = 1 ; i <= p ; i ++) if(check(i)) ans = max(ans , dp[i]);
	printf("%lld" , 2 * n - ans);
	return 0;
}
#endif