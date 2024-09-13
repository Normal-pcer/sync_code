#include <bits/stdc++.h>
bool debug = false;
int main(int argc, char const *argv[]) {
    debug = (argc-1) && !strcmp("-d", argv[1]);
    if (debug)  printf("debug\n");
    return 0;
}