/**
 * 
 */

#include "lib"
#include "libs/io.hpp"
using namespace lib;


namespace Solution {

    int N, M;  const int _N = 1005;
    bool map[_N][_N];

    void init() {
        io >> N >> M;
        from(i, 1, N) {
            from(j, 1, M) {
                char ch = io.get<char>();
            }
        }
    }

    void solve() {
        init();

    }
}


int main() {
    initDebug;
    Solution::solve();
    return 0;
}