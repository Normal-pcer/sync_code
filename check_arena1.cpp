#include<bits/stdc++.h>
#define N 1200000
using namespace std;
struct node
{
	int a,b,c,id;
}a[1010],b[N];
int f[100010],ans[N];
bool comp(node aa,node bb)
{
	return aa.a<bb.a;
}
int n,m;
int main()
{
	scanf("%d",&n);
	for(int i=1;i<=n;++i) scanf("%d%d%d",&a[i].c,&a[i].a,&a[i].b);
	scanf("%d",&m);
	for(int i=1;i<=m;++i) scanf("%d%d%d",&b[i].a,&b[i].b,&b[i].c),b[i].id=i;
	sort(a+1,a+n+1,comp);
	sort(b+1,b+m+1,comp);
	int j=1;f[0]=1e9;
	for(int i=1;i<=m;++i)
	{
		while(j<=n&&a[j].a<=b[i].a)
		{
			for(int k=100000;k>=a[j].c;--k)
			f[k]=max(f[k],min(f[k-a[j].c],a[j].b));
			j++;
		}
		if(f[b[i].b]>b[i].a+b[i].c) ans[b[i].id]=1;
	}
	for(int i=1;i<=m;++i) puts(ans[i]?"TAK":"NIE");
	return 0;
}