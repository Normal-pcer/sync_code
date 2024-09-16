#include <bits/stdc++.h>
#define Infinity 100005
using namespace std;

int N, M, a[Infinity], OP, num1, num2;
int S[Infinity];  // S_i = i^Sigma(a_i)_j=1

void calculateS() {
    S[0] = a[0];
    for (int i = 1; i <= N; i++) {
        S[i] = S[i - 1] + a[i];
    }

    // for (int i=1; i<=N; i++) cout << S[i] << " ";
}

int sum(int left, int right) {
    return S[right] - S[left - 1];
}

int main(int argc, char const* argv[]) {
    cin >> N;
    for (int i = 1; i <= N; i++) { scanf("%d", a + i); }
    calculateS();
    cin >> M;
    for (int i = 1; i <= M; i++) {
        scanf("%d %d %d", &OP, &num1, &num2);
        if (OP == 1) {
            // Modify
            a[num1] = num2;
            calculateS();
        }
        else {
            // Query
            printf("%d\n", sum(num1, num2));
        }
    }
    return 0;
}