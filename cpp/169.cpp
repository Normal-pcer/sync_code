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

#define sqr(x) ((x)*(x))
#define min(a,b) ((a)<(b)?(a):(b))
#define dp F
const int L = 50;
struct Point
{
	double x, y;
	void Read()
	{
		scanf("%lf%lf", &x, &y);
	}
	void operator -= (const Point &s)
	{
		x -= s.x;
		y -= s.y;
	}
	double operator - (const Point &s) const
	{
		return sqr(x-s.x)+sqr(y-s.y);
	}
	bool operator < (const Point &s) const
	{
		return atan2(y, x) < atan2(s.y, s.x);
	}
}P[L], tmp;
double area(int i, int j, int k)
{
	double a = sqrt(P[i]-P[j]);
	double b = sqrt(P[j]-P[k]);
	double c = sqrt(P[k]-P[i]);
	double p = (a+b+c)/2;
	return p*(p-a)*(p-b)*(p-c);
}
double F[L][L]={}, T;
void displayDp(int N) {
    from(i, 0, N) {
        from(j, 0, N) {
            if (dp[i][j] > 1000000000)  printf("        INF");
            else if (dp[i][j] < -1000000000)  printf("       -INF");
            else  printf("%11.0lf", dp[i][j]);
        }
        printf("\n");
    }
}
int main()
{
    initDebug;
    freopen("168.in", "r", stdin);
    freopen("168.out", "w", stdout);
	int N, i, j, k;
	const double inf = 1e100;
	double Area[L][L][L]={}, S = 0;
	scanf("%d", &N);
	for(i=0; i<N; ++i)
		P[i].Read();

	// for(i=1; i<N; ++i)
	// 	if(P[i].y < tmp.y)
	// 		tmp = P[i];

    double averageX=0.0, averageY=0.0;
    from(i, 0, N-1) {
        averageX += P[i].x;
        averageY += P[i].y;
    }
    
    averageX /= (double)N; averageY /= (double)N;
    Point base = {averageX, averageY};
    from(i, 0, N)  P[i] -= base;
	std::sort(P, P+N);
	for(i=0; i+2<N; ++i)
	{
		for(j=i+1; j+1<N; ++j)
			for(k=j+1; k<N; ++k)
				Area[i][j][k] = area(i, j, k);
		S += sqrt(Area[0][i+1][i+2]);
	}
    from(i, 0, N-1) {
    from(j, 0, N-1) {
        from(k, 0, N-1)
            printf("%11.1lf", Area[i][j][k]);
        printf("\n");
    } 
    printf("\n=====\n");
    }
	for(j=2; j<N; ++j)
		for(i=0; i+j<N; ++i)
		{
			F[i][i+j] = inf;
			for(k=1; k<j; ++k)
			{
				T = F[i][i+k]+F[i+k][i+j]+Area[i][i+k][i+j];
				F[i][i+j] = min(F[i][i+j], T);
			}
		}
    debug displayDp(N);
	printf("%.2f", sqrt(fabs(F[0][N-1]-sqr(S)/(N-2))/(N-2)));
}