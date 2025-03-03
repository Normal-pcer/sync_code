#include <bits/stdc++.h>

char constexpr endl = '\n';
int main() {
    std::fstream fs1("1.in", std::ios::in);
    std::fstream fs2("2.in", std::ios::out | std::ios::binary);

    int T;  fs1 >> T;
    for (int i = 1; i <= T; i++) {
        // std::cout << i << std::endl;
        int N;  fs1 >> N;  fs2 << N << endl;
        for (int j = 1; j <= N; j++) {
            int a, b, c, d;  fs1 >> a >> b >> c >> d;
            fs2 << a << " " << b << " " << c << " " << d << endl;
        }
        if (i == 4516)  break;
    }
    fs1.close();
    fs2.close();
}