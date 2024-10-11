#include <bits/stdc++.h>
namespace lib {
    class string: public std::string {
    public:
        string() = default;
        string(const char* s) : std::string(s) {}
        string(std::string s) : std::string(s) {}
        string join(std::vector<std::string> s) {
            string res = "";
            size_t size = 0;
            for (auto& i: s)  size += i.size();
            res.reserve(size);
            for (auto& i: s) {
                res += i;
                res += *this;
            }
            return res;
        }
        vector<string> split(std::vector<char> c={' ', '\n', '\t'}) {
            vector<string> res;
            size_t start = 0;
            for (size_t i = 0; i < this->size(); i++) {
                if (std::find(c.begin(), c.end(), (*this)[i]) != c.end()) {
                    if (i - start > 0)
                        res.push_back(this->substr(start, i - start));
                    start = i + 1;
                }
            }
            if (start < this->size())
                res.push_back(this->substr(start, this->size()));
            return res;
        }
        vector<string> split(char c) {
            return split(vector<char>{c});
        }
        string repeat(int times) const {
            string result = *this;
            result.reserve(times * this->size());
            for (int i=1; i<times; i++)  result += *this;
            return result;
        }
        string operator* (int times) const {
            return this->repeat(times);
        }
    };
}