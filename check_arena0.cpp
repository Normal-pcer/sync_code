#include<bits/stdc++.h>
#define int long long
using namespace std;
const int N=1002,Q=1e6+5;
struct stf{
	int a;
	int b;
	int c;
}t[N];
struct qry{
	int idx;
	int m;
	int s;
	int k;
}q[Q];
int dp[N][N*10]; 
int ans[Q];
int cmt(stf x,stf y){
	return x.a<y.a;
}
int cmq(qry x,qry y){
	return x.m<y.m;
}
signed main(){
	ios::sync_with_stdio(false);
	cin.tie(0);
	int n,qt;
	cin>>n;
	for(int i=1;i<=n;i++){
		cin>>t[i].c>>t[i].a>>t[i].b;
	}
	cin>>qt;
	for(int i=1;i<=qt;i++){
		q[i].idx=i;
		cin>>q[i].m>>q[i].k>>q[i].s;
	}
	sort(t+1,t+n+1,cmt);
	sort(q+1,q+qt+1,cmq);
	dp[0][0]=-1;
	for(int i=1;i<=n;i++){
		for(int j=0;j<=10000;j++){
			int c=t[i].c;
//			cout<<c<<endl;
			if(c<=j){
				if(dp[i-1][j-c]==-1)dp[i][j]=max(dp[i-1][j],t[i].b);
				else if(dp[i-1][j-c])dp[i][j]=max(dp[i-1][j],min(dp[i-1][j-c],t[i].b));
			}else{
				dp[i][j]=dp[i-1][j];
			}
		}
	}
//	for(int i=1;i<=n;i++){
//		for(int j=0;j<=7;j++){
//			printf("dp[%d][%d]=%d\n",i,j,dp[i][j]);
//		}
//	}	
	int l=0;
	t[n+1].a=1e9+1;
	for(int i=1;i<=qt;i++){
		while(q[i].m>=t[l].a)l++;
		int r=l-1;
//		cout<<"-------"<<i<<" "<<r<<"\n";
		if(dp[r][q[i].k]==0){
//			cout<<"NIE"<<"\n";
			ans[q[i].idx]=0;
			continue;
		}else if(dp[r][q[i].k]<=q[i].m+q[i].s){
//			cout<<"NIE"<<"\n";
			ans[q[i].idx]=0;
			continue;
		}
//		cout<<"TAK"<<"\n";
		ans[q[i].idx]=1;
	}
	for(int i=1;i<=qt;i++){
		if(ans[i])cout<<"TAK"<<"\n";
		else cout<<"NIE"<<"\n";
	}
}