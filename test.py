s = input()
print("Yes" if s.lower() != s and s.upper() != s and len(s) == len(set(s)) else "No")
