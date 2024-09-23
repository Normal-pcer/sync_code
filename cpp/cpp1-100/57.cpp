#include <bits/stdc++.h>
#define upto(i, n) for(int i=1;i<=n;i++)
#define until(condition) while(!condition)
#define DEBUG_MODE false
#define debug if(DEBUG_MODE)
#define MAX 100005

int a[MAX], N;
int sigma[MAX];

int main() {
    scanf("%d", &N);
    upto(i, N)      scanf("%d", a+i);
    upto(i, N)      sigma[i] = sigma[i-1] + a[i];

    int maxSigma = -0x80000000;
    int minSigma = 0x7FFFFFFF;
    upto(i, N) {
        debug printf("%d, ", sigma[i]);
        if (maxSigma < sigma[i])    maxSigma = sigma[i];
        else if (minSigma > sigma[i])  minSigma = std::min(minSigma, sigma[i]);
    }
    upto(i, N) {
        debug printf("%d, ", sigma[i]);
        if (maxSigma < sigma[i])    maxSigma = sigma[i];
        else if (minSigma > sigma[i])  minSigma = std::min(minSigma, sigma[i]);
    }

    printf("%d\n", maxSigma>=0?(maxSigma-minSigma):0);
    return 0;
}