#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) {
        long long a, b, k, l;
        cin >> a >> b >> k >> l;

        if (l == 0) {
            cout << a + b << '\n';
            continue;
        }

        long long x_m1 = a / l;
        long long y_m1 = b / l;

        long long base = min(k, x_m1 + y_m1);
        long long base_reduction = base * l;
        long long remaining_k = k - base;

        long long x_remainder = a % l;
        long long y_remainder = b % l;

        bool x_extra = (x_remainder * 2) > l;
        bool y_extra = (y_remainder * 2) > l;

        vector<long long> candidates;
        if (x_extra) {
            candidates.push_back(2 * x_remainder - l);
        }
        if (y_extra) {
            candidates.push_back(2 * y_remainder - l);
        }

        sort(candidates.rbegin(), candidates.rend());

        long long take = min(remaining_k, (long long)candidates.size());
        long long extra_reduction = 0;
        for (long long i = 0; i < take; ++i) {
            extra_reduction += candidates[i];
        }

        long long total_reduction = base_reduction + extra_reduction;
        long long time = max(a + b - total_reduction, 0LL);

        cout << time << '\n';
    }

    return 0;
}