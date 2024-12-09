#include<bits/stdc++.h>
using namespace std;
void solve(){
	vector<int> a,b,c,d,sa,sb;
    int n;
    cin>>n;
    a.resize(n+5,0);
    b.resize(n+5,0);
    c.resize(n+5,0);
    d.resize(n+5,0);
    sa.resize(n+5,0);
    sb.resize(n+5,0);
    int ans=0;
    for(int i=1;i<=n;i++){
        sa.at(i)=sb.at(i)=0;
        scanf("%1d",&a.at(i));
    }
    for(int i=1;i<=n;i++){
        scanf("%1d",&b.at(i));
    }
    for(int i=1;i<=n;i++){
        scanf("%1d",&c.at(i));
    }
    for(int i=1;i<=n;i++){
        scanf("%1d",&d.at(i));
    }
    for(int i=1;i<=n;i++){
        sa.at(i)=sa.at(i-1)+a.at(i);
        sb.at(i)=sb.at(i-1)+b.at(i);
    }
    for(int i=1;i<=n;i++){
        if(c.at(i)==0&&d.at(i)==0&&a.at(i)==b.at(i))ans++;
    }
    int i=1,j=1,t=1,sti=c.at(1),stj=d.at(1);
    for(;t<=n;){
    	
        sti=c.at(i),stj=d.at(j);
        while(i<n and c.at(i+1)==sti)i++;
        while(j<n and d.at(j+1)==stj)j++;
        //cout<<t<<" "<<i<<" "<<j<<" "<<ans<<endl;
        //if(t>i||t>j)cout<<"yaeh"<<endl;
        if(i>=j){
            if(sti==1){
                int x=sa.at(i)-sa.at(t-1);
                int y=sb.at(j)-sb.at(t-1);
                int leni=i-t+1,lenj=j-t+1;
//                cout<<x<<" "<<y<<endl;
//                if(lenj<0||leni<0)cout<<"yeah"<<endl;
//                if(leni<x||lenj<y)cout<<"yeah"<<endl;
                ans+=min(x,y);
//            	cout<<ans<<endl;
                ans+=min(leni-x,lenj-y);
                int tmp=max(min(x,y),(lenj-(leni-x)));
                sa.at(j)=sa.at(t-1)+tmp;
                for(int k=j+1;k<=i;k++){
                    sa.at(k)=min(sa.at(i),sa.at(k-1)+1);
                }
                if(j<n)j++;
                t=j;
                i=max(i,t);
            }else if(stj==1){
                int x=sa.at(j)-sa.at(t-1);
                int y=sb.at(j)-sb.at(t-1);
                int leni=j-t+1,lenj=j-t+1;
                ans+=min(x,y);
                ans+=min(leni-x,lenj-y);
                if(j<n)j++;
                t=j;
                i=max(t,i);
            }else{
                if(j<n)j++;
                t=j;
                i=max(t,i);
            }
        }else{
            if(stj==1){
                int x=sa.at(i)-sa.at(t-1);
                int y=sb.at(j)-sb.at(t-1);
                int leni=i-t+1,lenj=j-t+1;
                ans+=min(x,y);
                ans+=min(leni-x,lenj-y);
                sb.at(i)=sb.at(t-1)+min(x,y);
                for(int k=i+1;k<=j;k++){
                    sb.at(k)=min(sb.at(j),sb.at(k-1)+1);
                }
                if(i<n)i++;
                t=i;
                j=max(j,t);
            }else if(sti==1){
                int x=sa.at(i)-sa.at(t-1);
                int y=sb.at(i)-sb.at(t-1);
                int leni=i-t+1,lenj=i-t+1;
                ans+=min(x,y);
                ans+=min(leni-x,lenj-y);
                if(i<n)i++;
                t=i;
                j=max(t,j);
            }else{
                if(i<n)i++;
                t=i;
                j=max(t,j);
            }
        }
		if(j==n&&i==n)break;
    }
    cout<<ans<<endl;
}
int main(){
//    freopen("edit.in","r",stdin);
//    freopen("edit.out","w",stdout);
    int T;
    cin>>T;
    while(T--){
        solve();
    }
}
/*
1
9
111110011
011011111
110111110
111111111
8
*/
/*
10
9
111101010
101011100
101111111
111111011
9
001110100
111010101
111111111
011011111
8
01000100
00000000
11111111
11111110
9
111110011
011011111
110111110
111111111
8
00100000
00111110
11011001
11110101
7
0111110
1111111
1111101
1011011
7
1101000
0000000
0010001
1110111
10
0000000000
1000110111
1011111111
0111111000
8
11110000
01101111
11101111
10111011
8
11100010
10011011
10111011
11111111
*/
/*
1
10
0000000000
1000110111
1011111111
0111111000
*/