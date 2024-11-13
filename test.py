def quick_power(base, exponent):
    result = 1
    while exponent > 0:
        if exponent % 2 == 1:
            result *= base
        base *= base
        exponent //= 2
    return result

print(quick_power(2, 10))  # 输出 1024
print(quick_power(3, 5))   # 输出 243
print(quick_power(5, 3))   # 输出 125