#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<int,int> pii;

int n;
string s1,s2,t1,t2;
int a1[100001],a2[100001];
vector<pii> v1,v2;
int ans;

void dfs2(int dep){
	if (dep==v2.size()){
		//for (int i=1;i<=n;i++) cout<<a1[i];
		//cout<<"\n";
		//for (int i=1;i<=n;i++) cout<<a2[i];
		//cout<<"*\n";
		int x=0;
		for (int i=1;i<=n;i++) if (a1[i]==a2[i]) x++;
		ans=max(ans,x);
		return;
	}
	auto [l,r]=v2[dep];
	int len=r-l+1;
	int cnt=0;
	for (int i=l;i<=r;i++) if (s2[i]=='1') cnt++;
	for (int i=0;i<(1<<len);i++){
		if (__builtin_popcount(i)==cnt){
			for (int j=0,k=l;j<len;j++,k++){
				a2[k]=((i>>j)&1);
			}
			dfs2(dep+1);
		}
	}
}

void dfs1(int dep){
	if (dep==v1.size()){
		dfs2(0);
		return;
	}
	auto [l,r]=v1[dep];
	int len=r-l+1;
	int cnt=0;
	for (int i=l;i<=r;i++) if (s1[i]=='1') cnt++;
	for (int i=0;i<(1<<len);i++){
		if (__builtin_popcount(i)==cnt){
			for (int j=0,k=l;j<len;j++,k++){
				a1[k]=((i>>j)&1);
			}
			dfs1(dep+1);
		}
	}
}

int main(){
	ios::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
	int T;
	cin>>T;
	while (T--){
		vector<pii>().swap(v1);
		vector<pii>().swap(v2);
		cin>>n>>s1>>s2>>t1>>t2;
		s1='_'+s1;
		s2='_'+s2;
		t1='0'+t1;
		t2='0'+t2;
		int lst=1;
		for (int i=1;i<=n;i++){
			if (t1[i]=='0'){
				if (lst<i-1) v1.push_back({lst,i-1});
				lst=i+1;
			}
		}
		if (lst<n) v1.push_back({lst,n});
		sort(v1.begin(),v1.end());
		lst=1;
		for (int i=1;i<=n;i++){
			if (t2[i]=='0'){
				if (lst<i-1) v2.push_back({lst,i-1});
				lst=i+1;
			}
		}
		if (lst<n) v2.push_back({lst,n});
		sort(v2.begin(),v2.end());
		//for (auto [x,y]:v1) cout<<x<<" "<<y<<"\n";
		//cout<<"*\n";
		//for (auto [x,y]:v2) cout<<x<<" "<<y<<"\n";
		//cout<<"*\n";
		if (v1==v2){
			for (auto [x,y]:v1){
				sort(s1.begin()+x,s1.begin()+y+1);
				sort(s2.begin()+x,s2.begin()+y+1);
			}
			//cout<<s1<<"\n"<<s2<<"\n";
			int ans=0;
			for (int i=1;i<=n;i++) if (s1[i]==s2[i]) ans++;
			cout<<ans<<"\n";
		}else{
			bool flag=true;
			for (int i=1;i<n;i++){
				if (s1[i]!=s1[i+1]){
					flag=false;
					break;
				}
			}
			if (flag){
				char c=s1[1];
				cout<<count(s2.begin()+1,s2.end(),c)<<"\n";
			}else{
				if (n<=10){
					ans=0;
					for (int i=1;i<=n;i++){
						a1[i]=(s1[i]=='1');
						a2[i]=(s2[i]=='1');
					}
					dfs1(0);
					cout<<ans<<"\n";
				}else{
					cout<<"qwq\n";
				}
			}
		}
	}
	return 0;
}