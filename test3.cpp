#define GNU_DEBUG
#define _GLIBCXX_DEBUG 1
#define _GLIBCXX_DEBUG_PEDANTIC 1
#define _GLIBCXX_SANITIZE_VECTOR 1

#include<bits/stdc++.h>
#define int long long
using namespace std;
const int N=2e5+5,M=1E9+1;
int la[N],lb[N],ra[N],rb[N];
int C,T;
struct node{
	int num;
	int cnt;
}tmp[N];
map<int,int> bin;
int n;
bool cmp(node x,node y){
	return x.num<y.num;
}
int check(int mid){
	int l=0,r=0,b=0,L=0,R=0;
	for(int i=1;i<=n;i++){
		if(rb[i]<mid){
			l+=la[i];
			r+=ra[i];
		}else if(lb[i]>mid){
			L+=la[i];
			R+=ra[i];
		}else{
			b+=ra[i];
		}
	}
	int lc=max(L,l-b+1),rc=min(R,r+b);
	return lc<=rc;
}
struct kdl{
	int l,r;
	mutable int v;
	kdl(int L,int R=-1,int V=0):l(L),r(R),v(V){}
	bool operator <(const kdl& x) const{
		return l<x.l;
	}
};
set<kdl> s;
auto split(int pos){
	auto it=s.lower_bound(kdl{pos,-1,0});
	if(it!=s.end()&&it->l==pos)return it;
	it--;
	int l=it->l,r=it->r;
	int v=it->v;
	s.erase(it);
	s.insert(kdl(l,pos-1,v));
	return s.insert(kdl(pos,r,v)).first;
}
void tp(int l,int r,int val){
	std::cout << "tp: " << l << " " << r << " " << val << std::endl;
    auto ir=split(r+1);
    
	auto il=split(l);
//	for(auto i=il;i!=ir;i++){
//		cout<<i->l<<" "<<i->r<<" "<<i->v<<"\n";
//	}
	
	s.erase(il,ir);
//	cout<<"he"<<endl;
	s.insert(kdl(l,r,val));
}
int wk(int l,int r){
	auto ir=split(r+1), il=split(l);
	int res=0;
	for(;il!=ir;il++){
		res+=il->v*(il->r - il->l+1);
	}
	return res;
}
void solve(){
	memset(tmp,0,sizeof(tmp));
	s.clear();
	s.insert(kdl(0, M, 0));
	bin.clear();
	cin>>n;
	
	int tot=0;
	int sum=0;
	for(int i=1;i<=n;i++){
		cin>>la[i]>>ra[i]>>lb[i]>>rb[i];
//		cout<<"             "<<lb[i]<<" "<<rb[i]<<endl;;
		tp(lb[i],rb[i],1);
		
		sum+=la[i];
		if(!bin[lb[i]]){	
			tmp[++tot].num=lb[i];
			bin[lb[i]]=tot;
			tmp[tot].cnt=la[i];
		}else{
			int k=bin[lb[i]];
			tmp[k].cnt+=la[i];
		}
	}
	sort(tmp+1,tmp+tot+1,cmp);
//	if(T==2)cout<<tot<<"***"<<endl;
	sum=(sum+1)/2;
	int res=0,lg=0;
	for(int i=1;i<=tot;i++){
		res+=tmp[i].cnt;
		if(res>=sum){
			lg=tmp[i].num;
			break;
		}
	}
	int l=1,r=lg,ans=1;
	while(l<=r){
		int mid=(l+r)>>1;
		if(check(mid)){
			ans=mid;
			r=mid-1;
		}else{
			l=mid+1;
		}
	}
	int L=ans;
	l=lg,r=1e9,ans=lg;
	while(l<=r){
		int mid=(l+r)>>1;
		if(check(mid)){
			ans=mid;
			l=mid+1;
		}else{
			r=mid-1;
		}
	}
	int R=ans;
	cout<<wk(L,R)<<"\n";
}
signed main(){
//	ios::sync_with_stdio(false);
//	cin.tie(0);
	
	// cin>>C>>T;
	// while(T--){
		solve();
	// }
}