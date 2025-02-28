#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

int n,q;
int a[100001];

int main(){
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.tie(nullptr);
	cin>>n>>q;
	for (int i=1;i<=n;i++) cin>>a[i];
	while (q--){
		int x,y;
		cin>>x>>y;
		swap(a[x],a[y]);
		cout<<(is_sorted(a+1,a+n+1)?"Yes":"No")<<"\n";
	}
	return 0;
}
