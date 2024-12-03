#include "./lib"

int main() {
    std::vector<int> v1 {0, 1, 2};
    std::vector<int> v2 {3, 4, 5};

                        

    std::vector<int> res;
    res.resize(v1.size());
    std::transform(v1.begin(), v1.end(), v2.begin(), res.begin(), lam(x, y, x*y));

    for (auto i: res)  std::cout << i << ' ';
    std::cout << std::endl;
    return 0;
}