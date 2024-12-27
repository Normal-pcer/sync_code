import copy

class Time:
    hour: int
    minute: int

    def __init__(self, minute: int, second: int):
        self.hour = minute
        self.minute = second

    @classmethod
    def parse(cls, text: str):
        min_s, sec_s = text.split(':')
        return cls(int(min_s), int(sec_s))
    
    def toMinute(self) -> int:
        return self.hour * 60 + self.minute

class Bus:
    begin: Time
    end: Time

    def __init__(self, begin: Time, end: Time):
        self.begin = begin
        self.end = end

if __name__ == "__main__":
    N = int(input())
    lis0 = list[Bus]()
    lis1 = list[Bus]()
    for _ in range(N):
        tag, time_pair = input().split()
        part = (tag != "Zagreb-Graz")
        begin_s, end_s = time_pair.split("--")
        begin, end = Time.parse(begin_s), Time.parse(end_s)
        if end.toMinute() <= begin.toMinute():
            end.hour += 24
        (lis1 if part else lis0).append(Bus(begin, end))
    
    ans = 0x3f3f3f3f3f3f3f3f
    for first_ in lis0:
        for second_ in lis1:
            first, second = copy.copy(first_), copy.copy(second_)
            while first.end.toMinute() >= second.begin.toMinute():
                second.begin.hour += 24  # Day += 1
                second.end.hour += 24
            cur = second.end.toMinute() - first.begin.toMinute()
            ans = min(ans, cur)
    
    if ans == 0x3f3f3f3f3f3f3f3f:
        print("NEMOGUCE")
    else:
        ans += 1
        ans_hour = ans // 60
        ans_minute = ans % 60
        print(f"{ans_hour}:{ans_minute}")