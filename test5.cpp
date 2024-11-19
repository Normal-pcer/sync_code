/**
 * 
 */

#include "./lib_legecy.hpp"
#include "./libs/io.hpp"
using namespace lib;


namespace Solution_1165032995890248 {

    const int _N = 1e5+5;

    std::vector<int> primes;
    bool notPrime[_N];
    void pre() {
        notPrime[0] = notPrime[1] = true;
        for (int i = 2; i*i <= _N; i++) {
            if (notPrime[i])  continue;
            for (int j = i<<1; j < _N; j += i) {
                notPrime[j] = true;
            }
        }

        from(i, 2, _N-1)  if (not notPrime[i]) {
            primes.push_back(i);
        }
    }

    // 质因子数量
    int mem_count[_N];
    int count(int x, int t = 0) {
        if (mem_count[x])  return mem_count[x];
        if (x == 0 or x == 1)  return 0;
        if (not notPrime[x])  return 1;
        int ptr = t;
        while (x % primes[ptr])  ptr++;
        return (mem_count[x] = count(x / primes[ptr]) + 1);
    }

    int N;
    int a[_N];

    void solve() {
        pre();
        // io << count(1680) << endl;
        io >> N;
        from(i, 1, N)  io >> a[i];

        std::transform(a+1, a+N+1, a+1, lam(a, count(a)));
        

        auto s = std::accumulate(a+1, a+N+1, 0, lam(a, b, a^b));
        if (s)  io << "Anna" << endl;
        else  io << "Bruno" << endl;
    }
}


int main() {
    initDebug;
    Solution_1165032995890248::solve();
    return 0;
}