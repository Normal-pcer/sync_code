#include <bits/stdc++.h>
#include "./libs/debug_log.hpp"

int N;
struct Node {
    int begin, end;
    bool flag = false;
};
Node tr[10000];
std::set<int> set;
int ans = 0;
void build(int begin, int end, int p = 1) {
    std::cout << begin << ' ' << end << std::endl;
    ans++;
    tr[p].begin = begin, tr[p].end = end, tr[p].flag = true;
    auto x = (((begin + end) | 1)^ (begin + 1 == end));
    std::cout << x << std::endl;
    set.insert(x);
    if (begin + 1 == end)  return;
    auto mid = std::midpoint(begin, end);
    build(begin, mid, p << 1), build(mid, end, p << 1 | 1);
}

void build2(int l, int r, int p = 1) {
    ans++;
    tr[p].begin = l, tr[p].end = r;
    auto x = ((l + r) | (l != r)) - 1;
    set.insert(x);
    if (l == r)  return;
    auto mid = std::midpoint(l, r);
    build2(l, mid), build2(mid+1, r);
}

void build3(int begin, int end, int p = 1) {
    std::cout << begin << ' ' << end << ' ' << p << std::endl;
    ans++;
    tr[p].begin = begin, tr[p].end = end, tr[p].flag = true;
    std::cout << p << std::endl;
    set.insert(p);
    if (begin + 1 == end)  return;
    auto mid = std::midpoint(begin, end);
    build(begin, mid, mid << 1), build(mid, end, mid << 1 | 1);
}

int main() {
    int begin, end;
    std::cin >> begin >> end;
    build3(begin, end, std::midpoint(begin, end));
    printValues(ans, set.size());
    return 0;
} 