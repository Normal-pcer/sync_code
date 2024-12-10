#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

#define ls (i<<1)
#define rs (i<<1|1) 

struct Tree{
	int l,r;
	ll sum,lmax,rmax,smax;
	Tree(){
		l=r=sum=0;
		lmax=rmax=smax=-1e18;
	}
};

int n,q;
int a[10001];
Tree tr[32769];

void meg(Tree &ans,Tree l,Tree r){
	ans.sum=l.sum+r.sum;
	ans.lmax=max(l.lmax,l.sum+r.lmax);
	ans.rmax=max(r.rmax,r.sum+l.rmax);
	ans.smax=max({l.smax,r.smax,l.rmax+r.lmax});
}

void pushup(int i){
	meg(tr[i],tr[ls],tr[rs]);
}

void build(int i,int l,int r){
	tr[i].l=l;
	tr[i].r=r;
	if (l==r){
		tr[i].sum=tr[i].lmax=tr[i].rmax=tr[i].smax=a[l];
		return;
	}
	int mid=(l+r)>>1;
	build(ls,l,mid);
	build(rs,mid+1,r);
	pushup(i);
}

Tree query(int i,int l,int r){
	if (tr[i].l>=l and tr[i].r<=r) return tr[i];
	Tree ans;
	if (tr[ls].r>=l) ans=query(ls,l,r);
	if (tr[rs].l<=r) meg(ans,ans,query(rs,l,r));
	return ans;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
	int T;
	cin>>T;
	while (T--){
		cin>>n;
		for (int i=1;i<=n;i++) cin>>a[i];
		build(1,1,n);
		cin>>q;
		while (q--){
			int l1,r1,l2,r2;
			cin>>l1>>r1>>l2>>r2;
			if (r1<l2){
				cout<<query(1,l1,r1).rmax+query(1,r1+1,l2-1).sum+query(1,l2,r2).lmax<<"\n";
			}else{
				ll ans=query(1,l2,r1).smax;
				if (l1<l2) ans=max(ans,query(1,l1,l2).rmax+query(1,l2,r2).lmax-a[l2]);
				if (r2>r1) ans=max(ans,query(1,l1,r1).rmax+query(1,r1,r2).lmax-a[r1]);
				cout<<ans<<"\n";
			}
		}
	}
	return 0;
}