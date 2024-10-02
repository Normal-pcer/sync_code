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




const int _N = 505; int N;
const int mod = 1e9+7;
std::string s;
ll F[_N][_N];  // 区间 [i, j] 的括号匹配数

inline int match(char i, char j) {
    // 特别地，"?" 可以匹配任意字符
    if (i == '?')  return match('(', j) + match('[', j) + match('{', j);
    if (j == '?')  return match(i, ')') + match(i, ']') + match(i, '}');

    return (i == '(' && j == ')') + (i == '[' && j == ']') + (i == '{' && j == '}');
}

int main() {
    initDebug;


    std::cin >> N;
    s.reserve(N);
    std::cin >> s;
    s = " "+s;
    assert((int)s.size() == N+1);

    from(l, 2, N) {  // 区间长度
        from(i, 1, N) {  // 区间左端点
            int j = i+l-1;  // 区间右端点
            if (j > N) break;

            // 合并两个区间
            from(k, i, j) {  // 两个区间分别为 [i, k], [k+1, j]
                F[i][j] += F[i][k] * F[k+1][j];
                F[i][j] %= mod;
            }

            // 如果外层是匹配的
            if (match(s[i], s[j])) {
                F[i][j] += ((j-i!=1)? F[i+1][j-1]: 1) * match(s[i], s[j]);
                F[i][j] %= mod;
            }
        }
    }

    debug batchOutput2d(F, N+1, N+1, "%5lld");

    std::cout << F[1][N] << std::endl;
    return 0;
}