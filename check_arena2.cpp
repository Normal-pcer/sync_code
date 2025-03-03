#include<bits/stdc++.h>
using namespace std;
#define int long long
#define rep(i,l,r) for(int i=(l);i<=(r);++i)
#define pre(i,l,r) for(int i=(l);i>=(r);--i)
const int N=2e5+5;
int L[N],R[N],bel[N],sum[N],add[N],len[N];
int A[N],B[N],C[N],n;
struct node{
	int a,b,c,id;
}p[N];
int calc(int l,int r){
	return ((l+r)*(r-l+1))/2;
}

void solve(){
	cin>>n;
	rep(i,1,n){
		cin>>p[i].a>>p[i].b>>p[i].c;p[i].id=i;
		A[i]=p[i].a;B[i]=p[i].b;C[i]=p[i].c;
	}
	sort(p+1,p+n+1,[&](const node&x,const node&y){
		return x.c<y.c;
	});
	int now=0;
	bool f=1;
	rep(i,1,n){
		int j=p[i].id;
		if(A[j]==B[j]){
			if(now>C[j]){
				f=0;
				break;
			}
			continue;
		} 
		if(A[j]>B[j]){
			int k=0;
			pre(q,j,1){
				if(A[q]<=p[i].b-(j-q)){
					break;
				}
				k=q;
			}
			rep(q,k,j) now+=A[q];
			now-=calc(p[i].b-(j-k),p[i].b);
			rep(q,k,j) A[q]=p[i].b-(j-q);
			if(now>p[i].c){
				f=0;break;
			}
		}
		if(A[j]<B[j]){
			int k=0;
			rep(q,j,n){
				if(A[q]>=p[i].b+(q-j)) break;
				k=q;
			}
			rep(q,j,k) now-=A[q];
			now+=calc(p[i].b,p[i].b+(k-j));
			rep(q,j,k) A[q]=p[i].b+(q-j);
			if(now>p[i].c){
				f=0;
				break; 
			}
		}
	}
	if(f){
		cout<<"Yes\n";
	}else{
		cout<<"No\n";
	}
}
int32_t main(){
	// freopen("move.in","r",stdin);
	// freopen("move.out","w",stdout);
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.tie(nullptr);
	int c,t;
	cin>>c>>t;
	while(t--) ::solve();
	return 0;
}
//tomxi