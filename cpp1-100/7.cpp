#include <bits/stdc++.h>
#define Infinity (int)1e6+5
#define debug false

int personsCount;
int teamOf[Infinity];
bool containedTeam[Infinity];
int leftPointer = 1;
int maxZoneLength = 0;

int temp;
int getInt() {
    scanf("%d", &temp);
    return temp;
}

int main(int argc, char const* argv[]) {
    personsCount = getInt();
    for (int personId = 1; personId <= personsCount; personId++) {
        teamOf[personId] = getInt();
        if (personId == leftPointer) {
            containedTeam[teamOf[personId]] = true;
            continue;
        }
        while (containedTeam[teamOf[personId]]) {
            containedTeam[teamOf[leftPointer]] = false;
            leftPointer++;
        }
        containedTeam[teamOf[personId]] = true;
        maxZoneLength = std::max(maxZoneLength, personId - leftPointer);
#if debug
        printf("%d %d\n", leftPointer, personId);
#endif
    }
    printf("%d\n", maxZoneLength+1);
    return 0;
}

/*
Sample:
10
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