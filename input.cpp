#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=(n);i++)
#define from(i,b,e) for(int i=(b);i<=(e);i++)
#define rev(i,e,b) for(int i=(e);i>=(b);i--)
#define main() main(int argc, char const *argv[])
#define optimizeIO std::ios::sync_with_stdio(false); std::cin.tie(0); std::cout.tie(0);
#define chkMax(base,cmp...) (base=std::max({(base),##cmp}))
#define chkMin(base,cmp...) (base=std::min({(base),##cmp}))
#define chkMaxEx(base,exchange,other,cmp...) {auto __b__=base;if(__b__!=chkMax(base,##cmp)){exchange;} else other;}
#define chkMinEx(base,exchange,other,cmp...) {auto __b__=base;if(__b__!=chkMin(base,##cmp)){exchange;} else other;}
#define update(base,op,modify...) base=op((base),##modify)
#define ensure(con, otw) ((con)? (con): (otw))
#define check(v, con, otw) (((v) con)? (v): (otw))
#define optional(ptr) if(ptr)ptr
#define never if(0)
#define always if(1)
#define bitOr(x,y) (((x)&(y))^(((x)^(y))|(~(x)&(y))))
#define Infinity 2147483647
#define putInt(n) printf("%d\n",(n))
#define compare(x,y,g,e,l) (((x)>(y))?(g):(((x)<(y))?(l):(e)))
bool DEBUG_MODE=false;
typedef long long ll; typedef unsigned long long ull;
inline void batchOutput(int *begin, int n, const char *format){upto(i, n)printf(format, begin[i]);printf("\n");} inline void batchOutput(int*begin, int n) {batchOutput(begin,n,"%3d ");}
#define batchOutput2d(b, r, c, fmt) upto(i,r){upto(j,c)printf(fmt,b[i][j]);printf("\n");}
template <class T=int>inline T read(){ T x=0;int f=1;char c;while((c=getchar())<'0'||c>'9')if(c=='-')f=-1;do{x=(((x<<2)+x)<<1)+c-'0';}while((c=getchar())>='0'&&c<='9');return x*f; }

const int _N = 53; int N; int max[_N*2][_N*2], min[_N*2][_N*2];

int v[_N*2];
char e[_N*2];

int eval(char c, int a, int b) {
    return ((c=='t') ? (a+b) : (a*b));
}

int main() {
    optimizeIO;
    initDebug;

    std::cin >> N;

    debug {
        freopen("171.out", "w", stdout);
    }

    upto(i, N) {
        std::cin >> e[i] >> v[i];
    }

    memcpy(e+1+N, e+1, sizeof(e[0])*N);
    memcpy(v+1+N, v+1, sizeof(v[0])*N);

    debug upto(i, 2*N)  printf("%c", e[i]);
    debug batchOutput(v, 2*N);

    memset(max, -0x3f, sizeof(max));
    memset(min, 0x3f, sizeof(min));
    upto(i, N*2) {
        max[i][i] = min[i][i] = v[i];
        max[i][i+1] = min[i][i+1] = eval(e[i+1], v[i], v[i+1]);
    }

    from(l, 2, N-1) {  // 区间长度
        upto(i, N*2) {  // 区间起点
            int j = i+l;  // 区间终点
            if (j>N*2)  break;
            from(k, i+1, j) {  // 合并线段
                chkMax(max[i][j], 
                    eval(e[k], max[i][k-1], max[k][j]),
                    eval(e[k], min[i][k-1], min[k][j])
                );
                chkMin(min[i][j],
                    eval(e[k], max[i][k-1], max[k][j]), 
                    eval(e[k], min[i][k-1], min[k][j])
                );
            }
        }
    }
    debug batchOutput2d(max, N*2, N*2, "%13d")
    debug batchOutput2d(min, N*2, N*2, "%13d")
    int ans = 0;
    upto(i, N) {
        chkMax(ans, max[i][i+N-1]);
        chkMax(ans, min[i][i+N-1]);
    }
    printf("%d\n", ans);
    bool flag = false;
    upto(i, N) {
        if (ans == max[i][i+N-1] || ans == min[i][i+N-1]) {
            if (flag)  printf(" ");
            printf("%d", i);
            flag = true;
        }
    }
    printf("\n");
    return 0;
\
#include<bits/stdc++.h\
>
         #define       ll     long       long
      u\
sing     names\
pace        std       ;
    inline     ll    read         (       ){
        	l\
l     x        =0,y   \
=1     ;
    	cha\
r     c    =get\
char       (       )   ;
     	while       (c\
<'0'||c>'9'){
		if(c=='-')y=-1;
		c=getchar();
	}
	while(c>=\
'0'&&c<='9'\
)x=(x<<3)+(x<<1)+c-'0',c=getchar();
\
	return x*y;
}
ll T,M,a,b,c,delta;
ll gcd(ll\
 a,ll\
 b){
	if(b==0)return a;
\
	return gcd(b,a\
%b);
}
in\
t main(){
	T=read();M=read();
	w\
hile(T\
--\
){
		a=\
read();b=read();\
c=read\
();
		delta=b*b-4*a*c\
;
		if(\
delta<0){
			printf("NO\n");
		}else if(del\
ta==\
0){
			ll p=-\
b,q=2*a;
			ll pq=gcd(abs(q),abs(p));
			p/=pq,q/=pq;
			if(q<0)q=-q,p=-p;
			if(q=\
=1)printf("%lld\n",p);
			else printf("%lld/%lld\n",p,\
q);
		}else\
{
			ll p=-b,q=2*a;
			ll \
sq=(ll)sqrt(delta);
			if(sq\
*sq==delta){
				if(q>0)p+=sq;
				\
else p-=sq;
				ll pq=gcd(abs(q),abs(p));
				p\
/=pq,q/=pq;
				if(q<0\
)\
q=-q,p=-p;
			\
	if(q==1)\
printf("%lld\\
n",p);
			\
	els\
e printf("\
%lld\
/%lld\n",p,q)\
;
	\
		}else{
\
				ll\
 pq=g\
cd(abs(q),abs(p));
				p/=pq,q/=pq;
				if(q<0)q=-q,p=-p;
				i\
f(p!=0){
					if(q==1)printf\
("%lld+",p);
					\
else pri\
ntf("%lld/%lld+",p,q);
				}
				q=abs(2*\
a);
				p=1;
	\
			ll t=0;
				for(int r=sq;r>=1;r--)
					if(delta%(r*r)\
==0)\
{
						p*=r;
						t=del\
ta/(r\
*\
r);
						\
break;
					}
				pq=gcd(abs(q),abs(p))\
;
				p/=pq\
,q/=pq;
			\
	if\
(\
q\
<0)q=-q,p=-p;
				if(p==q) printf("sqrt(%lld)\n"\
,t);
				else if(q==1) printf("%lld\
*sqrt(%lld)\n",p,t);
				else i\
f(p==1) printf("sqrt(%lld)/%lld\n",t,q);
\
				else printf(\
"%lld*sqrt(%lld)/%ll\
d\n",p,t,q)\
;
			\
}
	\
	\
}
	}
	return 0;
}
}