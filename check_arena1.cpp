#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

struct Tree {
	int ls,rs,cnt;
};

struct Robot{
	int x,r,q;
};

int n,k,len,tot;
Robot a[500001];
int b[500001];
map<int,int> rt;
Tree tr[20000001];

int update(int i,int l,int r,int pos,int k) {
	if (i==0) i=++tot;
	tr[i].cnt+=k;
	if (l==r) return i;
	int mid=(l+r)>>1;
	if (pos<=mid) tr[i].ls=update(tr[i].ls,l,mid,pos,k);
	else tr[i].rs=update(tr[i].rs,mid+1,r,pos,k);
	return i;
}

int query(int i,int l,int r,int ql,int qr){
	if (i==0) return 0;
	if (l>=ql and r<=qr) return tr[i].cnt;
	int sum=0,mid=(l+r)>>1;
	if (ql<=mid) sum+=query(tr[i].ls,l,mid,ql,qr);
	if (mid<qr) sum+=query(tr[i].rs,mid+1,r,ql,qr);
	return sum;
}

int find(int x){
	return lower_bound(b+1,b+len+1,x)-b;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
	cin>>n>>k;
	for (int i=1;i<=n;i++){
		cin>>a[i].x>>a[i].r>>a[i].q;
		b[++len]=a[i].x;
		b[++len]=a[i].x+a[i].r;
		b[++len]=a[i].x-a[i].r;
	}
	sort(a+1,a+n+1,[](Robot u,Robot v){return u.r>v.r;});
	sort(b+1,b+len+1);
	len=unique(b+1,b+len+1)-b-1;
	ll sum=0;
	for (int i=1;i<=n;i++){
		int x=find(a[i].x),l=find(a[i].x-a[i].r),r=find(a[i].x+a[i].r);
		for (int j=a[i].q-k;j<=a[i].q+k;j++){
			if (rt.find(j)!=rt.end()){
				sum+=query(rt[j],1,len,l,r);
			}
		}
		rt[a[i].q]=update(rt[a[i].q],1,len,x,1);
	}
	cout<<sum;
	return 0;
}