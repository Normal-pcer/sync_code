/**
 * 
 */

#include "./lib_legecy.hpp"
#include "./libs/io.hpp"
using namespace lib;

namespace Solution_1583662521665877 {

    ll N;
    int P;

    std::vector<int> primes;
    const int requiredPrime = 100;
    bool notPrime[requiredPrime];

    void preProcess() {
        notPrime[0] = notPrime[1] = true;
        for (auto i=2; i*i<requiredPrime; i++) {
            if (notPrime[i])  continue;
            for (auto j=i*i; j<requiredPrime; j+=i) {
                notPrime[j] = true;
            }
        }
        for (auto i=2; i<requiredPrime; i++) {
            if (not notPrime[i]) {
                primes.push_back(i);
            }
        }
    }

    unsigned ans;
    const int _N = 26;
    const int _MEM = 1<<17;
    unsigned mem[_N][_MEM];  // 记忆化
    void dfs1(int p, ll res) {
        // never io << std::format("dfs1 - {} {}", p, res) << endl;
        if (res < _MEM and mem[p][res])  return void(ans += mem[p][res]);
        if (primes[p] == 2) {  // 方案数量固定
            ans += std::__lg(res) + 1;
            return;
        }
        auto ans_cpy = ans;
        // 不选这个质数
        dfs1(p-1, res);
        if (res >= primes[p]) {
            dfs1(p, res/primes[p]);
        }
        if (res < _MEM) {
            mem[p][res] = ans - ans_cpy;  // 计算增量
        }
    }

    void solve() {
        preProcess();

        io >> N >> P;
        auto num = std::lower_bound(primes.begin(), primes.end(), P) - primes.begin(); 
        dfs1(num, N);

        io << ans;
    }
}

int main() {
    initDebug;
    Solution_1583662521665877::solve();
    return 0;
}