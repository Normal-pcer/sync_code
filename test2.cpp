#include<bits/stdc++.h>
#define sqr(x) (ll)x*x
using namespace std;
typedef long long ll;
const int N=4e5+10;
int n,t[N];

struct node{
    int x,y;
}a[N];

bool cmp1(node x,node y)
{
    if(x.x==y.x) return x.y<y.y;
    return x.x<y.x;
}

bool cmp2(int x,int y)
{
    return a[x].y<a[y].y;
}

ll dist(int x,int y)
{
    return sqr((a[x].x-a[y].x))+sqr((a[x].y-a[y].y));
}

ll work(int l,int r)
{
    if(l==r) return 5e18;
    if(r-l==1) return dist(l,r);
    int mid=(l+r)>>1;
    ll d=min(work(l,mid),work(mid+1,r));
    int cnt=0;
    for(int i=l;i<=r;i++)
        if(sqr(a[i].x-a[mid].x)<d) t[++cnt]=i;
    sort(t+1,t+cnt+1,cmp2);
    for(int i=1;i<=cnt;i++)
        for(int j=i+1;j<=cnt&&sqr(a[t[j]].y-a[t[i]].y)<d;j++) d=min(d,dist(t[i],t[j]));
        //                    (ll)a[t[j]].y-a[t[i]].y*a[t[j]].y-a[t[i]].y
    return d;
}

int main()
{
    scanf("%d",&n);
    for(int i=1;i<=n;i++) scanf("%d%d",&a[i].x,&a[i].y);
    sort(a+1,a+n+1,cmp1);
    printf("%lld\n",work(1,n));
    return 0;
}