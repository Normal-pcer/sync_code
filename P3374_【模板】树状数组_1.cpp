/**
 * @link https://www.luogu.com.cn/problem/P3374
 */

#include "./lib"

#include "./libs/io.hpp"

using namespace lib;

namespace Solution_1386135091856135 {

    int N, M;
    enum Type {
        Query, Modify
    };
    struct Op {
        Type type;
        int item, attach;

        bool operator< (const Op &other) const noexcept {
            return item < other.item;
        }

        bool operator<= (const Op &other) const noexcept {
            return item <= other.item;
        }
    };
    std::vector<Op> operations;
    std::vector<int> queries;
    std::vector<int> result;

    // [begin, end)
    void merge_sort(int begin, int end) {
        if (begin + 1 == end)  return;
        auto mid = (begin + end) >> 1;
        merge_sort(begin, mid), merge_sort(mid, end);

        std::vector<Op> tmp(end - begin);
        auto i = begin, j = mid, k = 0;
        auto sum = 0;

        while (i != mid or j != end) {
            if (j == end or (i != mid and operations.at(i) <= operations.at(j))) {
                if (operations.at(i).type == Modify)  sum += operations.at(i).attach;
                tmp.at(k++) = operations.at(i++);
            } else {
                if (operations.at(j).type == Query)  result.at(operations[j].attach) += sum;
                tmp.at(k++) = operations.at(j++);
            }
        }

        std::copy(tmp.begin(), tmp.end(), operations.begin() + begin);
    }

    void solve() {
        io >> N >> M;
        for (auto i = 0; i < N; i++) {
            int x;  io >> x;
            operations.emplace_back(Modify, i + 1, x);
        }
        for (auto i = 0; i < M; i++) {
            int x, y, z;
            io >> x >> y >> z;
            if (x == 1) {
                operations.emplace_back(Modify, y, z);
            } else {
                operations.emplace_back(Query, z, queries.size()), queries.push_back(z);
                operations.emplace_back(Query, y-1, queries.size()), queries.push_back(y-1);
            }
        }
        result.resize(queries.size());

        merge_sort(0, operations.size());

        for (auto i = 0; i != (int)result.size(); i += 2) {
            auto x = result.at(i), y = result.at(i+1);
            io << x - y << endl;
        }
    }
}

int main() {
    initDebug;
    Solution_1386135091856135::solve();
    return 0;
}
