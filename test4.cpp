#include <iostream>

template <typename T>
void print(T const t)
{
	T x = t;
	if(x < 0) x = -x;
	if(x > 9)  print(x / 10);
	putchar(x % 10 + '0');
}

template <typename T = char>
void print(const char x)
{
	putchar(x);
}

template<typename T, typename... Args>
void print(T x, Args... args)
{
	print<T>(x);
	print(args...);
}

int main()
{
	print((int)-2147483648);
}