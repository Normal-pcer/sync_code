#include <cstdio>
#define mod 2333333

int main(int argc, char const* argv[]) {
    int n;
    long long result = 1;
    scanf("%d", &n);
    for (long long i = 2; i <= n; i++) {
        /*forall*/              result = (result + (i + 1) * i / 2) % mod;
        if (i % 2 == 0)         result = (result + i * i / 4) % mod;
        else                    result = (result + (i * i - 1) / 4) % mod;

        //// printf("%d %d %d => %d\n", (i + 1) * i / 2, i * i / 4, (i * i - 1) / 4, result);
    }

    printf("%lld\n", result);
    return 0;
}