import os
import glob

# 设置目标目录
target_dir = "."

# 查找并删除符合条件的文件
for pattern in ["*.in", "*.out", "*.ans", "*.exe"]:
    for file_path in glob.glob(os.path.join(target_dir, "**", pattern), recursive=True):
        try:
            os.remove(file_path)
            print(f"已删除: {file_path}")
        except Exception as e:
            print(f"删除失败: {file_path}, 错误: {e}")

print("清理完成")