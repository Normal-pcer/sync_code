#include<bits/stdc++.h>
using namespace std;
const int N=2e5+5;
int c[3*N],v[3*N],l[N],cnt;
map<int,int> bin;
int dp[3*N];
int main(){
	memset(dp,0x3f,sizeof(dp));
	int n,m,nw=0;
	scanf("%d%d",&n,&m);
	for(int i=1;i<=n;i++){
		int x,y;
		scanf("%d%d",&x,&y);
		nw+=x;
		int tmp=y-x;
		if(tmp==0)continue;
		if(bin[tmp])l[bin[tmp]]++;
		else{
			c[++cnt]=tmp;
			l[cnt]=1;
			v[cnt]=1;
			bin[tmp]=cnt;
		}
	}
	int tot=cnt;
//	cout<<"L"<<l[1]<<endl;
	for(int i=1;i<=cnt;i++){
		if(l[i]!=1){
			l[i]--;
			for(int j=1;;j++){
				int k=1<<j;
				if(l[i]>k){
					c[++tot]=k*c[i];
					v[tot]=k;
					l[i]-=k;
				}
				else{
					c[++tot]=l[i]*c[i];
					v[tot]=l[i];
					break;
				}
			}
		}
	}
	
	dp[m]=0;
//	for(int i=1;i<=tot;i++){
//		cout<<"----------"<<c[i]<<endl;
//	}
	for(int i=1;i<=tot;i++){
		for(int j=2*m-nw;j>=m-nw;j--){
			dp[j]=min(dp[j],dp[j-c[i]]+v[i]);
		}
	}
	for(int i=m-nw;i<=2*m-nw;i++){
		if(dp[i]==0x3f3f3f3f)cout<<-1<<endl;
		else cout<<dp[i]<<endl;
	}
}