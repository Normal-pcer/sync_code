#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
void solve()
{
    int n;
    cin>>n;
    vector<int>a(n+10),b(n+10),p(n);
    vector<ll>t(n+10);
    map<int,int>odt;
    for(int i=1;i<=n;++i)
    {
        cin>>a[i]>>b[i]>>t[i];
        odt[i]=a[i]-i;
    }
    odt[n+1]=INT_MAX;
    odt[0]=INT_MIN;
    iota(p.begin(),p.end(),1);
    sort(p.begin(),p.end(),[&](int i,int j)
         {
             return t[i]<t[j];
         }); // 钦定最优顺序
    auto odiv=[&](int k) //分裂一个颜色段
    {
        auto it=prev(odt.upper_bound(k));
        odt.insert(it,{k,it->second});
    };
    auto assignR=[&](int i,int k) //查询并覆盖右边
    {
        odiv(i);
        auto it=odt.lower_bound(i);
        ll ret=0;
        for(;it->second<k;it=odt.erase(it))
            ret+=(next(it)->first-it->first)*1LL*it->second;
        odt[i]=k;
        return 1LL*(it->first-i)*k-ret;
    };
    auto assignL=[&](int i,int k) //查询并覆盖左边
    {
        odiv(i+1);
        auto it=prev(odt.lower_bound(i+1));
        ll ret=0;
        for(;it->second>k;--it);
        int tmp=(++it)->first;
        for(;it->first<=i;it=odt.erase(it))
            ret+=(next(it)->first-it->first)*1LL*it->second;
        odt[tmp]=k;
        return ret-1LL*(i+1-tmp)*k;
    };
    ll T=0;
    for(int i:p)
    {
        if(a[i]>b[i])
            T+=assignL(i,b[i]-i);
        if(a[i]<b[i])
            T+=assignR(i,b[i]-i);
        if(t[i]<T)
        {
            cout<<"No\n";
            return;
        }
    }
    cout<<"Yes\n";
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int c,t;
    cin>>c>>t;
    while(t--)
        solve();
    return 0;
}