#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int mod=998244353;
void add(int &x,int y){
    x+=y;
    if(x>=mod)x-=mod;
}
const int N=105;
int f[N][N][N],g[N][N],n,k;
vector<int> e[N];
int siz[N];
void dfs(int p,int fa){
    f[p][1][0]=1;
    siz[p]=1;
    for(int t:e[p]){
        if(t==fa)continue;
        dfs(t,p);
        for(int i=0;i<=siz[p];++i)
            for(int j=0;j<=siz[p];++j)
                g[i][j]=f[p][i][j],f[p][i][j]=0;
        for(int i=1;i<=siz[p];++i)
            for(int j=0;j<=siz[p];++j)
                add(f[p][i][j+1],g[i][j]);
        for(int i=1;i<=siz[p];++i)
            for(int j=0;j<=siz[p];++j)
                for(int k=1;k<=siz[t];++k)
                    for(int l=0;l<=siz[t];++l)
                        add(f[p][i+k][j+l],(ll)g[i][j]*f[t][k][l]%mod);
        siz[p]+=siz[t];
    }
}
int fac[N],ifac[N];
int fastmi(int x,ll n){
    int res=1;
    for(;n;n>>=1,x=(ll)x*x%mod)
        if(n&1)res=(ll)res*x%mod;
    return res;
}
int main(){
    cin>>n>>k;int x,y;
    for(int i=1;i<n;++i){
        scanf("%d%d",&x,&y);
        e[x].push_back(y);
        e[y].push_back(x);
    }
    fac[0]=ifac[0]=1;
    for(int i=1;i<=n;++i){
        fac[i]=(ll)fac[i-1]*i%mod;
        ifac[i]=fastmi(fac[i],mod-2);
    }
    dfs(1,0);
    int ans=0;
    for(int i=1;i<=n;++i)
        for(int j=k+1;j<=n;++j)
            for(int k=0;k<=n;++k)if(f[i][j][k]){
                int x=j,y=k+(i!=1),v=f[i][j][k];
                add(ans,(ll)fac[x]*fac[y]%mod*ifac[x+y]%mod*v%mod);
            }
    cout<<ans;
}