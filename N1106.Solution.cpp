#include <cmath>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
using namespace std;
long long g[10001][20][20],f[10001][20][20];
long long n,m,e,v[30];
long long bb[30];
bool b[30][30];
int garbage;
int main()
{        
	freopen("N1106.in", "r", stdin);
	freopen("N1106.out", "w", stdout);
	cin>>e;
	long long x,y;
	bb[1]=1;
	int i,j,k,l;
	for(i=2;i<=29;i++)
	  	bb[i]=bb[i-1]*2;
	while(e--)
	{
		scanf("%lld%lld",&n,&m);
		for(i=0;i<=10000;i++)
            for(j=0;j<=19;j++)
            for(k=0;k<=19;k++)
                {
		    		f[i][j][k]=0;
		    		g[i][j][k]=0;
				}
		for(i=0;i<=n;i++)
		    for(j=0;j<=n;j++)
		    {
		        b[i][j]=false;
			}
		for(i=1;i<=n;i++)
		{
			scanf("%lld",&v[i]);
		}
		for(i=1;i<=m;i++)
		{
			scanf("%lld%lld",&x,&y);
			b[x][y]=b[y][x]=true;
			f[bb[x]+bb[y]][x][y]=f[bb[x]+bb[y]][y][x]=v[x]*v[y]+v[x]+v[y];//初始化f数组
			g[bb[x]+bb[y]][x][y]=g[bb[x]+bb[y]][y][x]=1;//初始化g数组
		}
		for(i=0;i<=bb[n+1]-1;i++)
		{
			for(j=1;j<=n;j++)
				if((bb[j] | i)==i)        
					for(k=1;k<=n;k++)
			  			if((bb[k] | i)==i && b[j][k]){if(!g[i][j][k]) continue;
			  				for(l=1;l<=n;l++)
			  			    	if((bb[l] | i)!=i && b[k][l] && b[j][l])//当j和l有路径相同
			  			    	{
			  			        	if(f[i+bb[l]][k][l]==f[i][j][k]+v[k]*v[l]+v[j]*v[k]*v[l]+v[l])
			  			        		g[i+bb[l]][k][l]+=g[i][j][k];
									else
									//当j和l无路径相同
									if(f[i+bb[l]][k][l]<f[i][j][k]+v[k]*v[l]+v[j]*v[k]*v[l]+v[l])
									{
										f[i+bb[l]][k][l]=f[i][j][k]+v[k]*v[l]+v[j]*v[k]*v[l]+v[l];
			  			        		g[i+bb[l]][k][l]=g[i][j][k];
									}
								}
								else
			  			    	if((bb[l] | i)!=i && b[k][l])
			  			    	{
			  			        	if(f[i+bb[l]][k][l]==f[i][j][k]+v[k]*v[l]+v[l])
			  			        		g[i+bb[l]][k][l]+=g[i][j][k];
									else
									if(f[i+bb[l]][k][l]<f[i][j][k]+v[k]*v[l]+v[l])
									{
			  			            	f[i+bb[l]][k][l]=f[i][j][k]+v[k]*v[l]+v[l];
			  			        		g[i+bb[l]][k][l]=g[i][j][k];
									}
								}}
		}
		long long ans=0;
		long long ma=0;
		for(i=1;i<=n;i++)
		    for(j=1;j<=n;j++)
		    {
		    	if(f[bb[n+1]-1][i][j]>ma)
		    	{
		    	    ma=f[bb[n+1]-1][i][j];
                    ans=g[bb[n+1]-1][i][j];
				}
				else
				if(f[bb[n+1]-1][i][j]==ma)
				{
					ans+=g[bb[n+1]-1][i][j];
				}
			}
		if(n==1) cout<<v[1]<<' '<<1<<endl;else
		if(ma==0 || ans==0) cout<<0<<' '<<0<<endl;else
		printf("%lld %lld\n",ma,ans/2);

		// unsigned useless = 10;
		// for (unsigned i=0; i<2e7; i++)  useless ^= (((useless&(i+((i-1)<<2)))|useless*useless)-1)*(useless%3);
		// garbage = useless;
	}
	if (e>=25)  printf("%d\n", garbage);
	return 0;
}


