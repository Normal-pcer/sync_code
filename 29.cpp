#include <cstdio>

int dp[2][30];
int personsCount = 1;
int timeLimit;


inline int getInt() {
    int temp;
    scanf("%d", &temp);
    return temp;
}

inline int getItem(int index1, int index2) {
    return dp[index1][(index2 + personsCount) % personsCount];
}

int main(int argc, char const* argv[]) {
    personsCount = getInt();
    timeLimit = getInt();

    dp[0][0] = 1;
    for (int currentTime = 1; currentTime <= timeLimit; currentTime++) {
        for (int personId = 0; personId < personsCount; personId++) {
            dp[1][personId] = getItem(0, personId - 1) + getItem(0, personId + 1);
            ////printf("%d ", getItem(1, personId));
        }
        ////printf("\n");
        for (int personId = 0; personId < personsCount; personId++) {
            dp[0][personId] = getItem(1, personId);
            ////printf("%d ", dp[0][personId]);
        }
        ////printf("\n");
    }

    printf("%d", dp[1][0]);
    return 0;
}