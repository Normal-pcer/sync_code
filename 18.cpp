#include <bits/stdc++.h>
#define Infinity 10

bool isNumberAvailable[Infinity];  // 忽略其中所有的0
int stack[Infinity];
int stack_depth = 0;
int N;

inline std::string joinNumbers(std::string connector, int* begin, int* end) {
    std::string result = std::to_string(*begin);
    while (++begin != end) {
        result += connector + std::to_string(*begin);
    }
    return result;
}

void generateArray() {
    for (int i = 1; i < Infinity; i++) {
        if (!isNumberAvailable[i])  continue;
        isNumberAvailable[i] = false;
        stack[stack_depth++] = i;

        if (stack_depth == N) {
            printf("%s\n", joinNumbers(" ", stack, stack + N).c_str());
        }
        else {
            generateArray();
        }
        isNumberAvailable[ stack[--stack_depth] ] = true;
    }
}

int main(int argc, char const* argv[]) {
    std::cin >> N;
    for (int i=1; i<=N; i++)  isNumberAvailable[i] = true;
    generateArray();
    return 0;
}