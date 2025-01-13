#include <bits/stdc++.h>
using namespace std;

const int maxn = 500005;
const int inf = 1000000000;

int n, m;
int s[maxn];
deque<int> q;
int ans = -inf;

int main() {
    int n, m;
    cin >> n >> m;
    for (int i = 1; i <= n; i++) {
        cin >> s[i];
        s[i] += s[i - 1];
    }
    for (int i = 0; i <= n; i++) {
        while (!q.empty() && q.front() + m < i) q.pop_front();
        ans = max(ans, s[i] - s[q.front()]);
        while (!q.empty() && s[q.back()] > s[i]) q.pop_back();
        q.push_back(i);
    }
    cout << ans;
    return 0;
}