#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define ls tr[i].lson
#define rs tr[i].rson
int n,m;
int h[100001],cnt;
struct seg {
	int siz,lson,rson;
	ll sum;
} tr[3200001];
void update(int &i,int l,int r,int pos,int v) {
	if (!i) i=++cnt;
	tr[i].siz+=v,tr[i].sum+=v*pos;
	if (l==r) return;
	int mid=(l+r)>>1;
	if (pos<=mid) update(ls,l,mid,pos,v);
	else update(rs,mid+1,r,pos,v);
}
ll siz,sum;
ll query(int i,int l,int r,ll lim) {
	if (!i) return l;
	if (l==r) return siz+=tr[i].siz,sum+=tr[i].sum,l;
	ll mid=((l+r)>>1)+1;
	ll res=mid*(tr[ls].siz+siz)-(tr[ls].sum+sum);
	if (res>=lim) return query(ls,l,mid-1,lim);
	else return siz+=tr[ls].siz,sum+=tr[ls].sum,query(rs,mid,r,lim);
}
int rt;
int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
	cin>>n>>m;
	for (int i=1; i<=n; i++) {
		cin>>h[i];
		update(rt,0,1e9,h[i],1);
	}
	while (m--) {
		ll op,l,r;
		cin>>op>>l;
		if (op==1) {
			cin>>r;
			update(rt,0,1e9,h[l],-1);
			update(rt,0,1e9,r,1);
			h[l]=r;
		} else {
			siz=sum=0;
			int height=query(rt,0,1e9,l);
			l-=siz*height-sum;
			cout<<fixed<<setprecision(5)<<height+1.0*l/siz<<"\n";
		}
	}
	return 0;
}