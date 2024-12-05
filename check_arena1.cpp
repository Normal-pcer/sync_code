#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef double db;

struct Data{
	int maxn;
	db val;
	bool flag;
	Data(){
		maxn=0;
		val=1;
		flag=true;
	}
	Data(int _1,db _2,bool _3=true){
		maxn=_1;
		val=_2;
		flag=_3;
	}
};

inline void meg(Data &i,Data l,Data r){
	if (l.flag){
		i=r;
		return;
	}
	if (r.flag){
		i=l;
		return;
	}
	i=l;
	if (r.maxn>i.maxn){
		i=r;
	}else if (r.maxn==i.maxn){
		i.val+=r.val;
	}
}

namespace segin{
	struct Node{
		int ls,rs;
		Data val;
		Node(){
			ls=rs=0;
			val=Data();
		}
		Node(Data t){
			ls=rs=0;
			val=t;
		}
	};

	int cnt;
	int rt[1000000];
	Node tr[13000000];

	void update(int &i,int l,int r,int x,Data k){
		if (i==0) i=++cnt;
		meg(tr[i].val,tr[i].val,k);
		if (l==r) return;
		int mid=(l+r)>>1;
		if (mid>=x) update(tr[i].ls,l,mid,x,k);
		else update(tr[i].rs,mid+1,r,x,k);
	}

	Data query(int i,int l,int r,int ql,int qr){
		if (i==0) return Data();
		if (l>=ql and r<=qr) return tr[i].val;
		int mid=(l+r)>>1;
		Data ans;
		if (mid>=ql) ans=query(tr[i].ls,l,mid,ql,qr);
		if (mid+1<=qr) meg(ans,ans,query(tr[i].rs,mid+1,r,ql,qr));
		return ans;
	}
}

struct Node{
	int ls,rs;
};

int n,cnt,rt,cnth,cntv;
int h[50001],v[50001],a[50001],b[50001],f[50001],rf[50001];
db g[50001],rg[50001];
Node tr[1000000];

void update(int &i,int l,int r,int x,int y,Data k){
	if (i==0) i=++cnt;
	segin::update(segin::rt[i],1,cntv,y,k);
	if (l==r) return;
	int mid=(l+r)>>1;
	if (mid>=x) update(tr[i].ls,l,mid,x,y,k);
	else update(tr[i].rs,mid+1,r,x,y,k);
}

Data query(int i,int l,int r,int ql,int qr,int qlin,int qrin){
	if (i==0) return Data();
	if (l>=ql and r<=qr) return segin::query(segin::rt[i],1,cntv,qlin,qrin);
	int mid=(l+r)>>1;
	Data ans;
	if (mid>=ql) ans=query(tr[i].ls,l,mid,ql,qr,qlin,qrin);
	if (mid+1<=qr) meg(ans,ans,query(tr[i].rs,mid+1,r,ql,qr,qlin,qrin));
	return ans;
}

int main(){
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.tie(nullptr);
	cin>>n;
	for (int i=1;i<=n;i++){
		cin>>h[i]>>v[i];
		a[i]=h[i];
		b[i]=v[i];
	}
	sort(a+1,a+n+1);
	sort(b+1,b+n+1);
	cnth=unique(a+1,a+n+1)-a-1;
	cntv=unique(b+1,b+n+1)-b-1;
	// cout<<cnth<<" "<<cntv<<"\n";
	for (int i=1;i<=n;i++){
		h[i]=lower_bound(a+1,a+cnth+1,h[i])-a;
		v[i]=lower_bound(b+1,b+cntv+1,v[i])-b;
		Data t=query(rt,1,cnth,h[i],cnth,v[i],cntv);
		t.flag=false;
		t.maxn++;
		f[i]=t.maxn;
		g[i]=t.val;
		update(rt,1,cnth,h[i],v[i],t);
	}
	// cout<<*max_element(f+1,f+n+1)<<"\n";
	fill(segin::tr,segin::tr+segin::cnt+1,segin::Node(Data(0,0)));
	segin::cnt=0;
	memset(segin::rt,0,sizeof(segin::rt));
	fill(tr,tr+cnt+1,Node());
	cnt=rt=0;
	Data ans;
	for (int i=n;i>=1;i--){
		Data t=query(rt,1,cnth,1,h[i],1,v[i]);
		t.flag=false;
		t.maxn++;
		rf[i]=t.maxn;
		rg[i]=t.val;
		update(rt,1,cnth,h[i],v[i],t);
		meg(ans,ans,Data(f[i],g[i],false));
	}
	cout<<ans.maxn<<"\n";
	for (int i=1;i<=n;i++){
		// cout<<"0 ";
		// cout<<g[i]<<" "<<rg[i]<<"\n";
		if (f[i]+rf[i]-1!=ans.maxn) cout<<"0.00000 ";
		else cout<<fixed<<setprecision(5)<<g[i]*rg[i]/ans.val<<" ";
	}
	return 0;
}