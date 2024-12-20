class Element:
    or_count: int
    and_count: int
    val: bool
    def __init__(self, val: bool) -> "Element":
        self.or_count = 0
        self.and_count = 0
        self.val = val
    
    def copy(self) -> "Element":
        res = Element(self.val)
        res.and_count = self.and_count
        res.or_count = self.or_count
        return res

    def __or__(self, other: "Element") -> "Element":
        res = self.copy()
        if self.val:
            res.or_count += 1
        else:
            res.or_count += other.or_count
            res.and_count += other.and_count
        res.val = self.val or other.val
        return res
    
    def __and__(self, other: "Element") -> "Element":
        res = self.copy()
        if not self.val:
            res.and_count += 1
        else:
            res.and_count += other.and_count
            res.or_count += other.or_count
        res.val = self.val and other.val
        return res

def getTrue() -> Element:
    return Element(True)

def getFalse() -> Element:
    return Element(False)

if __name__ == "__main__":
    expr = input()
    expr = expr.replace('1', "getTrue()").replace('0', "getFalse()")
    res: Element = eval(expr)

    print(int(res.val))
    print(res.and_count, res.or_count)