/**
 * @link https://www.luogu.com.cn/problem/P7075
 */

#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=(n);i++)
#define from(i,b,e) for(int i=(b);i<=(e);i++)
#define rev(i,e,b) for(int i=(e);i>=(b);i--)
#define main() main(int argc, char const *argv[])
#define optimizeIO std::ios::sync_with_stdio(false); std::cin.tie(0); std::cout.tie(0);
#define chkMax(base,cmp...) (base=std::max({(base),##cmp}))
#define chkMin(base,cmp...) (base=std::min({(base),##cmp}))
#define chkMaxEx(base,exchange,other,cmp...) {auto __b__=base;if(__b__!=chkMax(base,##cmp)){exchange;} else other;}
#define chkMinEx(base,exchange,other,cmp...) {auto __b__=base;if(__b__!=chkMin(base,##cmp)){exchange;} else other;}
#define ensure(v, con, otw) (((v) con)? (v): (otw))
#define never if constexpr(0)
#define always if constexpr(1)
#define bitOr(x,y) (((x)&(y))^(((x)^(y))|(~(x)&(y))))
#define Infinity 2147483647
#define compare(x,y,g,e,l) (((x)>(y))?(g):(((x)<(y))?(l):(e)))
bool DEBUG_MODE=false;
typedef long long ll; typedef unsigned long long ull;
inline void batchOutput(int *begin, int n, const char *format){upto(i, n)printf(format, begin[i]);printf("\n");} inline void batchOutput(int*begin, int n) {batchOutput(begin,n,"%3d ");}
#define batchOutput2d(b, r, c, fmt) upto(i,r){upto(j,c)printf(fmt,b[i][j]);printf("\n");}
namespace lib{}
#include <bits/stdc++.h>
namespace lib{
	template <const size_t MAXSIZE>
	struct IO {
		char buf[MAXSIZE],*p1,*p2;
		char pbuf[MAXSIZE],*pp;
		inline char gc() {
			if (p1==p2) p2=(p1=buf)+fread(buf,1,MAXSIZE,stdin);
			return p1==p2?' ':*p1++;
		}
        // inline char gc() {
		// 	return getchar();
        // }
		inline bool blank(char ch) { return ch==' ' or ch=='\n' or ch=='\r' or ch=='\t'; }
		inline bool isd(char x) {return (x>='0' and x<='9');}
        std::string input() {
            char c = gc();
            std::string res="";
            while((c&&!blank(c)) || c==' ') {
                res.push_back(c);
                c = gc();
            }
            return res;
        }
		template <class T>
		inline void scan(T &x) {
			double tmp=1;
			bool sign=0;
			x=0;
			char ch=gc();
			for (; not isd(ch); ch=gc()) if (ch=='-') sign=1;
			for (; isd(ch); ch=gc()) x=x*10+(ch^48);
			if (ch=='.') for (ch=gc(); isd(ch); ch=gc()) tmp*=.1f,x+=tmp*(ch^48);
			if (sign) x=-x;
		}
		inline void scan(char *s) {
			char ch=gc();
			for (; blank(ch); ch=gc());
			for (; not blank(ch); ch=gc()) *s++=ch;
			*s=0;
		}
		inline void scan(char &c) {
			for (c=gc(); blank(c); c=gc());
		}
		inline void scan(std::string &s){
			std::string().swap(s);
			char ch=gc();
			for (; blank(ch); ch=gc());
			for (; not blank(ch); ch=gc()) s.push_back(ch);
		}
		template <class T,class... Types>
		inline void scan(T &x,Types &...args){
			scan(x);
			scan(args...);
		}
		inline void push(const char &c) {
			// if (pp-pbuf==MAXSIZE) fwrite(pbuf,1,MAXSIZE,stdout),pp=pbuf;
			// *pp++=c;
            putchar(c);
		}
		template <class T>
		inline void write(T x) {
			if (x<0) x=-x,push('-');
			static char sta[40];
			int top=0;
			do {
				sta[top++]=x%10^48,x/=10;
			} while (x);
			while (top) push(sta[--top]);
		}
        inline void write(double x) {
            printf("%lf",x);
        }
        inline void write(float x) {
            printf("%f",x);
        }
        inline void write(char c) {
            push(c);
        }
		inline void write(std::string &s){
			for (auto i:s) push(i);
		}
        inline void write(const char *s){
			for (; *s; ++s) push(*s);
        }
		template <class T,class... Types>
		inline void write(T &x,Types &...args){
			write(x);
			write(args...);
		}

        template<class T=int>
        inline T read() {
            T x;
            scan(x);
            return x;
        }

        // 流式输入输出
		template <class T>
        inline IO& operator>>(T&x) {
            scan(x);
            return *this;
        }
        template <class T>
        inline IO& operator<<(T&x) {
            write(x);
            return *this;
        }
	};
	IO<1048576> io;
    const std::string endl = "\n";
}
using namespace lib;

;
namespace Solution {

    int Q;

    const int AverageYearsMonth_negative[] = {-365, -334, -304, -273, -243, -212, -181, -151, -120, -90, -59, -31, 0};
    const int LeapYearsMonth_negative[] = {-366, -335, -305, -274, -244, -213, -182, -152, -121, -91, -60, -31, 0};
    
    bool isLeap(int year, bool usingGregorian=true) {
        if (year<0)  year++;
        if (not usingGregorian)
            return (year%4 == 0);
        else
            return (year%400 == 0) or (year%100!=0 and year%4==0);
    }

    void init() {

    }

    // 计算年内的某个日期
    // 传入：从某年的 1/1 到目标日期的天数，包含两个端点
    // 返回：月，日
    std::pair<int, int> calculateDateInYear(ll days, bool leap=false) {
        const int *YearMonth = leap? LeapYearsMonth_negative: AverageYearsMonth_negative;
        auto monthToDecember = std::upper_bound(YearMonth, YearMonth+13, -days) - YearMonth;  // 和 12 月的差距
        auto month = 13 - monthToDecember;  // 月份标号
        
        auto preventMonthPassedDays = -YearMonth[monthToDecember];  // 先前的月份经过的天数
        auto currentDay = days - preventMonthPassedDays;  // 本月经过的天数

        return {month, currentDay};
    }

    void BC(ll julian) {
        auto daysNegativeCount = 1721424 - julian;  // 到 1/1/1 的天数
        auto termsCount = daysNegativeCount / 1461;  // 周期（4 年）数量
        auto yearAbs = termsCount * 4;  // 年份的绝对值
        daysNegativeCount %= 1461;  // 向前拨回若干周期后剩余的天数；到某年的 1 月 1 日，该年份绝对值是 4 的倍数
        auto newDays = daysNegativeCount;  // 再拨回一年，这一年一定是一个闰年
        while (newDays > 0) {
            newDays = newDays - 365 - isLeap(-yearAbs-1, false);
            yearAbs++;
        }

        int dayIndex = 0 - newDays;  // 目标日期到那年 1 月 1 日的天数
        dayIndex++;  // 包含那年的一月一日

        auto [month, day] = calculateDateInYear(dayIndex, isLeap(-yearAbs, false));
        printf("%d %d %lld BC\n", day, month, yearAbs);
    }

    void DC_WithJulianCalender(ll julian) {
        auto daysCount = julian - 1721424;  // 到 1/1/1 的天数
        auto termsCount = daysCount / 1461;  // 四年周期
        auto years = termsCount * 4 + 1;  // 此时是这一年的 1/1
        daysCount %= 1461;
        auto newDays = daysCount + 1;
        // 向后拨若干年，直到这个数大于等于 0 时的最小值
        while (newDays - 365 - isLeap(years, false) > 0) {
            newDays = newDays - 365 - isLeap(years, false);
            years++;
        }
        auto [month, day] = calculateDateInYear(newDays, isLeap(years, false));
        printf("%d %d %lld\n", day, month, years);
    }

    void DC_WithGregorianCalender(ll julian) {
        // auto daysFromBreaking = 2299160;
        const auto newYearJulianDay = 2299249-10;  // 1583/1/1 的儒略日
        if (julian < newYearJulianDay) {
            DC_WithJulianCalender(julian+10);
            return;
        }
        auto daysAfterNewYear = julian - newYearJulianDay + 1;  // 从 1583/1/1 开始
        // 先暴力推 17 年
        auto year = 1583LL;  // 当前处于 year/1/1
        while (daysAfterNewYear - 365 - isLeap(year) > 0) {
            daysAfterNewYear = daysAfterNewYear - 365 - isLeap(year);
            year++;
            if (year == 1601)  break;
        }
        if (year > 1600) {
            daysAfterNewYear--;  // ! 不包含 1 月 1 日
            // 对于一个 400 年的大周期
            const auto bigTermLength = 146097;
            auto bigTermCount = daysAfterNewYear / bigTermLength;
            daysAfterNewYear %= bigTermLength;
            year += bigTermCount * 400; assert(year%400==1);

            // 直接特判 400 年的倍数
            if (daysAfterNewYear >= bigTermLength - 366) {
                year += 399;
                auto [month, day] = calculateDateInYear(daysAfterNewYear - bigTermLength + 366 + 1, 1);
                printf("%d %d %lld\n", day, month, year);
                return;
            }

            // 100 年的中等周期
            const auto midTermLength = 36524;
            auto midTermCount = daysAfterNewYear / midTermLength;
            daysAfterNewYear %= midTermLength;
            year += midTermCount * 100;

            // 特判 100 的倍数
            if (daysAfterNewYear >= midTermLength - 365) {
                year += 99;
                auto [month, day] = calculateDateInYear(daysAfterNewYear - midTermLength + 365 + 1, 0);
                printf("%d %d %lld\n", day, month, year);
                return;
            }

            // 4 年
            const auto littleTermLength = 1461;
            auto littleTermCount = daysAfterNewYear / littleTermLength;
            daysAfterNewYear %= littleTermLength;
            year += littleTermCount * 4;
            while (daysAfterNewYear - 365 - isLeap(year) >= 0) {
                daysAfterNewYear = daysAfterNewYear - 365 - isLeap(year);
                year++;
            }
            daysAfterNewYear++;
            auto [month, day] = calculateDateInYear(daysAfterNewYear, isLeap(year));
            printf("%d %d %lld\n", day, month, year);
        } else {
            auto [month, day] = calculateDateInYear(daysAfterNewYear, isLeap(year));
            printf("%d %d %lld\n", day, month, year);
        }
    }

    void solve() {
        init();
        io >> Q;
        ll x;

        // auto test_ret = calculateDateInYear(15, true);
        // log("%d %d\n", test_ret.first, test_ret.second);
        from(_, 1, Q) {
            io >> x;
            
            // 1/1/1 BC 及以前
            if (x < 1721424) {
                BC(x);
            } else if (x <= 2299160LL) {
                DC_WithJulianCalender(x);
            } else {
                DC_WithGregorianCalender(x);
            }
        }
    }
}


int main() {
    initDebug;
    debug {
        freopen("julian3.in", "r", stdin);
        freopen("julian3.out", "w", stdout);
    }
    Solution::solve();
    return 0;
}