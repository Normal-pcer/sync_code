#include <bits/stdc++.h>
namespace lib {
    namespace bit {
        // 按位遍历所有的 1
        template <class T, class Func>
        inline void traverse1(T x, Func&& f) {  
            for (; x; x&=x-1)  f(x&-x);
        }
        // 按位大于
        template <class T>
        inline bool greater(T x, T y) {  
            return x & ~y;
        }
        // 枚举子集
        template <class T, class Func>
        inline void subset(T x, Func&& F) {  
            for (status t=s; t; t=(t-1)&s)  f(t);
        }
    }
}