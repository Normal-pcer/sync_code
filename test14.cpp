#include <fstream>
#include <iostream>
#include <string>
using namespace std;
int main() {
    unsigned char s[258], c = 0;
    string kyf, inf, ouf;
    cout << "input file: ";
    getline(cin, inf);
    cout << "output file: ";
    getline(cin, ouf);
    cout << "key file: ";
    getline(cin, kyf);
    ifstream ifile(inf, ios::binary), kfile(kyf, ios::binary);
    ofstream ofile(ouf, ios::binary);
    string text = string(istreambuf_iterator<char>(ifile), istreambuf_iterator<char>());
    string key = string(istreambuf_iterator<char>(kfile), istreambuf_iterator<char>());
    if (!key.size()) cout << "empty key", exit(1);
    for (int i = 0; i < 258; i++)
        s[i] = i > 0xff ? 0 : i;
    for (int i = 0; i < 256; i++)
        swap(s[i], s[c = s[i] + c + key[i % key.size()]]);
    for (auto &ch : text)
        s[257] += s[++s[256]], swap(s[s[256]], s[s[257]]), ch ^= s[(s[s[256]] + s[s[257]]) & 0xff];
    text.push_back(key.size());
    while (key.size())
        text.push_back(key.back() * 16 + 16 - key.back() / 16 & 0xff), key.pop_back();
    ofile.write(text.data(), text.size());
}
