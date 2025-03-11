#include<bits/stdc++.h>
using namespace std;

long long n, m, s, e, k, nfish;

struct mat
{
	long long a[51][51]; //邻接矩阵
	void unit() //构造单位矩阵
	{
		memset(a, 0, sizeof(a));
		for (long long i = 1; i <= n; i++) a[i][i] = 1;
	}
	mat operator *(const mat &b) const //矩阵乘法
	{
		mat c; memset(c.a, 0, sizeof(c.a));
		for (long long i = 1; i <= n; i++)
			for (long long k = 1; k <= n; k++)
				for (long long j = 1; j <= n; j++)
					c.a[i][j] |= a[i][k] & b.a[k][j];//只需知道两点之间是否联通，因此使用位运算
		return c;
	}
	mat operator ^(const long long &b) const //矩阵快速幂
	{
		mat t, res;
		res.unit();
		t = *this;
		long long qwq = b;
		while (qwq)
		{
			if (qwq & 1) res = res * t;
			t = t * t;
			qwq >>= 1;
		}
		return res;
	}
} tmp, ans, qaq[12], mp;

signed main()
{
	scanf("%lld%lld%lld%lld%lld", &n, &m, &s, &e, &k);
	for (long long i = 1; i <= m; i++)
	{
		long long x, y, z;
		scanf("%lld%lld%lld", &x, &y, &z);
		mp.a[x][y] = mp.a[y][x] = z;
	}
	for (long long i = 0; i < 12; i++) qaq[i] = mp;
	scanf("%lld", &nfish);
	for (long long i = 1; i <= nfish; i++)
	{
		long long t;
		scanf("%lld", &t);
		for (long long j = 0; j < t; j++)
		{
			long long x;
			scanf("%lld", &x);
			for (long long k = j; k < 12; k += t)
				for (long long o = 1; o <= n; o++)
					qaq[k].a[o][x] = 0; //得出在怪物影响下的图
		}
	}	
	long long l = -1, r = 1ll << 60;
	while (l + 1 < r) //二分答案
	{
		long long mid = (l + r) >> 1;
		tmp.unit();
		mat ouo[12]; 
		for (long long i = 0; i <= 11; i++) ouo[i] = qaq[i];
		for (long long i = 0; i < 12; i++)
			for (long long j = 1; j <= n; j++)
			 	for (long long l = 1; l <= n; l++) if (ouo[i].a[j][l]) ouo[i].a[j][l] = ouo[i].a[j][l] <= mid;
		for (long long i = 1; i < 12; i++) tmp = tmp * ouo[i];//12个单位时间为一组，处理tmp
		tmp = tmp * ouo[0];
		long long cnt = (k / 12);
		ans.unit();
		ans = ans * (tmp ^ cnt);
		for (long long i = 1; i <= k % 12; i++) ans = ans * ouo[i];//将余下的也乘上去
		if (ans.a[s][e]) r = mid; else l = mid; //判断是否能够到达
	}
	if (l > 1ll << 30) puts("'IMP0SSBLE!!'"); //无解情况
		else printf("%lld", r);
}