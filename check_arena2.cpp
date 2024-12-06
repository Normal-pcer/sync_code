#include<bits/stdc++.h>
using namespace std;
int t,n,ans,p[100009],q[100009],e[100009],f[100009],g[100009],h[100009];
string a,b,c,d;
int main(){
    cin>>t;
    while(t--){
        cin>>n>>a>>b>>c>>d,p[0]=q[0]=0,(a[0]&1)?++f[0]:++e[0],(b[0]&1)?++h[0]:++g[0],ans=0;
        for(int i=1;i<n;++i) p[i]=((c[i]&1)&&(c[i-1]&1)?p[i-1]:i),(a[i]&1)?++f[p[i]]:++e[p[i]];
        for(int i=1;i<n;++i) q[i]=((d[i]&1)&&(d[i-1]&1)?q[i-1]:i),(b[i]&1)?++h[q[i]]:++g[q[i]];
        for(int i=0;i<n;++i){
            if(e[p[i]]&&g[q[i]]) ++ans,--e[p[i]],--g[q[i]];
            else if(f[p[i]]&&h[q[i]]) ++ans,--f[p[i]],--h[q[i]];
            else if(e[p[i]]) --e[p[i]],--h[q[i]];
            else --f[p[i]],--g[q[i]];
        }
        cout<<ans<<"\n";
    }
    return 0;
}