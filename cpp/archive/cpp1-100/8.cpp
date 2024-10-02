#include <cstdio>
#include <algorithm>
#define Infinity (int)1e6+5
#define debug false

int personsCount;
int teamOf[Infinity];
long long leastSum;
int leftPointer = 1;
long long sumBetween = 0;
int minLength = Infinity;

int temp;
int getInt() {
    scanf("%d", &temp);
    return temp;
}

int main(int argc, char const *argv[]) {
    personsCount = getInt();
    scanf("%lld", &leastSum);

    for (int personId = 1; personId <= personsCount; personId++) {
        teamOf[personId] = getInt();
        sumBetween += teamOf[personId];
        do {
            sumBetween -= teamOf[leftPointer++];
        } while (sumBetween >= leastSum);
        sumBetween += teamOf[--leftPointer];
        if (sumBetween >= leastSum)  minLength = std::min(minLength, personId - leftPointer + 1);
#if debug
        printf("%d %d\n", minLength, personId - leftPointer + 1);
#endif
    }

    if (minLength == Infinity)  printf("NONE");
    else  printf("%d\n", minLength);

    return 0;
}

/*
10 9
3
2
5
4
2
6
1
3
5
2
*/