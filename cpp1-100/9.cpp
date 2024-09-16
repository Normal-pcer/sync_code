#include <cstdio>
#include <algorithm>
#define Infinity (int)1e6+5
#define debug false

int personsCount, targetTeamKinds;
int leftPointer = 1, rightPointer = 1;
int teamOf[Infinity];
int membersInZoneOf[Infinity];
int teamKindsInZone = 0;
int minLength = Infinity;

int temp;
int getInt() {
    scanf("%d", &temp);
    return temp;
}

int main(int argc, char const* argv[]) {
    personsCount = getInt();
    targetTeamKinds = getInt();
    for (;rightPointer <= personsCount; rightPointer++) {
        int team = getInt();
        teamOf[rightPointer] = team;
        if (!membersInZoneOf[team])  teamKindsInZone++;
        membersInZoneOf[team]++;

        while (teamKindsInZone >= targetTeamKinds) {
#if debug
            printf("%d %d\n", leftPointer, rightPointer);
            for (int i = 0; i <= 10; i++)  printf("[%d]%d ", i, membersInZoneOf[i]);
            printf("\n%d\n", teamKindsInZone);
#endif
            if (teamKindsInZone <= targetTeamKinds && membersInZoneOf[teamOf[leftPointer]] <= 1 || leftPointer >= rightPointer)  break;
            membersInZoneOf[teamOf[leftPointer]]--;
            if (!membersInZoneOf[teamOf[leftPointer]])  teamKindsInZone--;
            leftPointer++;
        }

        if (teamKindsInZone >= targetTeamKinds)  minLength = std::min(minLength, rightPointer - leftPointer + 1);

    }

    if (Infinity == minLength) printf("NONE");
    else printf("%d\n", minLength);
    return 0;
}

/*
10 6
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