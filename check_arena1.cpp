#include<bits/stdc++.h>
#define int long long
using namespace std;
int n , q , a[100010] , sum[100010];
signed main()
{
	scanf("%lld%lld" , &n , &q);
	for(int i = 1 ; i <= n ; i ++) scanf("%lld" , &a[i]);
	sort(a + 1 , a + n + 1);
	for(int i = 1 ; i <= n ; i ++) sum[i] = sum[i - 1] + a[i];
	while(q --)
	{
		int k , m , ans = 0; scanf("%lld%lld" , &k , &m);
		int l = 0 , r = min<long long>(m , lower_bound(a + 1 , a + n + 1 , k) - a - 1);
		while(l <= r)
		{
			int mid = (l + r) >> 1;
			if(n - (lower_bound(a + 1 , a + n + 1 , k * 2 - a[mid]) - a - 1) + mid <= m) l = mid + 1 , ans = mid;
			else r = mid - 1;
		}
		printf("%lld\n" , sum[ans] + 2 * k * (m - ans) + sum[n - m + ans] - sum[n]);
	}
	return 0;
}