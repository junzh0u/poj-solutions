// POJ 1446 - Moscow Time
// Model: gpt-6.1-sol
// Convert the local clock to Moscow minutes, then carry whole days through
// the Gregorian calendar. Shift the supplied weekday, even if it disagrees
// with the calendar date. Two-character years mean 19xx; print four digits.
#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <cstdio>
using namespace std;
int days(int y, int m) {
    const int len[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    return len[m] + (m == 1 && y%4 == 0 && (y%100 != 0 || y%400 == 0));
}
int main() {
    const string week[] = {"MON,","TUE,","WED,","THU,","FRI,","SAT,","SUN,"};
    const string month[] = {"JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};
    string w, mon, year, clock, zone;
    int d;
    if (!(cin >> w >> d >> mon >> year >> clock >> zone)) return 0;
    int y = atoi(year.c_str()), m = 0, wd = 0, h, n, s;
    if (year.size() == 2) y += 1900;
    while (month[m] != mon) ++m;
    while (week[wd] != w) ++wd;
    sscanf(clock.c_str(), "%d:%d:%d", &h, &n, &s);
    int offset = 0;
    if (zone[0] == '+' || zone[0] == '-') {
        offset = ((zone[1]-'0')*10+zone[2]-'0')*60 + (zone[3]-'0')*10+zone[4]-'0';
        if (zone[0] == '-') offset = -offset;
    } else if (zone == "EDT") offset = -240;
    else if (zone == "CDT") offset = -300;
    else if (zone == "MDT") offset = -360;
    else if (zone == "PDT") offset = -420;
    int minutes = h*60+n+180-offset;
    while (minutes < 0) {
        minutes += 1440; wd = (wd+6)%7;
        if (--d == 0) {
            if (--m < 0) { m = 11; --y; }
            d = days(y,m);
        }
    }
    while (minutes >= 1440) {
        minutes -= 1440; wd = (wd+1)%7;
        if (++d > days(y,m)) {
            d = 1;
            if (++m == 12) { m = 0; ++y; }
        }
    }
    cout << week[wd] << ' ' << setfill('0') << setw(2) << d << ' '
         << month[m] << ' ' << setw(4) << y << ' ' << setw(2) << minutes/60
         << ':' << setw(2) << minutes%60 << ':' << setw(2) << s << " +0300\n";
    return 0;
}
