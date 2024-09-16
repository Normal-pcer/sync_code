#include <cstdio>
#define Infinity 114514

int dataCount;
int operationsCount;

int amountSubList[Infinity];

int getAmountAt(int index) {
    int result = amountSubList[0];
    for (int i = 1; i <= index; i++) {
        result += amountSubList[i];
    }
    //// printf("Amount At [%d]: %d", index, result);
    return result;
}

int temp;

int getInt() {
    scanf("%d", &temp);
    return temp;
}

int main(int argc, char const* argv[]) {
    dataCount = getInt();
    int lastData = 0;
    for (int dataIndex = 1; dataIndex <= dataCount; dataIndex++) {
        int thisData = getInt();
        amountSubList[dataIndex] = thisData - lastData;
        lastData = thisData;
    }
    operationsCount = getInt();
    int operationType;
    for (int operationIndex = 1; operationIndex <= operationsCount; operationIndex++) {
        scanf("%d", &operationType);
        //// printf(": ");
        //// for (int index = 1; index <= dataCount; index++)
        ////     printf("%d, ", getAmountAt(index));
        //// printf("\n");
        if (operationType == 1) {
            // Batch Modify
            int left = getInt();
            int right = getInt();
            int addAmount = getInt();

            amountSubList[left] += addAmount;
            amountSubList[right + 1] -= addAmount;
        }
        else {
            // Query Sum
            int left = getInt();
            int right = getInt();

            int thisAmount = getAmountAt(left);
            long long result = thisAmount;
            for (int index = left + 1; index <= right; index++) {
                thisAmount += amountSubList[index];
                result += thisAmount;
                ////printf("->%d", result);
            }

            printf("%lld\n", result);
        }
    }

    return 0;
}
