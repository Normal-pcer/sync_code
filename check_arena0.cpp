// LUOGU_RID: 204085138
#include<bits/stdc++.h>
using namespace std;
int T , dp[2][1000010];
void solve()
{
	string s , t; int k; scanf("%d" , &k); cin >> s >> t; s = '_' + s , t = '_' + t;
//	for(int i = 0 ; i <= (int)max(s.size() , t.size()) ; i ++) dp[0][i] = dp[1][i] = 0;
	if(s.size() > t.size()) swap(s , t);
	// s çŸ­ 
	if(int(t.size() - s.size()) > k) return puts("No") , void();
	memset(dp , 0x3f , sizeof(dp));
	dp[0][0] = 0;
	for(int i = 1 ; i < (int)s.size() ; i ++)
	{
		for(int j = max(0 , i - k) ; j <= min((int)t.size() - 1 , i + k) ; j ++)
		{
			dp[i & 1][j] = min({dp[(i - 1) & 1][j - 1] + (s[i] != t[j]) , dp[(i - 1) & 1][j] + 1 , dp[i & 1][j - 1] + 1});
		}
	}
//	printf("%d\n" , dp[(s.size() - 1) & 1][t.size() - 1]);
//	printf("%d %d\n" , t.size() , s.size());
	if(dp[(s.size() - 1) & 1][t.size() - 1] <= k) puts("Yes");
	else puts("No");
}
int main()
{
//	freopen("data.in" , "r" , stdin);
//	freopen("1.ans" , "w" , stdout);
//	freopen("edit.in" , "r" , stdin);
//	freopen("edit.ans" , "w" , stdout);
//	scanf("%d" , &T);
	int T = 1;
	while(T --) solve();
	return 0;
}