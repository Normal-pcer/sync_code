#include<bits/stdc++.h>
using namespace std;
const int N=1e6+5;
#define ls ((p)<<1)
#define rs ((p)<<1|1)
#define mid ((x+y)>>1)
int a[N<<2],siz[N<<2];
void mdf(int x,int y,int idx,int v,int p){
	if(x==y){
		siz[p]+=v;
		if(siz[p])a[p]=x+siz[p]-1;
		else a[p]=0;
		return;
	}
	if(idx<=mid)mdf(x,mid,idx,v,ls);
	else mdf(mid+1,y,idx,v,rs);
	siz[p]=siz[ls]+siz[rs];
	if(!siz[ls]||!siz[rs])a[p]=a[ls]+a[rs];
	else a[p]=max(a[ls]+siz[rs],a[rs]);
}
int n,m,k,t;
int dep[N],q[N],L,R;
vector<int> e[N];
bool op[N];
int main(){
	cin>>n>>m>>k>>t;
	int x,y;
	while(m--){
		scanf("%d%d",&x,&y);
		e[y].push_back(x);
	}
	memset(dep,0x3f,sizeof(dep));
	q[L=R=1]=t,dep[t]=0;
	while(L<=R){
		int p=q[L++];
		for(int t:e[p]){
			if(dep[t]>dep[p]+1){
				dep[t]=dep[p]+1;
				q[++R]=t;
			}
		}
	}
	while(k--){
		scanf("%d",&x);
		if(!op[x]){
			op[x]=1;
			mdf(0,n,dep[x],1,1);
		}
		else{
			op[x]=0;
			mdf(0,n,dep[x],-1,1);
        }
        if(siz[1]==0)puts("0");
		else printf("%d\n",a[1]+1);
	}
}