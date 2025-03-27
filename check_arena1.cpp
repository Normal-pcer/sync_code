#include <bits/stdc++.h>
#include "./libs/fixed_int.hpp"
#define FILENAME "swiss"

char constexpr endl = '\n';

/*
执行无穷次，趋近于按照实力值排序。
上升的分数有限制。最多上升 R 分。
能够成为对应排名的人，当前分数的浮动一定不超过 2R。
所以最后值域很小，用类似桶排序的方法即可。

复杂度：O(N + 4R)。
*/
namespace Solution {
    struct Person {
        i32 score = 0, weight = 0, index = 0;

        auto operator< (Person const &other) -> bool {  // 按照排名排序
            if (score == other.score) return index < other.index;
            return score > other.score;
        }
    };
    auto solve2() -> void {
        i32 N, R, Q; std::cin >> N >> R >> Q;
        N += N;
        std::vector<Person> persons(N);
        for (i32 i = 0; i < N; i++) persons[i].index = i;
        for (i32 i = 0; i < N; i++) std::cin >> persons[i].score;
        for (i32 i = 0; i < N; i++) std::cin >> persons[i].weight;

        for (auto t = R; t --> 0; ) {
            std::sort(persons.begin(), persons.end());

            for (i32 i = 0; i < N; i += 2) {
                if (persons[i].weight < persons[i + 1].weight) persons[i + 1].score++;
                else persons[i].score++;
            }
        }

        std::sort(persons.begin(), persons.end());
        std::cout << persons[Q - 1].index + 1 << endl;
    }
    
    auto solve() -> void {
        i32 N, R, Q; std::cin >> N >> R >> Q;
        N += N;
        std::vector<Person> persons(N);
        for (i32 i = 0; i < N; i++) persons[i].index = i;
        for (i32 i = 0; i < N; i++) std::cin >> persons[i].score;
        for (i32 i = 0; i < N; i++) std::cin >> persons[i].weight;

        std::nth_element(persons.begin(), persons.begin() + Q - 1, persons.end());
        auto max_possible_score = persons[Q - 1].score + 2 * R + 10;
        auto min_possible_score = persons[Q - 1].score - 2 * R - 10;

        std::vector<Person> possible;
        for (auto x: persons) {
            if (x.score <= max_possible_score and x.score >= min_possible_score) {
                possible.push_back(x);
            }
        }

        auto cur_it = std::lower_bound(possible.begin(), possible.end(), persons[Q - 1]);
        auto new_rank = cur_it - possible.begin();  // 在有效人中的排名

        auto my_sort = [&](std::vector<Person> &vec) -> void {
            if (vec.empty()) return;
            i32 constexpr inf = 0x3f3f3f3f;
            auto min = inf, max = -inf;
            for (auto x: vec) {
                min = std::min(min, x.score);
                max = std::max(max, x.score);
            }
            
            i32 size = vec.size();
            std::vector<i32> index_buc(N, -1);

            for (i32 i = 0; i != size; i++) {
                index_buc[vec[i].index] = i;
            }

            std::vector<std::vector<i32>> score_buc(max - min + 1);
            for (i32 i = N; i --> 0; ) {
                if (index_buc[i] != -1) {
                    auto j = index_buc[i];
                    score_buc[vec[j].score - min].push_back(j);
                }
            }
            std::vector<Person> res;
            res.reserve(vec.size());
            std::for_each(score_buc.rbegin(), score_buc.rend(), [&](auto &line) -> void {
                for (auto x: line) res.push_back(vec[x]);
            });
            vec = res;
        };

        for (auto t = R; t --> 0; ) {
            my_sort(possible);

            for (i32 i = 0; i < N; i += 2) {
                if (persons[i].weight < persons[i + 1].weight) persons[i + 1].score++;
                else persons[i].score++;
            }
        }

        my_sort(possible);
        std::cout << possible[new_rank].index + 1 << endl;
    }
}

auto main() -> int {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
    // std::freopen(FILENAME ".in", "r", stdin);
    // std::freopen(FILENAME ".out", "w", stdout);

    Solution::solve2();
}
