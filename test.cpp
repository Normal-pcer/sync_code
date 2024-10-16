/**
 * 
 */

#include "lib"
using namespace lib;

using std::string;


namespace Solution {

    struct Identifier;

    struct Type {
        string name;
        std::vector<Identifier> members;
        size_t occupied_size;
        size_t align_size;
    };

    struct Identifier {
        string name;
        Type type;
    };

    struct Element {
        Identifier id;
        size_t address;
    };

    std::map<size_t, Element> memory;

    
    void init() {

    }

    void solve() {
        init();

    }
}


int main() {
    initDebug;
    Solution::solve();
    return 0;
}