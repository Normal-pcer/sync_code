import random
import math
from fractions import Fraction
from typing import Union, Tuple, List, Optional

# 类型别名
Number = Union[int, float, Fraction]
AnswerType = Union[float, Fraction, Tuple[Fraction, Fraction]]

# 常量配置
ADD_SUB_DIGITS = 4  # 加减法位数
MUL_DIGITS_A = 4    # 乘法第一个数位数
MUL_DIGITS_B = 3    # 乘法第二个数位数
DIV_DIGITS_A = 4    # 除法被除数位数
DIV_DIGITS_B = 3    # 除法除数位数
DIV_PRECISION = 6   # 除法结果有效数字位数
TOLERANCE = 1e-5    # 比较容差

def generate_number(digits: int, allow_negative: bool = False) -> float:
    """生成指定有效数字位数的随机数"""
    # 确定整数部分和小数部分的位数
    integer_digits = random.randint(1, digits)
    decimal_digits = digits - integer_digits
    
    # 生成整数部分
    integer_part = random.randint(10**(integer_digits-1), 10**integer_digits - 1)
    
    # 生成小数部分
    if decimal_digits > 0:
        decimal_part = random.randint(10**(decimal_digits-1), 10**decimal_digits - 1)
        number = float(f"{integer_part}.{decimal_part}")
    else:
        number = float(integer_part)
    
    # 随机添加负号
    if allow_negative and random.choice([True, False]):
        number = -number
    
    return number

def format_number(num: float, digits: int = 6) -> str:
    """格式化数字为字符串，控制有效数字位数"""
    if num == 0:
        return "0"
    
    # 处理负数
    sign = "-" if num < 0 else ""
    num = abs(num)
    
    # 确定格式
    if num >= 10**(digits-1) or num <= 10**(-digits):
        # 使用科学计数法避免过长的小数
        return f"{sign}{num:.{digits}g}"
    else:
        # 普通小数格式
        return f"{sign}{num:.{digits}g}"

def generate_arithmetic() -> Tuple[str, float]:
    """生成四则运算题目"""
    operations = ['+', '-', '×', '÷']
    op = random.choice(operations)
    
    if op == '+':  # 4+4位加法
        a = generate_number(ADD_SUB_DIGITS, allow_negative=True)
        b = generate_number(ADD_SUB_DIGITS, allow_negative=True)
        return f"{format_number(a)} + {format_number(b)} = ", a + b
    
    elif op == '-':  # 4-4位减法
        a = generate_number(ADD_SUB_DIGITS, allow_negative=True)
        b = generate_number(ADD_SUB_DIGITS, allow_negative=True)
        # 确保结果非负
        if a < b:
            a, b = b, a
        return f"{format_number(a)} - {format_number(b)} = ", a - b
    
    elif op == '×':  # 4×3位乘法
        a = generate_number(MUL_DIGITS_A, allow_negative=True)
        b = generate_number(MUL_DIGITS_B, allow_negative=True)
        result = a * b
        return f"{format_number(a)} × {format_number(b)} = ", result
    
    else:  # 4÷3位除法
        b = generate_number(DIV_DIGITS_B, allow_negative=True)
        # 避免除数为0
        while abs(b) < 0.001:
            b = generate_number(DIV_DIGITS_B, allow_negative=True)
        
        a = generate_number(DIV_DIGITS_A, allow_negative=True)
        result = a / b
        
        # 避免过大结果
        if abs(result) > 10**6:
            a = generate_number(2, allow_negative=True)
            result = a / b
        
        return f"{format_number(a)} ÷ {format_number(b)} = ", result

def generate_linear_equation() -> Tuple[str, Fraction]:
    """生成一次方程 ax+b=cx+d"""
    # 确保解为整数或简单分数
    while True:
        # 随机生成解
        numerator = random.randint(-10, 10)
        denominator = random.randint(1, 10)
        
        # 避免分母为0
        if denominator == 0:
            continue
            
        # 创建分数
        x = Fraction(numerator, denominator).limit_denominator(10)
        
        # 生成系数
        a = random.randint(1, 10)
        c = random.randint(1, 10)
        b = random.randint(-20, 20)
        
        # 确保方程有解且系数合理
        if a == c:
            continue
            
        # 计算d确保方程平衡: d = a*x + b - c*x
        d = (a - c) * x + b
        
        # 格式化系数
        def format_coeff(coeff: int) -> str:
            if coeff == 1:
                return ""
            elif coeff == -1:
                return "-"
            else:
                return str(coeff)
        
        # 构建方程字符串
        left = f"{format_coeff(a)}x"
        if b > 0:
            left += f" + {b}"
        elif b < 0:
            left += f" - {-b}"
        
        right = f"{format_coeff(c)}x"
        if d > 0:
            right += f" + {d}"
        elif d < 0:
            right += f" - {-d}"
        
        equation = f"{left} = {right}"
        return equation, x

def generate_quadratic_equation() -> Tuple[str, Tuple[Fraction, Fraction]]:
    """生成可因式分解的二次方程"""
    while True:
        # 生成两个根
        r1_num = random.randint(-10, 10)
        r1_den = random.randint(1, 10)
        r2_num = random.randint(-10, 10)
        r2_den = random.randint(1, 10)
        
        # 避免分母为0
        if r1_den == 0 or r2_den == 0:
            continue
            
        r1 = Fraction(r1_num, r1_den).limit_denominator(10)
        r2 = Fraction(r2_num, r2_den).limit_denominator(10)
        
        # 确保根不同
        if r1 == r2:
            continue
            
        # 构建方程: (x - r1)(x - r2) = 0
        # 展开: x² - (r1+r2)x + r1*r2 = 0
        b_coeff = -(r1 + r2)
        c_coeff = r1 * r2
        
        # 确保系数合理
        if abs(b_coeff) > 20 or abs(c_coeff) > 100:
            continue
            
        # 格式化系数
        def format_coeff(coeff: Fraction, is_first: bool = False) -> str:
            if coeff == 0:
                return "0"
            
            if coeff.denominator == 1:
                num = coeff.numerator
                if num == 1 and not is_first:
                    return ""
                elif num == -1:
                    return "-"
                else:
                    return str(num)
            else:
                return f"{coeff.numerator}/{coeff.denominator}"
        
        # 构建方程字符串
        equation = "x²"
        
        # b项
        if b_coeff != 0:
            if b_coeff > 0:
                equation += f" + {format_coeff(b_coeff)}x"
            else:
                equation += f" - {format_coeff(-b_coeff)}x"
        
        # c项
        if c_coeff != 0:
            if c_coeff > 0:
                equation += f" + {format_coeff(c_coeff)}"
            else:
                equation += f" - {format_coeff(-c_coeff)}"
        
        equation += " = 0"
        return equation, (r1, r2)

def parse_fraction(s: str) -> Optional[Fraction]:
    """解析分数输入"""
    try:
        s = s.strip()
        if not s:
            return None
            
        if '/' in s:
            parts = s.split('/')
            if len(parts) != 2:
                return None
            num = int(parts[0])
            denom = int(parts[1])
            if denom == 0:
                return None
            return Fraction(num, denom)
        else:
            # 尝试解析整数或小数
            return Fraction(s).limit_denominator(1000)
    except (ValueError, ZeroDivisionError):
        return None

def validate_arithmetic(user_input: str, correct_answer: float) -> bool:
    """验证四则运算答案"""
    try:
        # 尝试解析分数或小数
        user_value = parse_fraction(user_input)
        if user_value is None:
            return False
        
        # 转换为浮点数
        user_float = float(user_value)
        
        # 计算相对误差
        if abs(correct_answer) < 1e-10:  # 处理接近0的情况
            return abs(user_float) < 1e-5
        else:
            relative_error = abs(user_float - correct_answer) / abs(correct_answer)
            return relative_error < TOLERANCE
    except (ValueError, TypeError):
        return False

def validate_linear(user_input: str, correct_answer: Fraction) -> bool:
    """验证一次方程答案"""
    user_value = parse_fraction(user_input)
    if user_value is None:
        return False
    
    # 比较分数
    return user_value == correct_answer

def validate_quadratic(user_input: str, correct_roots: Tuple[Fraction, Fraction]) -> bool:
    """验证二次方程答案"""
    # 分割用户输入的根
    roots_str = user_input.split()
    if len(roots_str) != 2:
        return False
        
    # 解析两个根
    root1 = parse_fraction(roots_str[0])
    root2 = parse_fraction(roots_str[1])
    
    if root1 is None or root2 is None:
        return False
    
    # 检查根是否匹配（顺序无关）
    r1, r2 = correct_roots
    return (
        (root1 == r1 and root2 == r2) or 
        (root1 == r2 and root2 == r1)
    )

def format_answer(answer: AnswerType) -> str:
    """格式化答案用于显示"""
    if isinstance(answer, tuple):  # 二次方程
        r1, r2 = answer
        return f"{format_root(r1)} {format_root(r2)}"
    elif isinstance(answer, Fraction):  # 一次方程
        return format_root(answer)
    else:  # 四则运算
        # 处理非常小或非常大的数
        if abs(answer) < 1e-5 or abs(answer) > 1e10:
            return f"{answer:.{DIV_PRECISION}g}"
        
        # 去除不必要的尾随零
        formatted = f"{answer:.6f}".rstrip('0').rstrip('.')
        return formatted if formatted != "" else "0"

def format_root(root: Fraction) -> str:
    """格式化根显示"""
    if root.denominator == 1:
        return str(root.numerator)
    else:
        return f"{root.numerator}/{root.denominator}"

def main():
    print("数学计算能力训练程序")
    print("=" * 40)
    print("支持题型: 四则运算、一次方程、二次方程")
    print(f"加减法: {ADD_SUB_DIGITS}位数, 乘法: {MUL_DIGITS_A}×{MUL_DIGITS_B}位数, 除法: {DIV_DIGITS_A}÷{DIV_DIGITS_B}位数")
    print("除法结果有效数字: {DIV_PRECISION}位")
    print("分数输入格式: a/b (如 3/4)")
    print("二次方程答案格式: 根1 根2 (如 '2 3' 或 '1/2 3/4')")
    print("输入 'exit' 退出程序")
    print("=" * 40)
    
    # 错题记录
    mistakes: List[Tuple[str, str, str]] = []
    question_count = 0
    correct_count = 0
    
    while True:
        # 随机选择题型
        weights = [0.5, 0.25, 0.25]  # 四则运算占50%，方程各占25%
        problem_type = random.choices(
            ['arithmetic', 'linear', 'quadratic'], 
            weights=weights
        )[0]
        
        # 生成题目
        if problem_type == 'arithmetic':
            problem, answer = generate_arithmetic()
        elif problem_type == 'linear':
            problem, answer = generate_linear_equation()
        else:
            problem, answer = generate_quadratic_equation()
        
        question_count += 1
        
        # 显示问题
        print(f"\n题目 #{question_count}: {problem}")
        user_input = input("答案: ").strip()
        
        # 退出检查
        if user_input.lower() == 'exit':
            break
        
        # 验证答案
        is_correct = False
        if problem_type == 'arithmetic':
            is_correct = validate_arithmetic(user_input, answer)
        elif problem_type == 'linear':
            is_correct = validate_linear(user_input, answer)
        else:
            is_correct = validate_quadratic(user_input, answer)
        
        if is_correct:
            correct_count += 1
            print("✓ 正确！")
        else:
            correct_str = format_answer(answer)
            print(f"✗ 错误！正确答案: {correct_str}")
            mistakes.append((problem, correct_str, user_input))
    
    # 显示统计信息
    if question_count > 0:
        accuracy = correct_count / question_count * 100
    else:
        accuracy = 0.0
    
    print("\n" + "=" * 40)
    print(f"练习总结: 共 {question_count} 题, 正确 {correct_count} 题, 准确率 {accuracy:.1f}%")
    
    # 显示错题
    if mistakes:
        print("\n==== 错题总结 ====")
        for i, (problem, correct, user) in enumerate(mistakes, 1):
            print(f"{i}. 题目: {problem}")
            print(f"   你的答案: {user}")
            print(f"   正确答案: {correct}")
            print()
    
    print("练习结束！")

if __name__ == "__main__":
    main()