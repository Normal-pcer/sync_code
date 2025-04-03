#include <bits/stdc++.h>
#include <windows.h>

int main() {
    if (mciSendStringA("set CDAudio door open", NULL, 0, NULL) != 0) {
        std::cerr << "无法打开光驱" << std::endl;
    }
}
