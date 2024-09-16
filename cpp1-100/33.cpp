#include <cstdio>
#define Infinity 11

bool used[Infinity];
int stack[Infinity];
int stackDepth = 0;
int N, M;

inline void batchPrint(char connector, int* begin, int* end_notContains) {
    printf("%d", *begin);
    while (++begin != end_notContains) {
        putchar(connector);
        printf("%d", *begin);
    }
    putchar('\n');
}

void dfs() {
    for (int i = stackDepth ? stack[stackDepth - 1] : N; i >= 1; i--) {
        if (used[i])  continue;
        stack[stackDepth++] = i;
        used[i] = true;
        if (stackDepth == M) {
            batchPrint(' ', stack, stack + stackDepth);
        }
        else {
            dfs();
        }
        used[i] = false;
        stackDepth--;
    }
}

int main(int argc, char const* argv[]) {
    freopen("33.op", "w", stdout);
    scanf("%d%d", &N, &M);
    dfs();
    return 0;
}