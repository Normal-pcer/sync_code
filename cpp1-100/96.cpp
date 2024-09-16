#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=n;i++)
#define from(i,b,e) for(int i=b;i<e;i++)
bool DEBUG_MODE=false;

std::vector<int> primes;
std::bitset<(int)1e8+8> numbers;
int N, Q;

int main(int argc, char const *argv[]) {
    initDebug;
    std::ios::sync_with_stdio(false); std::cin.tie(0); std::cout.tie(0);
    std::cin >> N >> Q;
    from(p, 2, N) {
        if (!numbers[p])    primes.push_back(p);
        for (auto q=primes.begin(); q!=primes.end() && (*q)*p<=N; q++) {
            numbers[(*q) * p] = true;
            if (p % (*q) == 0)  break;
        }
    }
    int temp;
    upto(i, Q) {
        std::cin >> temp;
        std::cout << primes[temp-1] << std::endl;
    }
    return 0;
}