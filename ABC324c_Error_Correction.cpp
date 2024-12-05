/**
 * @link https://atcoder.jp/contests/abc324/tasks/abc324_c
 */

#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=(n);i++)
#define from(i,b,e) for(int i=(b);i<=(e);i++)
#define rev(i,e,b) for(int i=(e);i>=(b);i--)

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

;
namespace Solution {
    const int _N = 5e5+5;
    int N;
    std::string T;
    std::string s[_N];
    std::list<int> ans;
    int ansCount = 0;
    void solve() {
        // Your code here...
        std::cin >> N;
        std::cin >> T;
        upto(i, N) {
            std::cin >> s[i];
        }

        // std::sort(s+1, s+N+1);

        upto(i, N) {
            if (s[i] == T) {
                ans.push_back(i);
                ansCount++;
            } else if (abs( (int)(s[i].length() - T.length()) ) > 1) {
                continue;
            } else if (s[i].length() == T.length()) {
                int able = 2;
                for (int j = 0; j < s[i].length(); j++) {
                    if (s[i][j] != T[j]) able--;
                }
                if (able == 1) {
                    ans.push_back(i);
                    ansCount++;
                }
            }
            else {
                std::string a, b;
                if (s[i].length() > T.length()) {
                    a = s[i];
                    b = T;
                } else {
                    a = T;
                    b = s[i];
                }

                int modified = 0;
                for (int j = 0, k = 0; j < a.length(); j++, k++) {
                    if (a[j] != b[k]) {
                        modified++;
                        k--;
                        if (modified > 1) break;
                    }
                }
                if (modified == 1) {
                    ans.push_back(i);
                    ansCount++;
                }
            }
            
        }
        std::cout << ansCount << std::endl;
        for (auto it = ans.begin(); it != ans.end(); it++) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
    }
}

int main(int argc, char const *argv[]) {

    initDebug;
    optimizeIO;
    Solution::solve();
    return 0;
}