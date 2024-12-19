#include<bits/stdc++.h>
using namespace std;
const int N=505;
int x,y;
char a[N][N];
int vis[N][N];
int dx[4]={1,-1,0,0};
int dy[4]={0,0,1,-1};
int ans=0;
void dfs(int xxx,int yyy)
{
	if(vis[xxx][yyy]||a[xxx][yyy]=='*'||xxx<0||yyy<0||xxx>x||yyy>y)
	{
		return;
	}
	vis[xxx][yyy]=1;
	for(int i=0;i<4;i++)
	{
		int xx=xxx+dx[i];
		int yy=yyy+dy[i];
		dfs(xx,yy);
	}
}
int main()
{
	cin>>x>>y;
	for(int i=1;i<=x;i++)
	{
		for(int j=1;j<=y;j++)
		{
			cin>>a[i][j];
		}
	}
	dfs(0,0);
	for(int i=1;i<=x;i++)
	{
		for(int j=1;j<=y;j++)
		{
			if(vis[i][j]==0&&a[i][j]=='0')
			{
				ans++;
			}
		}
	}
	cout<<ans;
	return 0;
}