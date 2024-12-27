#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

struct Node{
	int ls,rs;
	ll cnt;
};

struct Point{
	int x,y;
	ll p;
	Point(){
		x=y=p=0;
	}
	Point(int _x){
		x=_x;
		y=p=0;
	}
};

bool cmp(Point u,Point v){
	return u.x<v.x;
}

int n,m,cnt;
int b[100001];
int rt[100001];
Point a[100001];
Node tr[10000000];

int clone(int u){
	tr[++cnt]=tr[u];
	return cnt;
}

void pushup(int i){
	tr[i].cnt=tr[tr[i].ls].cnt+tr[tr[i].rs].cnt;
}

void update(int &i,int u,int l,int r,int x,ll k){
	i=clone(u);
	if (l==r){
		tr[i].cnt+=k;
		return;
	}
	int mid=(l+r)>>1;
	if (mid>=x) update(tr[i].ls,tr[u].ls,l,mid,x,k);
	else update(tr[i].rs,tr[u].rs,mid+1,r,x,k);
	pushup(i);
}

ll query(int i,int l,int r,int ql,int qr){
	if (i==0) return 0;
	if (l>=ql and r<=qr) return tr[i].cnt;
	int mid=(l+r)>>1;
	ll ans=0;
	if (mid>=ql) ans+=query(tr[i].ls,l,mid,ql,qr);
	if (mid+1<=qr) ans+=query(tr[i].rs,mid+1,r,ql,qr);
	return ans;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
	cin>>n>>m;
	for (int i=1;i<=n;i++){
		cin>>a[i].x>>a[i].y>>a[i].p;
		b[i]=a[i].y;
	}
	sort(a+1,a+n+1,cmp);
	sort(b+1,b+n+1);
	int tot=unique(b+1,b+n+1)-b-1;
	for (int i=1;i<=n;i++){
		a[i].y=lower_bound(b+1,b+tot+1,a[i].y)-b;
	}
	for (int i=1;i<=n;i++){
		update(rt[i],rt[i-1],1,tot,a[i].y,a[i].p);
	}
	while (m--){
		int x1,y1,x2,y2;
		cin>>x1>>y1>>x2>>y2;
		x1=lower_bound(a+1,a+n+1,Point(x1),cmp)-a;
		x2=upper_bound(a+1,a+n+1,Point(x2),cmp)-a-1;
		y1=lower_bound(b+1,b+tot+1,y1)-b;
		y2=upper_bound(b+1,b+tot+1,y2)-b-1;
		cout<<query(rt[x2],1,tot,y1,y2)-query(rt[x1-1],1,tot,y1,y2)<<"\n";
	}
	return 0;
}