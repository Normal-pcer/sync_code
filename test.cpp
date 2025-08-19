#include<bits/stdc++.h>
#if true
//#include "inv.h"
#endif
#define rv std::views::reverse
#define in :
using namespace std;
using std::ranges::iota_view;
auto range(int end){return iota_view(1,end+1);}
auto range(int start,int end){if(start>end)return range(0);return iota_view(start,end+1);}
const int N=1024;
const int NN=(1<<21)|1;
int pool[NN*2],*iv=pool+NN,rec[NN],pot[NN];

void init(int p){
	int lim=p>>10;
	int _lim=p-lim;
	for(auto u in range(N-1)){
		
		int cur=0,d=u<<10;
		for(int i=0;i<=lim;){
			if(cur<=lim){
				rec[i]=u;
				cur+=d;
				i++;
				if(cur>=p)cur-=p;
			}else if(cur>=_lim){
				rec[i]=-u;
				cur+=d;
				i++;
				if(cur>=p)cur-=p;
			}else{
				int r=ceil(1.0*(_lim-cur)/d);
				if(!r){
					cout<<_lim<<" "<<cur<<" "<<d<<"\n";
					while(1);
				}
				cur+=r*d;
				i+=r;
			}
		}
	}
	
	for(auto i in range(0,lim)){
		pot[i]=1ll*(i<<10)%p*(rec[i]+p)%p;
	}
	iv[1]=1;
	for(auto i in range(2,NN-1)){
		iv[i]=1ll*iv[p%i]*(p-(p/i))%p;
	}
	for(auto i in range(1,NN-1)){
		iv[-i]=p-iv[i];
	}
}
int inv(int x){
	int a=x>>10;
	int b=x&1023;
	int u=rec[a];
	int t=pot[a]+b*u;
	return 1ll*(u+998244353)*iv[t]%998244353;
}
int main(){
	// int n;
//	cin>>n;
	init(998244353);
	int ans=0;
	for(auto i in range(1e8))ans^=inv(i);
	cout<<ans<<"\n";
}