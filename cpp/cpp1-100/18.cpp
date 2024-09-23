#include <bits/stdc++.h>
#define Infinity 10

//// bool isNumberAvailable[Infinity];
std::vector<int> availableNumbers;
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
    for (int i = 0; i < availableNumbers.size(); i++) {
        for (int j: availableNumbers)  printf("%d ", j);
        printf("\n");
        for (int j=0; j<=stack_depth; j++)  printf("%d ", stack[j]);
        printf("\n");
        availableNumbers.erase(availableNumbers.begin() + i);
        stack[stack_depth++] = availableNumbers[i];

        if (stack_depth == N) {
            printf("%s\n", joinNumbers(" ", stack, stack + N).c_str());
        }
        else {
            generateArray();
        }
        availableNumbers.insert(availableNumbers.begin() + i, stack[--stack_depth]);
    }
}

int main(int argc, char const* argv[]) {
    std::cin >> N;
    for (int i = 1; i <= N; i++) {
        availableNumbers.push_back(i);
    }
    generateArray();
    return 0;
}