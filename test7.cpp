#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int mod=998244353;
const int N=1e6+5;
int p[N],q[N],n;
//bool check(){
//    for(int i=1;i<=n;++i)
//        q[p[i]]=i;
//    for(int i=1;i<=n;++i)
//        for(int j=i+1;j<=n;++j)
//            if(p[i]>p[j]&&q[i]<q[j])
//                return false;
//    return true;
//}
int a[N];
int f[N];
int main(){
    cin>>n;
    for(int i=1;i<=n;++i)
        scanf("%d",&a[i]);
    for(int i=1;i<=n;++i){
        if(!a[i]||a[i]==i)continue;
        int x=a[i];
        if(!a[x])a[x]=i;
        else if(a[x]!=i){
            puts("0");
            return 0;
        }
    }
    int cnt=0;
    for(int i=1;i<=n;++i)
        if(a[i]==0)cnt++;

    f[0]=f[1]=1,f[2]=2;
    for(int i=3;i<=n;++i)
        f[i]=(f[i-1]+(ll)f[i-2]*(i-1)%mod)%mod;
    cout<<f[cnt];
    
    
        
//    int cnt=0;
//    for(int i=1;i<=n;++i)
//        p[i]=i;
//    do{
//        if(check()){
//            bool pd=1;
//            for(int i=1;i<=n;++i)
//                if(a[i]&&p[i]!=a[i]){
//                    pd=0;
//                    break;
//                }
//            cnt+=pd;
//            for(int i=1;i<=n;++i)
//                cout<<i<<" "<<p[i]<<"\n";
//            cout<<endl;
//        }
//    }while(next_permutation(p+1,p+n+1));
//    cout<<cnt;
}