#include <cstdio>
#define l long long
#define r return
l m[22];l n[22];l w(int s,bool u){if(s==1)r u?3:2;if(u&&!m[s])r m[s];if(!u&&!n[s])r n[s];l _=w(s-1,1);_+=(1+u)*w(s-1,0);if(!u)n[s]=_;else m[s]=_;r _;}
void main(){int n;scanf("%d",&n);printf("%lld\n",w(n,1));}