#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define from(i,b,e) for(int i=(b);i<=(e);i++)
#define rev(i,e,b) for(int i=(e);i>=(b);i--)

#define chkMax(base,cmp...) (base=std::max({(base),##cmp}))
#define chkMin(base,cmp...) (base=std::min({(base),##cmp}))
#define never if constexpr(0)
#define always if constexpr(1)
const int inf = 0x3f3f3f3f;  const long long infLL = 0x3f3f3f3f3f3f3f3fLL;
bool DEBUG_MODE=false;
typedef long long ll; typedef unsigned long long ull;

#define __macro_arg_counter(_1,_2,_3,_4,_5, N, ...) N
#define macro_arg_counter(...)  __macro_arg_counter(__VA_ARGS__,5,4,3,2,1,0)
#define __macro_choose_helper(M,count)  M##count
#define macro_choose_helper(M,count)   __macro_choose_helper(M,count)
#define __lambda_1(expr) [&](){return expr;}
#define __lambda_2(a, expr) [&](auto a){return expr;}
#define __lambda_3(a, b, expr) [&](auto a, auto b){return expr;}
#define __lambda_4(a, b, c, expr) [&](auto a, auto b, auto c){return expr;}
#define lambda(args...) macro_choose_helper(__lambda_, macro_arg_counter(args))(args)
#define lam lambda
namespace lib{}

#include "./libs/io.hpp"
using namespace lib;

namespace Solution_9106446611119119 {

    int A, B;
    int a[10005];
    void solve() {
        io >> A >> B;
        from(i, 1, A)  a[i] = 1;
        bool flag = true;
        while (flag) {
            flag = false;
            std::sort(a+1, a+1+A);
            from(i, 1, A-1) {
                if (a[i] == a[i+1] and a[i] != 0 and a[i] < B) {
                    a[i]--, a[i+1]++;
                    flag = true;
                    break;
                }  
            }
        }
        from(i, 1, A)  io << a[i] << ' ';
        io << endl;
    }
}

std::mt19937 rnd(745184);

int main(int argc, char const *argv[]) {
    initDebug;
    Solution_9106446611119119::solve();
    return 0;
}