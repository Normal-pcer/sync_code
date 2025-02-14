// LUOGU_RID: 202945832
#include<bits/stdc++.h>
#define int long long
using namespace std;
const int N=2e5+5;
int p[N];
int d[N];
signed main(){
	int n;
	cin>>n;
	for(int i=0;i<n;i++){
		cin>>p[i];
		int tmp=(p[i]-i+n)%n;
		int x=(tmp+n/2)%n;
		if(x<tmp){
			if(x>0){
				if(n%2==0){
					d[0]+=n-tmp;
					d[1]-=n-tmp;
					d[1]+=1;
					d[x+1]-=1;
					d[x+1]-=1;
					d[tmp+1]+=1;
					d[tmp+1]+=1;
				}else{
					d[0]+=n-tmp;
					d[1]-=n-tmp;
					d[1]+=1;
					d[x+1]-=1;
					d[x+2]-=1;
					d[tmp+1]+=1;
					d[tmp+1]+=1;
				}
			}
			if(x==0){
				if(n%2==0){
					d[0]+=n-tmp;
					d[1]-=n-tmp;
					d[1]-=1;
					d[tmp+1]+=1;
					d[tmp+1]+=1;
				}else{
					d[0]+=n-tmp;
					d[1]-=n-tmp;
					d[2]-=1;
					d[x+1]+=1;
					d[x+1]+=1;
				}				
			}
		}
		if(x>tmp){
			if(tmp>0){
				if(n%2==0){
					d[0]+=tmp;
					d[1]-=tmp;
					d[1]-=1;
					d[tmp+1]+=1;
					d[tmp+1]+=1;
					d[x+1]-=1;
					d[x+1]+=1;
				}else{
					d[0]+=tmp;
					d[1]-=tmp;
					d[1]-=1;
					d[tmp+1]+=1;
					d[tmp+2]+=1;
					d[x+1]-=1;
					d[x+1]-=1;
				}
			}
			if(tmp==0){
				if(n%2==0){
					d[0]+=tmp;
					d[1]+=1;
					d[x+1]-=1;
					d[x+1]-=1;
				}else{
					d[0]+=tmp;
					d[1]+=1;
					d[x+1]-=1;
					d[x+2]-=1;
//					d[x+1]+=1;
				}				
			}
		}	
	}
	for(int i=1;i<n;i++){
		d[i]+=d[i-1];
	}
	for(int i=1;i<n;i++){
		d[i]+=d[i-1];
	}
	int ans=1e18;
	for(int i=0;i<n;i++){
//		cout<<i<<" "<<d[i]<<endl;
//		d[i]+=d[i-1];
		ans=min(ans,d[i]);
	}
	cout<<ans;
}