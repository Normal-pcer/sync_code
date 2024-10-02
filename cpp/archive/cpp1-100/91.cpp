#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=n;i++)
bool DEBUG_MODE=false;

std::string s;
int count = 0;

int main(int argc, char const *argv[]) {
    initDebug;
    std::ios::sync_with_stdio(false); std::cin.tie(0); std::cin.tie(0);
    std::cin >> s;
    for (int i=0; i<(int)s.size(); i++) {
        for (int j=i; j<(int)s.size(); j++){
            if (s[i] == s[j])   count++;
        }
    }
    std::cout << count << std::endl;
    return 0;
}
