#include "lib"

int N, V;
const int maxN=102, maxV=10005;
int w[maxN], v[maxN];
int f[maxN][maxV];

int main() {
    initDebug;

    std::cin >> N >> V;
    upto(i, N) {
        w[i] = read(); v[i] = read();
    }

    upto(i, N) {
        from(j, w[i], V) {
            if (j>=w[i])  f[i][j] = std::max(f[i-1][j], f[i][j-w[i]]+v[i]);
        }
    }

    std::cout << f[N][V] << std::endl;
    return 0;
}