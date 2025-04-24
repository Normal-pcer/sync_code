#include<bits/stdc++.h>
using namespace std;
using ull=unsigned long long;
const int N=1e3+5;
const int sq=340;
const int siz=1570;
struct bit{
	ull a[siz];
	bit() {
		reset();
	}
	void reset(){
		memset(a,0,sizeof(a));
	}
	bool get(int x) const {
		return a[x>>6]>>(x&63)&1;
	}
	void set1(int x){
		a[x>>6]|=1ull<<(x&63);
	}
	void set0(int x){
		a[x>>6]&=~(1ull<<(x&63));
	}
	void operator |=(const bit &b){
		for(int i=0;i<siz;i++)a[i]|=b.a[i];
	}
	void operator ^=(const bit &b){
		for(int i=0;i<siz;i++)a[i]^=b.a[i];
	}
	void operator &=(const bit &b){
		for(int i=0;i<siz;i++)a[i]&=b.a[i];
	}
	auto operator& (const bit &other) const -> bit {
		auto tmp = *this;
		return tmp &= other, tmp;
	}
};
vector<int> mp[N];
bit rch[N];
bit sa[sq+2],sb[sq+2];

int a[N],b[N];
int ra[N],rb[N];
int n,m,q;
int st[N],ed[N],bl[N];
void build(){
	for(int i=1;i<=sq;i++){
		st[i]=n/sq*(i-1)+1;
		ed[i]=n/sq*i;
	}
	ed[sq]=n;
	for(int i=1;i<=sq;i++){
		for(int j=st[i];j<=ed[i];j++){
			bl[j]=i;
		}
	}
}
void init(){
	for(int i=sq;i>=1;i--){
		sa[i]|=sa[i+1];
		sb[i]|=sb[i+1];
		for(int j=st[i];j<=ed[i];j++){
			sa[i].set1(ra[j]);
			sb[i].set1(rb[j]);
		}
	}
}
void swapA(int l,int r){
	if(a[l]>a[r])swap(l,r);
	for(int i=bl[a[l]]+1;i<=bl[a[r]];i++){
		sa[i].set1(l);
		sa[i].set0(r);
	}
	swap(ra[a[l]],ra[a[r]]);
	swap(a[l],a[r]);
}
void swapB(int l,int r){
	if(b[l]>b[r])swap(l,r);
	for(int i=bl[b[l]]+1;i<=bl[b[r]];i++){
		sb[i].set1(l);
		sb[i].set0(r);
	}
	swap(rb[b[l]],rb[b[r]]);
	swap(b[l],b[r]);
}
bit gets(int x,int l,int r){
	bit res;
	if(bl[l]==bl[r]){
		for(int i=l;i<=r;i++){
			res.set1(ra[i]);
		}
	}else{
		res|=sa[bl[l]+1];
		res^=sa[bl[r]];
		for(int i=l;i<=ed[bl[l]];i++){
			res.set1(ra[i]);
		}
		for(int i=st[bl[r]];i<=r;i++){
			res.set1(ra[i]);
		}
	}
	res&=rch[x];
	return res;
}
int query(int x,int l,int r){
	bit des=gets(x,l,r);
	int i=1,j=0;
//	for(;i<=sq&&j<siz;){
//		if(sb[i].a[j]&des.a[j])i++;
//		else j++;
//	}
	for (; i <= n and j != siz; ++i) {
		auto unmatch = [&](int j) -> bool {
			return (sb[i].a[j] & des.a[j]) == 0;
		};
		while (j != siz and unmatch(j)) ++j;
		if (j == siz) break;
		
		cerr << "i = " << i << ", j = " << j << endl;
		{
			cerr << "match " << sb[i].a[j] << "with" << des.a[j] << endl;
			cerr << "x & y: " << (sb[i].a[j] & des.a[j]) << endl;
			auto tmp = des & sb[i];
			for (int i = 1; i <= n; i++) {
//				cerr << tmp.get(i);
				if (tmp.get(i)) cerr << i << "(" << bl[b[i]] << ")" << ", ";
			}
			cerr << endl;
		}
	}
	i--;
	if (i == 0) return 0;

	for(j=ed[i];j>=st[i];j--){
		if(des.get(rb[j]))return j;
	}
	
	
	cerr << "i = " << i << endl;
	{
		auto tmp = des & sb[i];
		for (int i = 1; i < n + 1; i++) {
//				cerr << tmp.get(i);
			if (tmp.get(i)) cerr << i << "(" << bl[b[i]] << ")" << ", ";
		}
		cerr << endl;
	}
	cerr << "ed[i] = " << ed[i] << ", st[i] = " << st[i] << endl;
	assert(false);
	return 0;
}
void solve(){
	cin>>n>>m>>q;
	for(int i=1;i<=n;i++)mp[i].clear();
	for(int i=1;i<=m;i++){
		int u,v;
		cin>>u>>v;
		mp[u].push_back(v);
	}
	for(int i=1;i<=n;i++){
		rch[i].reset();
		rch[i].set1(i);
	}
	for(int i=n;i>=1;i--){
		for(auto j:mp[i]){
			rch[i]|=rch[j];
		}
	}
	for(int i=1;i<=n;i++){
		cin>>a[i];
		ra[a[i]]=i;
	}
	for(int i=1;i<=n;i++){
		cin>>b[i];
		rb[b[i]]=i;
	}
	build();
	init();
	
	// for (int j = sq; j <= sq; j++) {
	// 	{
	// 		auto tmp = sb[j];
	// 		cerr << "j = " << j << endl;
	// 		for (int i = 1; i < n + 1; i++) {
	// //				cerr << tmp.get(i);
	// 			if (tmp.get(i)) cerr << i << "(b[i] = " << b[i] << ", bl[b[i]] = " << bl[b[i]] << ")" << ", ";
	// 		}
	// 		cerr << endl;
	// 	}
	// }
	// return;
	
	for(int i=1;i<=q;i++){
		int op,x,y,z;
		cin>>op>>x>>y;
		if(op==1){
			swapA(x,y);
		}
		if(op==2){
			swapB(x,y);
		}
		if(op==3){
			cin>>z;
			cout<<query(x,y,z)<<"\n";
		}
	}
	
}
int main(){
	freopen("t1.in", "r", stdin);
	ios::sync_with_stdio(false);
	cin.tie(0);
	int c,T;
	cin>>c>>T;
	while(T--){
		solve();
	}
}