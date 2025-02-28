#include<bits/stdc++.h>
using namespace std;
int a[10000003];
int b[10000003];
int bin[10000003];
int flag;
int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
	int n,m;
	scanf("%d%d",&n,&m);
	for(int i=1;i<=n;i++){
		scanf("%d",&a[i]);
		b[i]=a[i];
	}
	sort(b+1,b+n+1);
	for(int i=1;i<=n;i++){
		if(a[i]==b[i]){
			flag++;
			bin[i]=1;
		}
	}
	while(m--){
		int u,v;
		scanf("%d%d",&u,&v);
		swap(a[u],a[v]);
		if(!bin[u]&&a[u]==b[u]){
			flag++;
			bin[u]++;
		}
		if(!bin[v]&&a[v]==b[v]){
			flag++;
			bin[v]++;
		}
		if(bin[u]&&a[u]!=b[u]){
			flag--;
			bin[u]--;
		}
		if(bin[v]&&a[v]!=b[v]){
			flag--;
			bin[v]--;
		}
		if(flag==n)cout<<"Yes"<<"\n";
		else cout<<"No"<<"\n";
	}
}