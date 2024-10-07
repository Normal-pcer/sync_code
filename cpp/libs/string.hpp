#include <string>
#include <vector>

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
    };
}