#include<bits/stdc++.h>
#define int long long
using namespace std;
const int zero = 400000;
map<int , int> mp;
int n , m , sum , v[200010] , w[200010] , tot , dp[1000010];
signed main()
{
    scanf("%lld%lld" , &n , &m);
    for(int i = 1 , a , b ; i <= n ; i ++) scanf("%lld%lld", &a , &b) , mp[b - a] ++ , sum += a;
    memset(dp , 0x3f , sizeof dp) , dp[sum + zero] = 0;
    for(auto &[val , num] : mp)
    {
        int k = 1;
        while(k <= num) v[++ tot] = k * val , w[tot] = k , num -= k , k <<= 1;
        if(num > 0) v[++ tot] = num * val , w[tot] = num;
    }
    for(int i = 1 ; i <= tot ; i ++) {
		auto m_half = m / 2 + 1;
		auto j_begin = ((v[i] > 0) ? m : -m_half);
		auto j_end = ((v[i] > 0) ? -m_half : m);
		auto j_step = ((v[i] > 0) ? -1 : 1);
		for(int j = j_begin; j != j_end; j += j_step) {
			auto another = dp[j - v[i] + zero] + w[i];
			dp[j + zero] = min(dp[j + zero] , another);
		}
		// 
	}
    for(int i = 0 ; i <= m ; i ++) printf("%lld\n" , dp[i + zero] == 0x3f3f3f3f3f3f3f3f ? -1 : dp[i + zero]);
    return 0;
}