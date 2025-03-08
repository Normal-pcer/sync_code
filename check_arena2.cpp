#include<bits/stdc++.h>
#include<ext/pb_ds/priority_queue.hpp>
#define int long long
using namespace std;
const int N=2e5+5;
struct box{
	int a,b,t,id;
	int l1,r1;
}a[N];
int idt[N];
int mov[N];
__gnu_pbds::priority_queue<int,greater<int>>qmn[N];
__gnu_pbds::priority_queue<int,less<int>>qmx[N];
void mian(){
	int n;cin>>n;
	for(int i=0;i<=n+3;i++)mov[i]=idt[i]=a[i].a=a[i].b=a[i].t=a[i].id=a[i].l1=a[i].r1=0;
	for(int i=1;i<=n;i++)cin>>a[i].a>>a[i].b>>a[i].t,a[i].id=i;
	stack<int>st;
	for(int i=1;i<=n;i++){
		while(!st.empty()&&a[st.top()].t>a[i].t)st.pop();
		if(!st.empty())a[i].l1=st.top();
		st.emplace(i);
	}
	while(!st.empty())st.pop();
	for(int i=n;i>=1;i--){
		while(!st.empty()&&a[st.top()].t>a[i].t)st.pop();
		if(!st.empty())a[i].r1=st.top();
		st.emplace(i);
	}
	sort(a+1,a+1+n,[](box aa,box bb){
		if(aa.t!=bb.t)return aa.t>bb.t;
		if((aa.a<aa.b)!=(bb.a<bb.b))return aa.a<aa.b;
		if(aa.a<aa.b)return aa.a>bb.a;
		else return aa.a<bb.a;
	});
	for(int i=1;i<=n;i++)idt[a[i].id]=i;
	for(int i=1;i<=n;i++){
		qmx[i].clear();qmx[i].push(a[i].a);
		qmn[i].clear();qmn[i].push(a[i].a);
	}
	for(int i=1;i<=n;i++){
		if(a[i].a<a[i].b){
			int x=idt[a[i].l1];
			while(qmx[i].size()&&(!x||qmx[i].top()>=a[x].b+(int)(qmx[x].size()+qmx[i].size())-1)){
				mov[i]+=(a[i].b+qmx[i].size()-1)-qmx[i].top();
				qmx[i].pop();
			}
			if(x)mov[i]+=qmx[i].size()*(a[i].b-(a[x].b+qmx[x].size()));
			qmx[x].join(qmx[i]);
		}else{
			int x=idt[a[i].r1];
			while(qmn[i].size()&&(!x||qmn[i].top()<=a[x].b-(int)(qmn[x].size()+qmn[i].size())+1)){
				mov[i]+=qmn[i].top()-(a[i].b-qmn[i].size()+1);
				qmn[i].pop();
			}
			if(x)mov[i]+=qmn[i].size()*((a[x].b-qmn[x].size())-a[i].b);
			qmn[x].join(qmn[i]);
		}
	}
	for(int i=1;i<=n;i++){
		int x=mov[i];
		mov[i]=min(mov[i],a[i].t-a[i+1].t);
		x-=mov[i];
		mov[i+1]+=x;
	}
	if(mov[n+1]){
		cout<<"No\n";
	}else{
		cout<<"Yes\n";
	}
}
signed main(){
	ios::sync_with_stdio(0);cin.tie(0);
	int c,t;cin>>c>>t;
	while(t--){
		mian();
	}
	return 0;
}