#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=n;i++)
bool DEBUG_MODE=false;

inline long long A(int m, int n) {
    long long result=1;
    for (int i=0; i<m; i++)     result *= n-i;
    return result;
}

inline long long C(int m, int n) {
    if (m+m>n)  return C(n-m, n);
    return A(m, n) / A(m, m);
}

int M, N, T;

int main(int argc, char const *argv[]) {
    initDebug;
    std::cin >> T;
    upto(t, T) {
        std::cin >> M >> N;
        long long result = 1;
        upto(_, M)  result *= N;
        result /= A(N, N);
        result /= A(M, M);
        std::cout << result << std::endl;
    }
    return 0;
}