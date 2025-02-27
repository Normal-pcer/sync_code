from math import comb

# 定义变量
N = 5548
K = 17
n = 310
k = 1

# 计算组合数
C_K_k = comb(K, k)
C_NminusK_nminusk = comb(N - K, n - k)
C_N_n = comb(N, n)

# 计算概率
probability = (C_K_k * C_NminusK_nminusk) / C_N_n
print(probability)