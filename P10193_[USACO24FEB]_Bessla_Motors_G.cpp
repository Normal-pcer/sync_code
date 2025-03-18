/**
 * @link https://www.luogu.com.cn/problem/P10193
 */
#include "./lib_v5.hpp"

#include "./libs/fixed_int.hpp"
using namespace lib;

/**
 * 首先发现 K 特别小（K <= 10），所以在图的所有节点上记录最近的 K 个充电站是可行的。
 * 一个比较暴力的方法是从每个充电站开始向外走，更新每个节点到自己的距离。
 * 一个优化是，如果不会对一个点造成任何贡献，那么就不需要向外走了。
 * 这是直接跑 C 次最短路。
 * 
 * 考虑优化。
 * 如果 K = 1，可以一上来就把所有的起点都扔到堆里。
 * 有一个很奇怪的想法，记录一个点从哪个充电站转移过来，以及经过的距离。
 * 跑最短路的时候还是根据距离排序。
 * 如果每个节点都被更新过了 K 次，就结束。
 * 具体地，vis[] 的定义改成被经过了多少次，这个值 >= K 即可直接 continue
 * 然后记录前 K 个 dis 即可。复杂度应该是正确的。（这个图最多被遍历 10 次）
 */
namespace Solution_4040868418093689 {
    i32 constexpr maxK = 10;
    i64 constexpr inf64 = infLL;

    template <uz maxSize>
    class InplaceVector {
        std::array<i64, maxSize> _data;
        uz _size = 0;
    public:
        constexpr InplaceVector() { _data.fill(0); }
        auto constexpr size() const -> uz { return _size; }
        auto constexpr resize(uz new_size) -> void {
            assert(new_size < maxSize);
            if (new_size < size())  return _size = new_size, void();
            if (new_size > size()) {
                for (uz i = size(); i < new_size; i++) {
                    _data[i] = 0;
                }
                _size = new_size;
            }
        }
        auto constexpr begin() const -> i64 const * { return _data.begin(); }
        auto constexpr end() const -> i64 const * { return _data.end(); }
        auto constexpr begin() -> i64 * { return _data.begin(); }
        auto constexpr end() -> i64 * { return _data.end(); }
        auto constexpr pushBack(i64 x) -> void { return assert(size() < maxSize), _data[_size++] = x, void(); }
        auto constexpr operator[] (uz idx) const -> i64 { return assert(idx < size()), _data[idx]; }
        auto constexpr operator[] (uz idx) -> i64 & { return assert(idx < size()), _data[idx]; }
    };

    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N, M, C, R, K;
        std::cin >> N >> M >> C >> R >> K;

        struct Node {
            i32 to = 0, val = 0;
        };
        std::vector<std::vector<Node>> graph(N + 1);
        for (i32 m = M; m --> 0; ) {
            i32 x, y, val;  std::cin >> x >> y >> val;
            graph[x].push_back({y, val});
            graph[y].push_back({x, val});
        }

        std::vector<InplaceVector<maxK>> dis(N + 1);  // 距离最近的 K 个数
        [&]() -> void {
            struct QueueNode {
                i32 p = 0;    // 当前点
                i32 src = 0;  // 从谁转移
                i64 dis = 0;  // 距离多远

                // 按照 dis **反向**比较
                auto operator<=> (QueueNode const &other) const -> std::weak_ordering {
                    return other.dis <=> dis;
                }
            };

            std::priority_queue<QueueNode> q;
            for (i32 i = 1; i <= C; i++) {
                q.push({i, i, 0});
            }

            std::set<std::pair<i32, i32>> vis;  // 是否经过从 i 转移到 j 点
            while (not q.empty()) {
                auto [x, src, val] = q.top();  q.pop();
                debug  std::cout << "x = " << x << ", src = " << src << ", val = " << val << std::endl;
                if (static_cast<i32>(dis[x].size()) >= K)  continue;
                if (vis.contains({src, x}))  continue;
                dis[x].pushBack(val);
                vis.insert({src, x});

                for (auto y: graph[x]) {
                    q.push({y.to, src, y.val + val});
                }
            }
        }();

        std::vector<i32> ans_collection;
        for (i32 i = C + 1; i <= N; i++) {
            if (static_cast<i32>(dis[i].size()) >= K and dis[i][K - 1] <= R) {
                ans_collection.push_back(i);
            }
        }
        std::cout << ans_collection.size() << endl;
        for (auto x: ans_collection)  std::cout << x << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_4040868418093689::solve();
    return 0;
}
