#include <iostream>
#include <cstdio>
#include <vector>
#include <queue>
#include <cstring>
#include <algorithm>
#define Infinity (int)1e5+5

std::vector<int> linksFromNode[Infinity];
bool isNodeWalked[Infinity];
int nodesCount, connectionsCount;


void searchNode_dfs(int node, bool first) {
    if (isNodeWalked[node])  return;
    isNodeWalked[node] = true;
    if (!first) printf(" ");
    printf("%d", node);
    for (auto head : linksFromNode[node]) {
        searchNode_dfs(head, false);
    }
    //isNodeWalked[node] = false;
}

void searchNode_bfs(int outset) {
    std::queue<int> nodeQueue;
    nodeQueue.push(outset);
    int outputCount = 0;
    while (!nodeQueue.empty()) {
        int thisNode = nodeQueue.front();
        nodeQueue.pop();
        if (isNodeWalked[thisNode]) continue;
        for (auto head : linksFromNode[thisNode]) {
            nodeQueue.push(head);
        }
        if (outputCount++)  printf(" ");
        printf("%d", thisNode);
        isNodeWalked[thisNode] = true;
    }
}

int main(int argc, char const* argv[]) {
    scanf("%d%d", &nodesCount, &connectionsCount);
    int connectTail, connectHead;
    for (int connectionIndex = 1; connectionIndex <= connectionsCount; connectionIndex++) {
        scanf("%d%d", &connectTail, &connectHead);
        linksFromNode[connectTail].push_back(connectHead);
    }
    for (int nodeIndex = 1; nodeIndex <= nodesCount; nodeIndex++) {
        std::sort(linksFromNode[nodeIndex].begin(), linksFromNode[nodeIndex].end());
    }
    searchNode_dfs(1, true);
    printf("\n");
    std::fill(isNodeWalked, isNodeWalked+sizeof(isNodeWalked), false);
    searchNode_bfs(1);

    return 0;
}