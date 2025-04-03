#include<iostream>
using namespace std;

int main() {
    int t;
    cin >> t;
    for (int i = 0; i < t; i = i + 1) {    
        int n, m;
        cin >> n >> m;
        if (n > m) {
            int tmp = n;
            n = m;
            m = tmp;
        }

        if (n == 1) {
            cout << m / 2 + 1 << endl;
        } else {
            cout << (m + 1) / 2 + (n + 1) / 2 << endl;
        }
    }
    return 0;
}