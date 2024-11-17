#include <bits/stdc++.h>

class Solution {
public:
    // 计算子矩形数量，必须包含 mid 列
    int sub(int height, int left, int mid, int right) {
        return height * (mid - left + 1) * (right - mid + 1);
    }

    int numSubmat(std::vector<std::vector<int>>& mat) {
        if (mat.empty())  return 0;  // 特判空的情况

        auto M = mat.at(0).size();  // 计算原边长

        std::vector<int> H(M+1);  // 至多向上延伸的高度，保留右侧的一个 0

        int ans = 0;
        for (auto &bottom: mat) {
            for (auto i = 0ULL; i < M; i++) {
                if (bottom[i])  H[i] += 1;
                else  H[i] = 0;
            }

            std::vector<int> left(M+1);  // 左侧第一列不高于 i 的
            std::deque<int> st;  // H[i] 严格递增

            for (auto i = 0ULL; i < H.size(); i++) {  // 遍历每一列
                while (not st.empty() and H[st.back()] >= H[i]) {
                    // 压入一个更高或等高的
                    auto height = H[st.back()];  // 结算这一列
                    auto l = left[st.back()] + 1;
                    auto r = i - 1;

                    auto ans_this = sub(height, l, st.back(), r);
                    ans += ans_this;
                    st.pop_back();
                }
                
                left[i] = st.empty()? -1: st.back();  // 为空则取 -1
                st.push_back(i);
            }
        }

        return ans;
    }
};

Solution sol;
int main() {
    int N, M;
    std::cin >> N >> M;
    std::vector<std::vector<int>> mat(N, std::vector<int>(M));
    for (auto i = 0; i < N; i++) {
        for (auto j = 0; j < M; j++) {
            std::cin >> mat[i][j];
        }
    }
    std::cout << sol.numSubmat(mat) << std::endl;
}