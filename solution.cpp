#include<bits/stdc++.h>
using namespace std;
using ll=long long;
inline ll read(){
	ll x=0;
	bool f=0;
	char c=getchar();
	while(!isdigit(c)){
		if(c=='-') f=1;
		c=getchar();
	}
	while(isdigit(c)){
		x=x*10+c-'0';
		c=getchar();
	}
	return f?-x:x;
}
const int maxn=50+5;
int n;
char s[maxn];
int f[maxn][maxn];
int main(){
#ifdef LOCAL
	freopen("in.txt","r",stdin);
	freopen("out.txt","w",stdout);
#endif
	scanf("%s",s+1);
	n=strlen(s+1);
	for(int i=n;i>0;i--) for(int j=i;j<=n;j++){
		if(i==j) f[i][j]=1;
		else if(s[i]==s[j]) f[i][j]=f[i][j-1];
		else{
			f[i][j]=n;
			for(int k=i;k<j;k++)
				f[i][j]=min(f[i][j],f[i][k]+f[k+1][j]);
		}
	}
	printf("%d\n",f[1][n]);
#ifdef LOCAL
	fprintf(stderr,"%f\n",1.0*clock()/CLOCKS_PER_SEC);
#endif
	return 0;
}