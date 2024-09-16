#include <bits/stdc++.h>
#define Infinity 10

bool used[Infinity];  // 忽略其中所有的0
int stack[Infinity];
int stackDepth = 0;
int N;

//// inline std::string joinNumbers(std::string connector, int* begin, int* end) {
////     std::string result = std::to_string(*begin);
////     while (++begin != end) {
////         result += connector + std::to_string(*begin);
////     }
////     return result;
//// }

inline void batchPrint(char connector, int* begin, int* end) {
    putchar(*begin + '0');
    while (++begin != end) {
        putchar(connector);
        putchar(*begin + '0');
    }
}

void dfs() {
    for (int i = 1; i < Infinity; i++) {
        if (!used[i])  continue;
        used[i] = false;
        stack[stackDepth++] = i;

        if (stackDepth == N) {
            printf("%s\n", batchPrint(' ', stack, stack + N).c_str());
        }
        else {
            dfs();
        }
        used[stack[--stackDepth]] = true;
    }
}

int main(int argc, char const* argv[]) {
    std::cin >> N;
    for (int i = 1; i <= N; i++)  used[i] = true;
    dfs();
    return 0;
}