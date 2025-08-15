import sys

def is_julian_leap(year):
    if year <= 0:
        year = -year + 1
    return year % 4 == 0

def is_gregorian_leap(year):
    if year % 400 == 0:
        return True
    if year % 100 == 0:
        return False
    return year % 4 == 0

def solve():
    Q = int(sys.stdin.readline())
    queries = [int(sys.stdin.readline()) for _ in range(Q)]
    
    for r in queries:
        day = r + 1  # since Julian Day starts from day 1 (4713 BC Jan 1 is 0 days)
        
        # Phase 1: Before 1582 Oct 15 (Julian calendar)
        if day <= 2299160:  # 1582 Oct 4 Julian is 2299160 days from 4713 BC Jan 1
            # Julian calendar handling
            # Calculate years in 4-year cycles for efficiency
            temp_day = day
            # The 4-year cycle has 365*4 +1 =1461 days
            cycles_4 = (temp_day - 1) // 1461
            remaining_days = (temp_day - 1) % 1461
            year = 4713 - 4 * cycles_4
            # Now find the exact year in the 4-year block
            for _ in range(4):
                leap = is_julian_leap(year)
                days_in_year = 366 if leap else 365
                if remaining_days < days_in_year:
                    break
                remaining_days -= days_in_year
                year -= 1
                if year == 0:
                    year = -1
            # Now find month and day
            month_days = [
                31, 29 if is_julian_leap(year) else 28, 31, 30, 31, 30,
                31, 31, 30, 31, 30, 31
            ]
            month = 1
            for m in range(12):
                md = month_days[m]
                if remaining_days < md:
                    day_in_month = remaining_days + 1
                    month = m + 1
                    break
                remaining_days -= md
            # Output
            if year < 1:
                print(f"{day_in_month} {month} {-year + 1} BC")
            else:
                print(f"{day_in_month} {month} {year}")
        else:
            # Gregorian calendar handling
            # Adjust for the 10-day gap: 1582-10-5 to 1582-10-14 don't exist
            if day >= 2299161:
                day -= 10  # days after 2299160 (1582-10-4) add 10 days to skip the gap
            # Now proceed with Gregorian calendar calculations
            # Start from 1 Jan 1 AD (proleptic Gregorian)
            # Days from 4713 BC Jan 1 to 1 AD Jan 1 is 1721424
            # So day starts from 1 AD Jan 1 is day - 1721424
            temp_day = day - 1721424
            # Now find the year
            year = 1
            # Use 400-year cycles for efficiency (each has 400*365 + 97 = 146097 days)
            cycles_400 = temp_day // 146097
            remaining_days = temp_day % 146097
            year += 400 * cycles_400
            # Find the exact year in the 400-year block
            found = False
            for _ in range(400):
                leap = is_gregorian_leap(year)
                days_in_year = 366 if leap else 365
                if remaining_days < days_in_year:
                    found = True
                    break
                remaining_days -= days_in_year
                year += 1
            if not found:
                year += 399
                remaining_days = 0
            # Now find month and day
            month_days = [
                31, 29 if is_gregorian_leap(year) else 28, 31, 30, 31, 30,
                31, 31, 30, 31, 30, 31
            ]
            month = 1
            for m in range(12):
                md = month_days[m]
                if remaining_days < md:
                    day_in_month = remaining_days + 1
                    month = m + 1
                    break
                remaining_days -= md
            print(f"{day_in_month} {month} {year}")

solve()