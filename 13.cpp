#include <bits/stdc++.h>
#define mod 12345

int memory[1010];

int getLinesOfArc(int length_notContains) {
    //// printf("[%d]", length_notContains);
    if (length_notContains <= 1)  return 1;
    if (memory[length_notContains])  return memory[length_notContains];
    int result = 0;
    for (int begin = 1; 2 * begin <= length_notContains; begin++) {
        for (int end = begin + 1; end <= length_notContains; end++) {
            result += (getLinesOfArc(end - begin - 1) * getLinesOfArc(length_notContains - end + begin - 1) * 2) % mod;
            printf("[%d](%d, %d): %d\n", length_notContains, begin, end, getLinesOfArc(end - begin - 1) * getLinesOfArc(length_notContains - end + begin - 1) * 2);
        }
    }
    memory[length_notContains] = result;
    printf("[%d]: %d\n\n", length_notContains, result);
    return result;
}

int main(int argc, char const* argv[]) {
    getLinesOfArc(4);
    // std::cout << getLinesOfArc(5) << std::endl;
    return 0;
}