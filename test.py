import re

def process_string(input_str):
    # 使用正则表达式匹配逗号分割的英文单词部分
    def replace_match(match):
        # 提取匹配到的部分（包含原始空格）
        words_str = match.group(0).strip()
        # 按逗号分割，但保留每个部分的原始内容
        parts = [part.strip() for part in words_str.split(',') if part.strip()]
        # 只保留前12个部分，剩下的用省略号代替
        if len(parts) > 10:
            result = ', '.join(parts[:10]) + ', ...'
        else:
            result = ', '.join(parts)
        return result

    # 匹配由逗号分割的英文单词部分（可能包含空格）
    pattern = r'([a-zA-Z0-9]+\s*,\s*)*([a-zA-Z0-9]+\s*)*[a-zA-Z0-9]+'
    # 替换匹配到的部分
    output_str = re.sub(pattern, replace_match, input_str)
    return output_str

# 读取多行输入并逐行处理
if __name__ == "__main__":
    import sys
    input_lines = sys.stdin.read()  # 读取所有输入
    output_lines = process_string(input_lines)
    print(output_lines, end="")  # 输出时不额外添加换行符