#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

mt19937_64 rnd(random_device{}());

int n,m;
int a[100001];

struct Node{
	int x,siz,ls,rs,tag,rev,sum[21];
	Node(){
		x=siz=ls=rs=tag=rev=0;
		memset(sum,0,sizeof(sum));
	}
};
int rt,cnt;
Node tr[100001];
int newNode(int x){
	++cnt;
	tr[cnt].x=x;
	tr[cnt].siz=1;
	for (int i=0;i<21;i++) tr[cnt].sum[i]=((x>>i)&1);
	return cnt;
}
void pushup(int i){
	tr[i].siz=tr[tr[i].ls].siz+tr[tr[i].rs].siz+1;
	for (int j=0;j<21;j++) tr[i].sum[j]=tr[tr[i].ls].sum[j]+tr[tr[i].rs].sum[j]+((tr[i].x>>j)&1);
}
void pushdown(int i){
	int &ls=tr[i].ls,&rs=tr[i].rs;
	if (tr[i].rev){
		swap(ls,rs);
		if (ls) tr[ls].rev^=1;
		if (rs) tr[rs].rev^=1;
		tr[i].rev=0;
	}
	if (tr[i].tag){
		if (ls){
			tr[ls].x^=tr[i].tag;
			tr[ls].tag^=tr[i].tag;
			for (int j=0;j<21;j++) tr[ls].sum[j]=tr[ls].siz-tr[ls].sum[j];
		}
		if (rs){
			tr[rs].x^=tr[i].tag;
			tr[rs].tag^=tr[i].tag;
			for (int j=0;j<21;j++) tr[rs].sum[j]=tr[rs].siz-tr[rs].sum[j];
		}
		tr[i].tag=0;
	}
}
void split(int i,int &x,int &y,int siz){
	if (i==0){
		x=y=0;
		return;
	}
	pushdown(i);
	if (tr[tr[i].ls].siz<siz) x=i,split(tr[i].rs,tr[i].rs,y,siz-tr[tr[i].ls].siz-1);
	else y=i,split(tr[i].ls,x,tr[i].ls,siz);
	pushup(i);
}
int meg(int x,int y){
	if (x==0 or y==0) return x|y;
	if (rnd()%(tr[x].siz+tr[y].siz)<tr[x].siz){
		pushdown(x);
		tr[x].rs=meg(tr[x].rs,y);
		pushup(x);
		return x;
	}else{
		pushdown(y);
		tr[y].ls=meg(x,tr[y].ls);
		pushup(y);
		return y;
	}
}
void range(int l,int r,int &x,int &y,int &z){
	split(rt,x,y,l-1);
	split(y,y,z,r-l+1);
}

int main(){
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.tie(nullptr);
	cin>>n>>m;
	for (int i=1;i<=n;i++){
		cin>>a[i];
		rt=meg(rt,newNode(a[i]));
	}
	while (m--){
		int op,l,r,k,x,y,z;
		cin>>op>>l>>r;
		range(l,r,x,y,z);
		if (op==1){
			tr[y].rev^=1;
		}else if (op==2){
			cin>>k;
			tr[y].tag^=k;
			tr[y].x^=k;
			for (int i=0;i<21;i++){
				if ((k>>i)&1){
					tr[y].sum[i]=tr[y].siz-tr[y].sum[i];
				}
			}
		}else{
			ll ans=0;
			for (int i=0;i<21;i++){
				ans+=tr[y].sum[i]*(1ll<<i);
			}
			cout<<ans<<"\n";
		}
		rt=meg(x,meg(y,z));
		// for (int i=1;i<=n;i++){
		// 	range(i,i,x,y,z);
		// 	ll ans=0;
		// 	for (int i=0;i<20;i++){
		// 		ans+=tr[y].sum[i]*(1ll<<i);
		// 	}
		// 	cout<<ans<<" ";
		// 	rt=meg(x,meg(y,z));
		// }
		// cout<<"\n";
	}
	return 0;
}