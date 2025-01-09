with open("test.cpp", "w", encoding="UTF-8") as f:
    f.write("""#include <bits/stdc++.h>
int main() {
int x;  std::cin >> x;
if (x == 0)  std::cout << "x = 0" << std::endl;
""")
    for i in range(1000000):
        f.write('else if (x == {})  std::cout << "x = {}" << std::endl;\n'.format(i, i))

    f.write("}\n")