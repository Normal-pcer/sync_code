// lib.c
#include <stdio.h>

void normal_func() { printf("Original normal\n"); }
inline void weak_func() { printf("Original weak\n"); }
static void static_func() { printf("Original static\n"); }

void call_functions() {
    printf("Calling functions:\n");
    normal_func();
    weak_func();
    static_func();
}