#include <cstdio>
#define Infinity 1000005
#define debug false

int N;
//// int inputQueue[Infinity];
long long inputQueueSigma[Infinity];
long long contributionAmountOf[Infinity];
int lastBelieverIdOf[Infinity];
int temp;

int getInt() {
    scanf("%d", &temp);
    return temp;
}

long long getSumBetween(int left_contains, int right_contains) {
    return inputQueueSigma[right_contains] - inputQueueSigma[left_contains - 1];
}

int main(int argc, char const* argv[]) {
    scanf("%d", &N);

    //// for (int i = 1; i <= N; i++)  inputQueueSigma[i] = inputQueueSigma[i - 1] + inputQueue[i];

    for (int personId = 1; personId <= N; personId++) {
        int team = getInt();
#if debug
        printf("^%d\n", personId);
#endif
        inputQueueSigma[personId] = inputQueueSigma[personId - 1] + team;
        if (lastBelieverIdOf[team] != 0) {
            // With an available record
            contributionAmountOf[team] += getSumBetween(lastBelieverIdOf[team] + 1, personId - 1);
#if debug
            printf("[%d] += sumBet(%d, %d) = %d\n", team, lastBelieverIdOf[team] + 1, personId - 1,
                getSumBetween(lastBelieverIdOf[team] + 1, personId - 1));
#endif
        }
        lastBelieverIdOf[team] = personId;
    }

    bool isOutputNone = true;
    for (int team = 1; team < Infinity; team++) {
        if (contributionAmountOf[team] != 0) {
            printf("%d %lld\n", team, contributionAmountOf[team]);
            isOutputNone = false;
        }
    }

    if (isOutputNone)
        printf("NONE\n");

    return 0;
}