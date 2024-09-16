#include <bits/stdc++.h>
#define mod 12345

int memory[1010];

inline std::string addSpace(int count, std::string base) {
    std::string result = base;
    for (int i = 1; i <= count; i++)  result = "  " + result;
    return (result);
}

int getLinesOfArc(int length_notContains, int depth) {
    //// printf("[%d]", length_notContains);
    if (length_notContains <= 1)  return 1;
    if (memory[length_notContains])  return memory[length_notContains];
    int result = 0;
    printf(addSpace(depth, "[%d] start\n").c_str(), length_notContains);
    for (int begin = 1; 2 * begin <= length_notContains; begin++) {
        for (int end = begin + 1; end <= length_notContains; end++) {
            result += (getLinesOfArc(end - begin - 1, depth + 1) * getLinesOfArc(length_notContains - end + begin - 1, depth + 1) * 2) % mod;

            printf(addSpace(depth, "[%d](%d, %d): %d\n").c_str(), length_notContains, begin, end,
                getLinesOfArc(end - begin - 1, depth + 1) * getLinesOfArc(length_notContains - end + begin - 1, depth + 1) * 2);
        }
    }
    memory[length_notContains] = result;
    printf(addSpace(depth, "[%d]: %d\n").c_str(), length_notContains, result);
    return result;
}

inline int getLinesOfArc(int length_notContains) {
    return getLinesOfArc(length_notContains, 0);
}

int main(int argc, char const* argv[]) {
    getLinesOfArc(3);
    // std::cout << getLinesOfArc(5) << std::endl;
    return 0;
}