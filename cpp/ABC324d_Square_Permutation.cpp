/**
 * @link https://atcoder.jp/contests/abc324/tasks/abc324_d
 */

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




/*
Problem D: Square Permutation
Difficulty: 895   Time Limit: 4s   Memory Limit: 1024MB
已知一个长度为 N 的数字串 S，可以对这 N 个字符进行任意顺序排列
请问，在这 N! 种排列中，组成的新数字恰好是一个完全平方数的方案数是多少
其中 1 <= N <= 13; 数据保证字符串S中只包含数字;
*/

namespace Solution {
    int N;
    std::string S;

    int buc1[12];
    int buc2[12];
    ll cnt;

    void count(std::string S, int buc[12]) {
        std::memset(buc, 0, 4*12);
        for (auto c: S) buc[c-'0']++;
    }

    bool same() {
        return std::equal(buc1, buc1+10, buc2);
    }
    
    void solve() {
        std::cin >> N >> S;
        count(S, buc1);

        std::string S_max = std::string(S);
        std::sort(S_max.begin(), S_max.end(), std::greater<char>());
        ll maxSquare = std::stoll(S_max);

        for (ll i=0; i*i <= maxSquare; i++) {
            std::string S_i = std::to_string(i*i);
            if ((int)S_i.size()<N) {
                // 补 0
                S_i.reserve(N);
                while ((int)S_i.size()<N) S_i = '0' + S_i;
            }
            count(S_i, buc2);
            if (same()) {
                cnt++;
            }
        }

        std::cout << cnt << std::endl;
    }
}


int main() {
    initDebug;
    Solution::solve();
    return 0;
}