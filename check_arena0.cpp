#include<bits/stdc++.h>
using namespace std;
const int N=3e4+5;
int w,n;
int a[N];
int ans;
int main()
{
	cin>>w>>n;
	for(int i=1;i<=n;i++)
	{
		cin>>a[i];
	}
	sort(a+1,a+1+n);
//	for(int i=1;i<=n;i++)
//	{
//		cout<<a[i]<<' ';
//	}
//	cout<<endl;
	int j=n;
	for(int i=1;i<=n/2;i++)
	{
		if(j<=i)
		{
			break;
		}
		if(a[i]+a[j]>w&&i!=j)
		{
            // 不能减到 i <= j
			while(a[i]+a[j]>w&&i!=j)
			{
				j--;
			}
		}
		// cout<<i<<' '<<j<<endl;
        // 判断，如果 i == j 说明是单个物品
        if (i != j)  ans++;  // 包含两个物品的组数
		j--;
	}
//	cout<<endl;
	cout<<ans+n-ans*2;
	return 0;
}