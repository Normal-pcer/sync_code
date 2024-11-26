#include <bits/stdc++.h>
#ifdef __linux__
#include <sys/stat.h>
#include <sys/mman.h>
#endif

#include "./libs/io.hpp"

using namespace lib;
char *ccc;
#define isd(ch) ('0'<=ch and ch<='9')
#define gc() (*ccc++)
inline void read(int &x) {
    char c = gc();
	if ( c != '-' ) for ( x = c & 15 ; ( c = gc() ) & 16 ; x = x * 10 + ( c & 15 ) );
	else    for ( x = -( gc() & 15 ) ; ( c = gc() ) & 16 ; x = x * 10 - ( c & 15 ) );
}
int main() {
    struct stat s;
    fstat(0, &s);
    ccc = (char*)mmap(nullptr, s.st_size, 1, 2, 0, 0);
    int N, x, ans=0;
    read(N);
    for (; N--; )  read(x), ans+=x;
    return 0;
}