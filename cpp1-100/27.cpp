#include <cstdio>

int memory_up[1005];
int memory[1005];

int waysCount(int steps, bool up) {
    if (steps == 1)  return up ? 3 : 2;
    if (up && memory_up[steps] != 0)  return memory_up[steps];
    if (!up && memory[steps] != 0)  return memory[steps];
    //// std::cout << steps << up << std::endl;

    int result = waysCount(steps - 1, true);
    if (up) {
        result += 2 * waysCount(steps - 1, false);
    }
    else {
        result += waysCount(steps - 1, false);
    }
    result %= 12345;
    if (!up)  memory[steps] = result;
    else  memory_up[steps] = result;

    //// printf("%d[%d]: %d\n", steps, up, result);
    return result;
}

int main(int argc, char const* argv[]) {
    int n;
    scanf("%d", &n);
    printf("%d", waysCount(n, true));
    return 0;
}