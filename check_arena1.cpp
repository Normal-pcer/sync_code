/*
#include<bits/stdc++.h>
using namespace std;
int h , w , sx , sy , dx[5] = {0 , 1 , -1 , 0} , dy[5] = {1 , 0 , 0 , -1};
vector<int> mp[1000010];
vector<bool> flag[1000010];
bool check(int x , int y)
{
	return x >= 1 && x <= h && y >= 1 && y <= w && ((flag[x][y] == false && mp[x][y] != '#') || x == sx && y == sy);
}
void dfs(int x , int y , int num)
{
	if(mp[x][y] == 'S' && num >= 4) puts("Yes") , exit(0);
	for(int i = 0 ; i < 4 ; i ++)
	{
		int nx = x + dx[i] , ny = y + dy[i];
		if(check(nx , ny)) flag[nx][ny] = true , dfs(nx , ny , num + 1) , flag[nx][ny] = false;
	}
}
int main()
{
	scanf("%d%d" , &h , &w);
	for(int i = 1 ; i <= h ; i ++)
	{
		for(int j = 1 ; j <= w ; j ++)
		{
			char c; cin >> c;
			if(c == 'S') sx = i , sy = j;
			mp[i].push_back(c);
			flag[i].push_back(false);
		}
	}
	dfs(sx , sy , 0);
	puts("No");
	return 0;
}
*/
/*
#include<bits/stdc++.h>
using namespace std;
int h , w , sx , sy , ans , dx[5] = {0 , 1 , -1 , 0} , dy[5] = {1 , 0 , 0 , -1};
vector<int> mp[1000010];
vector<bool> flag[1000010];
bool check(int x , int y)
{
	return x >= 1 && x <= h && y >= 1 && y <= w && ((flag[x][y] == false && mp[x][y] != '#'));
}
void dfs(int x , int y)
{
	for(int i = 0 ; i < 4 ; i ++)
	{
		int nx = x + dx[i] , ny = y + dy[i];
		if(check(nx , ny)) flag[nx][ny] = true , dfs(nx , ny);
	}
}
int main()
{
	scanf("%d%d" , &h , &w);
	for(int i = 1 ; i <= h ; i ++)
	{
		mp[i].push_back('_');
		flag[i].push_back(false);
		for(int j = 1 ; j <= w ; j ++)
		{
			char c; cin >> c;
			if(c == 'S') sx = i , sy = j;
			mp[i].push_back(c);
			flag[i].push_back(false);
		}
	}
	dfs(sx , sy);
	for(int i = 1 ; i <= h ; i ++) for(int j = 1 ; j <= w ; j ++) ans += flag[i][j];
	puts(ans >= 4 ? "Yes" : "No");
	return 0;
}
*/
#include<bits/stdc++.h>
using namespace std;
int h , w , sx , sy , dx[5] = {0 , 1 , -1 , 0} , dy[5] = {1 , 0 , 0 , -1};
vector<int> mp[1000010];
vector<bool> flag[1000010];
bool check(int x , int y)
{
	return x >= 1 && x <= h && y >= 1 && y <= w && ((flag[x][y] == false && mp[x][y] != '#') || x == sx && y == sy);
}
void dfs(int x , int y , int num , int lx , int ly)
{
	if(mp[x][y] == 'S' && num >= 4 && (x != lx || y != ly)) puts("Yes") , exit(0);
	for(int i = 0 ; i < 4 ; i ++)
	{
		int nx = x + dx[i] , ny = y + dy[i];
		if(check(nx , ny) && (nx != lx || ny != ly)) flag[nx][ny] = true , dfs(nx , ny , num + 1 , x , y);
	}
}
int main()
{
	scanf("%d%d" , &h , &w);
	for(int i = 1 ; i <= h ; i ++)
	{
		mp[i].push_back('_');
		flag[i].push_back(false);
		for(int j = 1 ; j <= w ; j ++)
		{
			char c; cin >> c;
			if(c == 'S') sx = i , sy = j;
			mp[i].push_back(c);
			flag[i].push_back(false);
		}
	}
	dfs(sx , sy , 0 , sx , sy);
	puts("No");
	return 0;
}