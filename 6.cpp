#include <cstdio>
#define Infinity (int)1e6+5
#define debug false

int sub_containedTimeOf[Infinity];
int length, querysCount;
int left, right;

int temp;
int getInt() {
    scanf("%d", &temp);
    return temp;
}

void batchModify(int begin_contains, int end_contains, int amount) {
    sub_containedTimeOf[begin_contains] += amount;
    sub_containedTimeOf[end_contains + 1] -= amount;
}

int main(int argc, char const* argv[]) {
    length = getInt();
    querysCount = getInt();
    //// for (int pointIndex = 1; pointIndex <= length; pointIndex++) {

    //// }

    for (int queryIndex = 1; queryIndex <= querysCount; queryIndex++) {
        left = getInt(); right = getInt();
        batchModify(left, right, -1);
    }

    int currentValue = 1;
    int count = 0;
    for (int pointIndex = 0; pointIndex <= length; pointIndex++) {
        currentValue += sub_containedTimeOf[pointIndex];
        if (currentValue >= 1) {
            count++;
#if debug
        printf("Available point: %d\n", pointIndex);
#endif
        }
    }

    printf("%d\n", count);
    return 0;
}