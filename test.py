import math

def calculate_space_constant():
    # 输入元素大小和各维度尺寸
    element_size = int(input("Enter element sizeof (e.g., 4 for int): "))
    dims = list(map(int, input("Enter dimensions separated by space (e.g., 2 3 4): ").split()))
    
    if not dims:
        print("Error: At least one dimension is required.")
        return
    
    # 计算结构体总开销
    total_overhead = 24  # 外层 vector 的结构体开销
    cumulative = 1       # 累积维度乘积
    
    for dim in dims[:-1]:
        cumulative *= dim
        total_overhead += 24 * cumulative
    
    # 计算多维数组和嵌套 vector 的总内存
    array_mem = math.prod(dims) * element_size
    vector_mem = total_overhead + array_mem
    
    # 计算空间常数比率
    ratio = vector_mem / array_mem if array_mem != 0 else float('inf')
    
    # 输出结果
    print("\nResults:")
    print(f"- Nested vector total memory: {vector_mem} bytes")
    print(f"- Multidimensional array memory: {array_mem} bytes")
    print(f"- Space constant (vector/array): {ratio:.2f}x")

if __name__ == "__main__":
    calculate_space_constant()