#include<bits/stdc++.h>
using namespace std;
const int mod=1e9+7;
const int inv2=5e8+4;
inline int get_sum(int a,int b)
{
	return a+b-(a+b>=mod?mod:0);
}
inline int get_power(int a,int n)
{
	int res=1;
	while(n>0)
	{
		res=n&1?1ll*res*a%mod:res;
		a=1ll*a*a%mod;
		n>>=1;
	}
	return res;
}
int n,m;
inline int id(int x,int y)
{
	return (x-1)*(m<<1)+(y-1);
}
const int max_n=1e3+5;
const int max_m=1e3+5;
char str[max_m];
int Map[max_n<<1][max_m<<1];
const int max_tot=4e6+5;
int pow2[max_tot],pow_inv2[max_tot],Hash[max_n<<1][max_m<<1];
inline int get_Hash(int a,int b,int c,int d) // calculate the hash value of the matrix that the upper left corner is (a,b) and the lower right corner is (c,d)
{
	return (1ll*Hash[c][d]-Hash[a-1][d]-Hash[c][b-1]+Hash[a-1][b-1]+2*mod)*pow_inv2[id(a,b)]%mod;
}
int main()
{
	scanf("%d%d",&n,&m);
	for(int i=1;i<=n;++i)
	{
		scanf("%s",str+1);
		for(int j=1;j<=m;++j)
			Map[i][j]=Map[i+n][j]=Map[i][j+m]=Map[i+n][j+m]=(str[j]=='.');
	}
	pow2[0]=1;
	for(int i=1;i<=(n*m<<2);++i)
		pow2[i]=get_sum(pow2[i-1],pow2[i-1]);
	pow_inv2[n*m<<2]=get_power(pow2[n*m<<2],mod-2);
	for(int i=(n*m<<2)-1;i>=0;--i)
		pow_inv2[i]=get_sum(pow_inv2[i+1],pow_inv2[i+1]); 
	for(int i=1;i<=(n<<1);++i)	
		for(int j=1;j<=(m<<1);++j)
			Hash[i][j]=(1ll*Map[i][j]*pow2[id(i,j)]+Hash[i-1][j]+Hash[i][j-1]-Hash[i-1][j-1]+mod)%mod;
	int ans_x=1,ans_y=1;
	for(int i=1;i<=n;++i)
		for(int j=1;j<=m;++j)
		{
			int L=1,R=n,res_x=n+1,res_y=m+1;
			while(L<=R)
			{
				int mid=(L+R)>>1;
				if(get_Hash(i,j,i+mid-1,j+m-1)!=get_Hash(ans_x,ans_y,ans_x+mid-1,ans_y+m-1))
					res_x=mid,R=mid-1;
				else
					L=mid+1;
			}
			if(res_x==n+1)
				continue;
			L=1,R=m;
			while(L<=R)
			{
				int mid=(L+R)>>1;
				if(get_Hash(i+res_x-1,j,i+res_x-1,j+mid-1)!=get_Hash(ans_x+res_x-1,ans_y,ans_x+res_x-1,ans_y+mid-1))
					res_y=mid,R=mid-1;
				else
					L=mid+1;
			}
			if(!Map[i+res_x-1][j+res_y-1])
				ans_x=i,ans_y=j;
		}
	for(int i=1;i<=n;++i)
	{
		for(int j=1;j<=m;++j)
			putchar(Map[ans_x+i-1][ans_y+j-1]?'.':'*');
		putchar('\n');
	}
	return 0;
}
