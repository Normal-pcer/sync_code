#include<bits/stdc++.h>
using namespace std;
int f[100001];//并查集。
struct Node{//储存敌人的结构体。
	set<int>a;
}e[100001];
int n,m,q;
void read(int &x){//快读。
	char ch=getchar();
	x=0;
	while(ch<'0'||ch>'9'){
		ch=getchar();
	}
	while(ch>='0'&&ch<='9'){
		x*=10;
		x+=ch-'0';
		ch=getchar();
	}
}
int find(int x){//并查集的实现。
	if(f[x]!=x) f[x]=find(f[x]);
	return f[x];
}
void hb(int a,int b){
	n--;
	for(auto it=e[b].a.begin();it!=e[b].a.end();++it){//auto仅支持c++14以上。
		if(e[*it].a.size()<n){//小优化。
			e[*it].a.insert(a);
		}
		e[*it].a.erase(b);
		if(e[a].a.size()<n){//小优化。
			e[a].a.insert(*it);
		}
	}
	e[b].a.clear();
	f[b]=a;
}
int main(){
	read(n);
	read(m);
	read(q);
	for(int i=1;i<=n;i++){
		f[i]=i;
	}
	for(int i=1;i<=m;i++){
		int u,v;
		cin>>u;
		cin>>v;
		e[u].a.insert(v);
		e[v].a.insert(u);
	}
	for(int i=1;i<=q;i++){
		int a,b;
		cin>>a;
		cin>>b;
		a=find(a);
		b=find(b);
		if(!e[a].a.count(b)&&a!=b){
			if(e[a].a.size()<e[b].a.size()){//启发式合并。
				hb(b,a);
			}
			else{
				hb(a,b);
			}
			puts("agree.");
		}
		else{
			puts("no!");
		}
	}
	return 0;
}