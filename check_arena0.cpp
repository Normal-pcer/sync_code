#include<bits/stdc++.h>
#define int long long
using namespace std;
string a,b;
const int mo=1E9;
int k;
const int N=1e6+5;
int dp[N][45];
signed main(){
	int T=1;
//	cin>>T;
	while(T--){
		
		cin>>k;
		cin>>a>>b;
		int n=a.size();
		int m=b.size();
		a="-"+a;
		b="f"+b;
		if(n>m){
			swap(n,m);
			swap(a,b);
		}
		if(m-n>k){
			cout<<"No";
			continue;
		}
		for(int i=0;i<=n;i++){
			for(int j=0;j<=44;j++){
//				printf("dp[%d][%d]=%d\n",i,j,dp[i][j]);
				dp[i][j]=0x3f3f3f3f;
			}
		}
		dp[0][k]=0;
		for(int i=1;i<=n;i++){
			for(int j=max(1ll,i-k);j<=min(m,i+k);j++){
				int t=j-i+k;
				dp[i][t]=min({dp[i-1][t]+1,dp[i-1][t-1]+(a[i]!=b[j]),dp[i][j-1]});
			}
		}
		if(dp[n][m-n+k]<=k) cout<<"Yes"<<endl;
		else cout<<"No"<<endl;
//		for(int i=0;i<=n;i++){
//			for(int j=0;j<=k;j++){
//				printf("dp[%d][%d]=%d\n",i,j,dp[i][j]);
//				dp[i][j]=-1;
//			}
//		}		
	}
}
/*
3
bbcbcddcc
bcbccddccc
*/