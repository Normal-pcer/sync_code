#include<stdio.h>
main(){int x,y;scanf("%d%d",&x,&y);__builtin_add_overflow(x,y,&x);printf("%d\n",x);return 0;}
