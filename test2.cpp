#include<bits/stdc++.h>
using namespace std;
int sm[401][401];
int hi[50],ti[50],ki[50];
int main(){
    int h,t,n;
    cin>>h>>t>>n;
    for(int i=0;i<n;i++){
        cin>>hi[i]>>ti[i]>>ki[i];
    }
    for(int i=0;i<n;i++){
        for(int j=h;j>=hi[i];j--){
            for(int k=t;k>=ti[i];k--){
                if(sm[j-hi[i]][k-ti[i]]+ki>sm[j][k]){
                    sm[j][k]=sm[j-hi[i]][k-ti[i]]+ki;
                }
            }
        }
    }
    cout<<sm[h][t];
    return 0;
}