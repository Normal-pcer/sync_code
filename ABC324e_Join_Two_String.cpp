/**
 * @link https://atcoder.jp/contests/abc324/tasks/abc324_e
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
Problem E: Joint Two Strings
Difficulty: 1153   Time Limit: 2s   Memory Limit: 1024MB
给定一个目标字符串 T，并且给出 N 个字符串 S_i
请问有多少对 (i, j) 组合，可以使得 T 是 S_i + S_j 的子序列，i 可以等于 j
其中 1 <= N <= 5e5; 1 <= |T|, |S_i| <= 5e5;
\sum { |S_i| } <= 5e5; 数据保证所有的字符串只包含小写英文字母;
*/
namespace Solution {
    const int _N = 5e5+5;
    int N;
    std::string T;

    std::string s[_N];

    struct p {
        int id, len;
        bool operator<(const p& other) const {
            return this->len < other.id;
        }
    };

    int mp[_N], ms[_N];
    ll ans = 0;

    void solve() {
        std::cin >> N >> T;
        upto(i, N) {
            std::string s;
            std::cin >> s;

            // mp[i] -  T 连续的前缀 匹配 s 的非连续子序列
            from(j, 0, (int)s.length()-1) {
                if (mp[i]<(int)T.size() && s[j]==T[mp[i]])   mp[i]++;
            }

            // ms[i] -  T 连续的后缀 匹配 s 的非连续子序列
            rev(j, (int)s.length()-1, 0) {
                if ((int)T.length()-ms[i]-1>=0 && s[j]==T[(int)T.length()-ms[i]-1])   ms[i]++;
            }
        }

        never {
            upto(i, N)  std::cout << mp[i] << ' ';
            std::cout << std::endl;
            upto(i, N)  std::cout << ms[i] << ' ';
            std::cout << std::endl;
        }

        // 考虑连接字符串 s[i] 和 s[j]
        // 用 s[i] 匹配 T 的一段连续前缀，s[j] 匹配 T 的一段连续后缀
        // 那么需要满足 mp[i] + ms[j] >= T.length()
        // 先排序再二分查找最小的可行的 j，比它大的 j 都可以
        // 复杂度：O(N logN)
        std::sort(ms+1, ms+N+1);

        upto(i, N) {
            // ms[j] >= T.length() - mp[i]
            auto ptr = std::lower_bound(ms+1, ms+N+1, T.length()-mp[i]);
            int cnt = ms+N+1 - ptr;
            ans += cnt;
        }

        std::cout << ans << std::endl;
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    Solution::solve();
    return 0;
}