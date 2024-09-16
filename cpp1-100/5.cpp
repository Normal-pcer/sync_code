#include <cstdio>
#define Infinity (int)1e6+5

int personsCount;
int sub_timesOfPerson[Infinity];
int lastPersonIdOfTeam[Infinity];

int temp;
int getInt() {
    scanf("%d", &temp);
    return temp;
}

void batchModify(int begin_contains, int end_contains, int amount) {
    sub_timesOfPerson[begin_contains] += amount;
    sub_timesOfPerson[end_contains + 1] -= amount;
}

int main(int argc, char const *argv[]) {
    personsCount = getInt();
    for (int personId = 1; personId <= personsCount; personId++) {
        int thisTeam = getInt();
        if (lastPersonIdOfTeam[thisTeam]) {
            batchModify(lastPersonIdOfTeam[thisTeam], personId, 1);
        }
        lastPersonIdOfTeam[thisTeam] = personId;
    }
    
    int maxTime = -1;
    int maxOwner = -1;
    int currentValue = 0;
    for (int personId = 1; personId <= personsCount; personId++) {
        currentValue += sub_timesOfPerson[personId];
        if (currentValue > maxTime) {
            maxTime = currentValue;
            maxOwner = personId;
        }
    }
    printf("%d %d\n", maxOwner, maxTime);
    return 0;
}