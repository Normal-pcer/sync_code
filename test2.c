// hook.c
#include <stdio.h>

// 覆盖普通函数
void normal_func() { 
    printf("!!! HIJACKED normal_func !!!\n"); 
}

// 尝试覆盖弱符号
void weak_func() { 
    printf("!!! Attempt to hijack weak_func !!!\n"); 
}

// 无法覆盖静态函数（不在符号表）
void static_func() { 
    printf("This won't be called\n"); 
}