#include<bits/stdc++.h>
#define int long long
using namespace std;
int N , ans = 1000000000000000000;
signed main()
{
	scanf("%lld" , &N);
	int l = 0 , r = 1145141;
	while(l <= r)
	{
		while(l * l * l + l * l * r + l * r * r + r * r * r >= N) r --;
		assert(l * l * l + l * l * r + l * r * r + r * r * r < N);
		r ++;
        if (l > r)  break;
        std::cout << std::format("l={} r={} ans={}", l, r, ans) << std::endl;
		ans = min(ans , l * l * l + l * l * r + l * r * r + r * r * r);
		l ++;
	}
	printf("%lld" , ans);
	return 0;
}