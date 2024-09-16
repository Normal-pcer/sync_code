#include <cstdio>
#define l long long
#define r return
l n[22];l m[22]; l waysCount(int s, bool u) {if(s==1)r 2+u;if(u&&n[s])r n[s];if(!u&&m[s])r m[s];l _=waysCount(s-1,1);_+=(1+u)*waysCount(s-1,0);(u?n:m)[s]=_;r _;}int main() {int n;scanf("%d", &n);printf("%lld\n", waysCount(n, true));r 0;}