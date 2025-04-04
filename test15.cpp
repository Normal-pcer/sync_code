#include <fstream>
#include <iostream>
#include <string>
using namespace std;

// 逆变换函数
unsigned char decode_key_byte(unsigned char encoded_byte) {
    return ((encoded_byte - 16) * 16 + 16) % 256;
}

int main() {
    // 打开 enc.bin 文件
    ifstream file("enc.bin", ios::binary);
    string data((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    // 提取密钥长度
    unsigned char key_len = data.back();
    data.pop_back(); // 移除密钥长度

    // 提取编码后的密钥
    string encoded_key(data.end() - key_len, data.end());
    data.resize(data.size() - key_len); // 移除编码后的密钥

    // 解码密钥
    string key;
    for (auto it = encoded_key.rbegin(); it != encoded_key.rend(); ++it) {
        key += decode_key_byte(*it);
    }

    // RC4 解密
    unsigned char s[258], c = 0;
    for (int i = 0; i < 258; i++) s[i] = i > 0xff ? 0 : i;
    for (int i = 0; i < 256; i++) swap(s[i], s[c = s[i] + c + key[i % key.size()]]);
    for (auto &ch : data) {
        s[257] += s[++s[256]];
        swap(s[s[256]], s[s[257]]);
        ch ^= s[(s[s[256]] + s[s[257]]) & 0xff];
    }

    // 输出解密结果
    ofstream output("decrypted.txt", ios::binary);
    output.write(data.data(), data.size());
    output.close();

    cout << "Decryption completed. Check 'decrypted.txt'." << endl;
}