// POJ 1367 - Time
// Model: claude-sonnet-5
// Approach: for a "year"/"month" period, unit length varies (leap years,
// month lengths), so count via absolute year/month indices instead of
// seconds. For "day"/"hour"/"minute"/"second" periods, unit length is
// fixed, so convert both timestamps to total seconds since 1970-01-01
// (a plain proleptic-Gregorian day count, leap rule: /4 except /100
// except /400). In both cases: find the earliest boundary A >= D1 that is
// a legal start for the given unit (e.g. an hour period must start at
// minute=second=0; a year period at month=day=1, time=0), then the answer
// is floor((D2 - A) / (n * unit)) using integer index/second arithmetic,
// clamped to 0 if negative.
// Statement ambiguity: the discuss board (msg 41390) confirms the
// "60 minute" vs "1 hour" sample pair (11 vs 10, same 11h span) is exactly
// this alignment rule -- an hour period must start on the hour, while the
// given start 20:30:00 is already minute-aligned so it counts as its own
// 60-minute-period start. Verified against all 5 sample lines plus this
// reasoning. Board also flags y=1970 as a value some naive year-count
// formulas mishandle; the day-count loop here is a no-op for y==1970 and
// handles it correctly.
#include <iostream>
#include <string>
using namespace std;

static int mdays[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

static bool isLeap(long long y) {
    return (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0));
}

static long long daysBeforeYear(long long y) {
    long long days = 0;
    if (y >= 1970) {
        for (long long yy = 1970; yy < y; yy++) days += isLeap(yy) ? 366 : 365;
    } else {
        for (long long yy = y; yy < 1970; yy++) days -= isLeap(yy) ? 366 : 365;
    }
    return days;
}

static long long daysBeforeMonth(long long y, long long m) {
    long long days = 0;
    for (long long mm = 1; mm < m; mm++) {
        days += mdays[mm - 1];
        if (mm == 2 && isLeap(y)) days += 1;
    }
    return days;
}

static long long totalSeconds(long long y, long long m, long long d, long long h, long long mi, long long s) {
    long long days = daysBeforeYear(y) + daysBeforeMonth(y, m) + (d - 1);
    return days * 86400LL + h * 3600LL + mi * 60LL + s;
}

int main() {
    long long y1, m1, d1, h1, mi1, s1;
    long long y2, m2, d2, h2, mi2, s2;
    long long n;
    string unit;
    while (cin >> y1 >> m1 >> d1 >> h1 >> mi1 >> s1) {
        cin >> y2 >> m2 >> d2 >> h2 >> mi2 >> s2;
        cin >> n >> unit;
        long long answer = 0;
        char c0 = unit[0];
        char c1 = unit.size() > 1 ? unit[1] : ' ';
        if (c0 == 'y') {
            bool aligned = (m1 == 1 && d1 == 1 && h1 == 0 && mi1 == 0 && s1 == 0);
            long long YA = aligned ? y1 : y1 + 1;
            long long diff = y2 - YA;
            answer = diff >= 0 ? diff / n : 0;
        } else if (c0 == 'm' && c1 == 'o') {
            long long M1 = (y1 - 1970) * 12 + (m1 - 1);
            bool aligned = (d1 == 1 && h1 == 0 && mi1 == 0 && s1 == 0);
            long long MA = aligned ? M1 : M1 + 1;
            long long M2 = (y2 - 1970) * 12 + (m2 - 1);
            long long diff = M2 - MA;
            answer = diff >= 0 ? diff / n : 0;
        } else {
            long long gran;
            if (c0 == 'd') gran = 86400LL;
            else if (c0 == 'h') gran = 3600LL;
            else if (c0 == 'm' && c1 == 'i') gran = 60LL;
            else gran = 1LL;
            long long T1 = totalSeconds(y1, m1, d1, h1, mi1, s1);
            long long T2 = totalSeconds(y2, m2, d2, h2, mi2, s2);
            long long rem = T1 % gran;
            long long TA = (rem == 0) ? T1 : T1 + (gran - rem);
            long long diff = T2 - TA;
            long long period = n * gran;
            answer = diff >= 0 ? diff / period : 0;
        }
        cout << answer << "\n";
    }
    return 0;
}
