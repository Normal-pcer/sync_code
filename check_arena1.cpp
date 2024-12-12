/**
 * @link https://neooj.com:8082/oldoj/problem.php?id=3342
 */

#pragma GCC optimize(3)
#pragma GCC optimize("Ofast")
#pragma GCC optimize("inline")
#include "./lib"

#include "./libs/io.hpp"

using namespace lib;

#define at operator[]
namespace Solution_9736888855439916 {
    class BIT {
        std::vector<int> c;

        static int inline constexpr lowbit(int x) noexcept { return x & -x; }
    public:
        BIT(size_t N): c(N+1) {}
        int query(int x) const {
            x++;  auto ans = 0;
            while (x) {
                ans += c.at(x);
                x -= lowbit(x);
            }
            return ans;
        }
        void update(int x, int val) {
            x++;
            while (x < (int)c.size()) {
                c.at(x) += val;
                x += lowbit(x);
            }
        }
    } tr(0);

    struct Segment {
        int left, right, index;
        bool update;
    };

    std::vector<Segment> segs;
    std::vector<int> values {-inf, +inf};
    int getIndex(int x) {
        auto it = std::lower_bound(values.begin(), values.end(), x);
        assert(*it == x);
        return std::distance(values.begin(), it);
    }

    void cdq(std::vector<Segment>::iterator begin, std::vector<Segment>::iterator end, auto &ans) {
        // auto x = std::distance(segs.begin(), begin), y = std::distance(segs.begin(), end);
        // debug  io << std::format("cdq({}, {})", x, y) << endl;
        
        if (end - begin == 1)  return;
        auto mid = begin + ((end - begin) >> 1);

        cdq(begin, mid, ans);
        cdq(mid, end, ans);

        std::stable_sort(begin, mid, lam(x, y, x.right < y.right));
        std::stable_sort(mid, end, lam(x, y, x.left < y.left));

        auto i = begin, j = mid;
        auto cnt = 0;
        while (i != mid or j != end) {
            if (j == end or (i != mid and i->right < j->left)) {
                if (i->update)  cnt++;
                i++;
            } else {
                if (not j->update) {
                    ans.at(j->index) += cnt;
                    // debug {
                    //     if (begin != i) {
                    //         debug io << std::format("[{}]({}, {}) is not cross with: ", j->index, j->left, j->right);
                    //         for (auto it = begin; it != i; ++it)  io << std::format("[{}]({}, {})", it->index, it->left, it->right) << ' ';
                    //         io << endl;
                    //     }
                    // }
                }
                j++;
            }
        }
    }

    void cdq2(std::vector<Segment>::iterator begin, std::vector<Segment>::iterator end, auto &ans) {
        // auto x = std::distance(segs.begin(), begin), y = std::distance(segs.begin(), end);
        // debug  io << std::format("cdq({}, {})", x, y) << endl;
        
        if (end - begin == 1)  return;
        auto mid = begin + ((end - begin) >> 1);

        cdq2(begin, mid, ans);
        cdq2(mid, end, ans);

        std::sort(begin, mid, lam(x, y, x.left > y.left));
        std::sort(mid, end, lam(x, y, x.right > y.right));

        auto i = begin, j = mid;
        auto cnt = 0;
        while (i != mid or j != end) {
            if (j == end or (i != mid and i->left > j->right)) {
                if (i->update)  cnt++;
                i++;
            } else {
                if (not j->update) {
                    ans.at(j->index) += cnt;
                    // debug {
                    //     if (begin != i) {
                    //         debug io << std::format("[{}]({}, {}) is not cross with: ", j->index, j->left, j->right);
                    //         for (auto it = begin; it != i; ++it)  io << std::format("[{}]({}, {})", it->index, it->left, it->right) << ' ';
                    //         io << endl;
                    //     }
                    // }
                }
                j++;
            }
        }
    }

    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, Q;  io >> N;
        segs.resize(N);
        for (auto i = 0; i < N; i++) {
            auto &[left, right, index, update] = segs.at(i);
            io >> left >> right, index = i, update = true;
            values.push_back(left), values.push_back(right);
        }
        io >> Q;
        segs.resize(N + Q);
        values.reserve(1<<24);
        for (auto i = 0; i < Q; i++) {
            auto &[left, right, index, update] = segs.at(N+i);
            io >> left >> right, index = i, update = false;
            values.push_back(left), values.push_back(right);
        }

        std::sort(values.begin(), values.end());
        values.erase(std::unique(values.begin(), values.end()), values.end());
        for (auto &i: segs)  i.left = getIndex(i.left), i.right = getIndex(i.right);
        auto const begin = segs.begin(), end = segs.end(), mid = std::partition(begin, end, lam(x, x.update));

        tr = BIT(values.size()+5);

//         std::vector<ll> left_cross(Q);
//         {
//             auto &ans = left_cross;
//             std::sort(begin, mid, lam(const &x, const &y, x.right < y.right));
//             std::sort(mid, end, lam(const &x, const &y, x.right < y.right));
//             auto i = begin, j1 = mid, j2 = mid;
//             tr.tick();
//             while (j1 != end or j2 != end) {
//                 if (j1 == end or (j2 != end and j2->left < j1->right)) {
//                     while (i != mid and i->right < j2->left) {
//                         debug io << std::format("tr.update({}, +1)", i->right) << endl;
//                         tr.update(i->left, +1);
//                         i++;
//                     }
// debug io << std::format("[{}]ans.at({}) -= [{}]tr.query({})", ans.at(j2->index), j2->index, tr.query(j2->left-1), j2->left-1) << endl;
//                     ans.at(j2->index) -= tr.query(j2->left-1);
//                     j2++;
//                 } else {
//                     while (i != mid and i->right < j1->right) {
//                         tr.update(i->left, +1);
//                         i++;
//                     }
//                     ans.at(j1->index) += tr.query(j1->left-1);
//                     j1++;
//                 }
//             }
//         }
        std::vector<ll> left_contain(Q);
        {
            auto &ans = left_contain;
            std::sort(begin, mid, lam(const &x, const &y, x.right < y.right));
            std::sort(mid, end, lam(const &x, const &y, x.right < y.right));
            auto i = begin, j = mid;
            while (j != end) {
                while (i != mid and i->right <= j->right) {
                    tr.update(i->left, +1);
                    i++;
                }
                ans.at(j->index) += tr.query(getIndex(inf)) - tr.query(j->left - 1);
                j++;
            }
        }
        // std::vector<ll> right_cross(Q);
        // {
        //     auto &ans = right_cross;
        //     std::sort(begin, mid, lam(const &x, const &y, x.left > y.left));
        //     std::sort(mid, end, lam(const &x, const &y, x.left > y.left));
        //     auto i = begin, j1 = mid, j2 = mid;
        //     tr.tick();
        //     while (j1 != end or j2 != end) {
        //         if (j1 == end or (j2 != end and j2->right > j1->left)) {
        //             while (i != mid and i->left > j2->right) {
        //                 debug io << std::format("tr.update({}, +1)", i->right) << endl;
        //                 tr.update(i->right, +1);
        //                 i++;
        //             }
        //             debug  io << std::format("ans.at({}) -= tr.query(inf) - tr.query({})", j2->index, j2->right) << endl;
        //             ans.at(j2->index) -= tr.query(getIndex(inf)) - tr.query(j2->right);
        //             j2++;
        //         } else {
        //             while (i != mid and i->left > j1->left) {
        //                 tr.update(i->right, +1);
        //                 i++;
        //             }
        //             debug  io << std::format("ans.at({}) += tr.query(inf) - tr.query({})", j1->index, j1->right) << endl;
        //             ans.at(j1->index) += tr.query(getIndex(inf)) - tr.query(j1->right);
        //             j1++;
        //         }
                
        //     }
        // }
        // std::vector<ll> rev_contain(Q);
        // {
        //     auto &ans = rev_contain;
        //     std::sort(begin, mid, lam(const &x, const &y, x.right > y.right));
        //     std::sort(mid, end, lam(const &x, const &y, x.right > y.right));
        //     auto i = begin, j = mid;
        //     tr.tick();
        //     while (j != end) {
        //         while (i != mid and i->right >= j->right) {
        //             tr.update(i->left, +1);
        //             i++;
        //         }
        //         ans.at(j->index) += tr.query(j->left);
        //         j++;
        //     }
        // }
        std::vector<ll> not_cross(Q);
        {
            std::vector<ll> tmp(Q);
            cdq(begin, end, not_cross);
            std::partition(begin, end, lam(x, x.update));
            cdq2(begin, end, tmp);
            std::transform(tmp.begin(), tmp.end(), not_cross.begin(), not_cross.begin(), lam(x, y, x+y));
        }
        

        // debug for (auto i: left_cross)  io << i << ' ';
        // debug io << endl;
        debug for (auto i: left_contain)  io << i << ' ';
        debug io << endl;
        // debug for (auto i: right_cross)  io << i << ' ';
        // debug io << endl;
        // debug for (auto i: rev_contain)  io << i << ' ';
        // debug io << endl;
        debug for (auto i: not_cross)  io << i << ' ';
        debug io << endl;
        for (auto i = 0; i < Q; i++) {
            auto contain = left_contain.at(i);
            // auto cross = left_cross.at(i) + right_cross.at(i) + rev_contain.at(i);
            auto cross = N - not_cross.at(i) - contain;
            io << cross << ' ' << contain << endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_9736888855439916::solve();
    return 0;
}
