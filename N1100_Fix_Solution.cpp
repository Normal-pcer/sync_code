#include<cmath>
#include<cstdio>
#include<algorithm>
#include <bits/stdc++.h>
#define inf 2147483646.9
using namespace std;
int n,now,fix,op,i,j,k,num,er[20];
double l,f[500005];
struct node1
{
    int x,y;
}a[20];
struct node
{
    double val;
    int id;
}dis[20][20];
double sqr(int x)
{
    return 1.0*x*x;
}
bool cmp(node x,node y)
{
    return x.val<y.val;
}
int main()
{
    freopen("N1100_input.in", "r", stdin);
    freopen("N1100_output.out", "w", stdout);
    while (true)
    {
        scanf("%d",&n);
        if (n==0) break;
        now=0;
        fix=0;
        for (i=1;i<=n;i++)
        {
            scanf("%d%d%d",&a[i].x,&a[i].y,&op);
            if (op==1)
            {
                fix++;
                now+=(1<<(i-1));
            }
        }
        if (fix==n)
        {
            printf("0.000000\n");
            continue;
        }
        if (fix<2)
        {
            printf("No Solution\n");
            continue;
        }
        for (i=1;i<=n;i++)
        {
            for (j=1;j<=n;j++)
            {
                dis[i][j].val=sqrt(sqr(a[i].x-a[j].x)+sqr(a[i].y-a[j].y));
                dis[i][j].id=j;
            }
            sort(dis[i]+1,dis[i]+n+1,cmp);
        }
        for (i=0;i<=n;i++)
            er[i]=1<<i;
        for (i=1;i<=(1<<n);i++)
            f[i]=inf;
        f[now]=0;
        for (i=now;i<(1<<n);i++)
        {
            if (f[i]!=inf)
                for (j=1;j<=n;j++)
                    if ((i&(1<<(j-1)))==0)
                    {
                        num=0;
                        l=0;
                        for (k=1;k<=n;k++)
                            if ((i&(1<<(dis[j][k].id-1)))!=0)
                            {
                                num++;
                                l+=dis[j][k].val;
                                if(num==2)
                                {
                                    f[i+(1<<(j-1))]=min(f[i+(1<<(j-1))],f[i]+l);
                                    break;
                                }
                            }
                    }
        }
        printf("%.6f\n",f[(1<<n)-1]);
    }
    return 0;
}
