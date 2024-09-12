#include <cstdio>
#define r return
int N,K,n[21],m,s,c;
bool p(int n){for(int i=2;i*i<=n;i++)if(!n%i)r 0;r 1;}
void f(int d){if (d==K) {if(p(s))c++;r;}int q=m;for(int i=m+1;i<=N;i++) {m=i;s+=n[i];f(d+1);s-=n[i];}m=q;}
int main(){scanf("%d%d",&N,&K);for(int i=1;i<=N;i++)scanf("%d",n+i);f(0);printf("%d\n",c);r 0;}