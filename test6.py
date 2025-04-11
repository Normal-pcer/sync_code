n = int(input())
div_counts = [0] * (n + 1)

for i in range(1, n + 1):
    for j in range(i, n + 1, i):
        div_counts[j] += 1

total = sum(div_counts[1:n+1])
average = total / n
max_val = max(div_counts[1:n+1])

print(f"平均值: {average:.2f}")
print(f"最大值: {max_val}")
