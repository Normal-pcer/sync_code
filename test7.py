MOD = 1_000_000_007

def main():
    n, m = map(int, input().split())
    if n == 0 and m == 0:
        print(0)
        return
    
    max_nm = n + m
    # Precompute factorial and inverse factorial modulo MOD up to max_nm
    fact = [1] * (max_nm + 1)
    for i in range(1, max_nm +1):
        fact[i] = fact[i-1] * i % MOD
    inv_fact = [1] * (max_nm +1)
    inv_fact[max_nm] = pow(fact[max_nm], MOD-2, MOD)
    for i in range(max_nm-1, -1, -1):
        inv_fact[i] = inv_fact[i+1] * (i+1) % MOD
    
    # Compute combination C(n+m, n)
    a = n + m
    b = n
    if a < 0 or b < 0 or b > a:
        comb = 0
    else:
        comb = fact[a] * inv_fact[b] % MOD
        comb = comb * inv_fact[a - b] % MOD
    
    denominator = (n + m) % MOD
    if denominator == 0:
        print(0)
        return
    
    numerator = (denominator + 2 * m * n) % MOD
    term = numerator * pow(denominator, MOD-2, MOD) % MOD
    ans = comb * term % MOD
    print(ans)

if __name__ == "__main__":
    T = int(input())
    for _ in range(T):
        main()