#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int mod = 1e9 + 7;
const int mxn = 100, mxm = 1 <<8;
const int N = mxn + 10, M = mxm + 10;
int n, m, K, num[M], dp[N][M][M][30], ans;
int get_val(int x) {
	int sum = 0;
	while(x) sum += x & 1, x >>= 1;
	return sum;
} // 求出马的个数
signed main() {
	ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
	cin >> n >> m >> K;
	for(int i = 0; i ^ (1 << n); ++ i) num[i] = get_val(i), dp[1][0][i][num[i]] = 1; // 预处理马的个数
	for(int i = 2; i <= m; ++ i) // 列
		for(int j = 0; j ^ (1 << n); ++ j) // 本列状态
			for(int h = num[j]; h <= K; ++ h) // 马的个数
				for(int k = 0; k ^ (1 << n); ++ k) // 前前列状态
					for(int l = 0; l ^ (1 << n); ++ l) // 前列状态
						if(! ((j & (k << 1)) || (j & (k >> 1)) || (j & (l << 2)) || (j & (l >> 2)) || (l & (k << 2)) || (l & (k >> 2)))) // 判断条件是否满足
							dp[i][l][j][h] = (dp[i][l][j][h] + dp[i - 1][k][l][h - num[j]]) % mod; //进行转移
	for(int i = 0; i ^ (1 << n); ++ i)
		for(int j = 0; j ^ (1 << n); ++ j)
			ans = (ans + dp[m][i][j][K]) % mod;
	cout << ans << '\n';
	return 0;
}