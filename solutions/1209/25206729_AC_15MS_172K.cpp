// POJ 1209 - Calendar
// Model: claude-sonnet-5
// Approach: for each query date, scan every anniversary and compute the
// forward distance (mod the year's total day count) from the query date to
// the anniversary's date. Include it if that distance is 0 (happening today)
// or between 1 and the anniversary's P (its requested advance-warning
// window). A future anniversary `diff` days ahead gets P-diff+1 stars
// (tomorrow -> P stars, ..., P days ahead -> 1 star), matching the sample.
// Leap years are exactly the years divisible by 4 (the stated range
// 1901-1999 has no century exception), and the year's day count is fixed by
// the single given year, so a "D 31 12" query can legitimately pull in a
// "next year" anniversary near day 1 via the modulo wraparound.
// Output: "Today is:" followed directly by the day and month each printed
// in a width-3 field (confirmed on the discuss board: single-digit dates
// still take 3 columns, e.g. day 1 -> "  1", not "Today is: 1 1"). Each
// event line is day and month in width-3 fields, then the star token (or
// *TODAY*) left-justified in an 8-column field, then the description with
// its leading input spaces stripped (the board flags this as a common bug).
// Exactly one blank line separates consecutive blocks; none trails the last
// one (also flagged repeatedly on the board as a PE cause).
// Sort within a block, confirmed by the board's clarifying thread
// (message 12691): primary key is the date (the forward diff); for events
// landing on a day that is "today" order is purely input order (they are
// defined to share the same importance); for a non-today day, order by
// descending star count first, then by input order.

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

int monthLen[13];

void buildMonthLen(int year) {
    static const int normal[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    for (int i = 1; i <= 12; i++) monthLen[i] = normal[i];
    if (year % 4 == 0) monthLen[2] = 29;
}

int dayOfYear(int d, int m) {
    int total = d;
    for (int i = 1; i < m; i++) total += monthLen[i];
    return total;
}

int totalDaysInYear() {
    int t = 0;
    for (int i = 1; i <= 12; i++) t += monthLen[i];
    return t;
}

struct Anniversary {
    int day, month, p;
    string desc;
    int order;
};

struct EventOut {
    int day, month;
    int diff;
    int p;
    int order;
    string desc;
    bool today;
};

bool cmpEvent(const EventOut &a, const EventOut &b) {
    if (a.diff != b.diff) return a.diff < b.diff;
    if (a.today && b.today) return a.order < b.order;
    int sa = a.p - a.diff + 1;
    int sb = b.p - b.diff + 1;
    if (sa != sb) return sa > sb;
    return a.order < b.order;
}

static string rstrip(const string &s) {
    size_t end = s.size();
    while (end > 0 && (s[end-1] == '\r' || s[end-1] == '\n')) end--;
    return s.substr(0, end);
}

int main() {
    int year;
    if (scanf("%d", &year) != 1) return 0;
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}

    buildMonthLen(year);
    int total = totalDaysInYear();

    vector<Anniversary> anns;
    string line;
    bool firstBlock = true;

    while (getline(cin, line)) {
        line = rstrip(line);
        size_t pos = line.find_first_not_of(" \t");
        if (pos == string::npos) continue;
        char type = line[pos];
        if (type == '#') break;

        if (type == 'A') {
            int d, m, p, n;
            sscanf(line.c_str() + pos + 1, "%d %d %d%n", &d, &m, &p, &n);
            string rest = line.substr(pos + 1 + n);
            size_t rpos = rest.find_first_not_of(" \t");
            rest = (rpos == string::npos) ? "" : rest.substr(rpos);
            Anniversary a;
            a.day = d; a.month = m; a.p = p; a.desc = rest; a.order = (int)anns.size();
            anns.push_back(a);
        } else if (type == 'D') {
            int d, m;
            sscanf(line.c_str() + pos + 1, "%d %d", &d, &m);

            if (!firstBlock) printf("\n");
            firstBlock = false;
            printf("Today is:%3d%3d\n", d, m);

            int qdoy = dayOfYear(d, m);
            vector<EventOut> outs;
            for (size_t i = 0; i < anns.size(); i++) {
                int edoy = dayOfYear(anns[i].day, anns[i].month);
                int diff = edoy - qdoy;
                diff = ((diff % total) + total) % total;
                EventOut eo;
                eo.day = anns[i].day; eo.month = anns[i].month; eo.diff = diff;
                eo.p = anns[i].p; eo.order = anns[i].order; eo.desc = anns[i].desc;
                if (diff == 0) {
                    eo.today = true;
                    outs.push_back(eo);
                } else if (diff <= anns[i].p) {
                    eo.today = false;
                    outs.push_back(eo);
                }
            }
            sort(outs.begin(), outs.end(), cmpEvent);

            for (size_t i = 0; i < outs.size(); i++) {
                string star;
                if (outs[i].today) star = "*TODAY*";
                else star = string(outs[i].p - outs[i].diff + 1, '*');
                printf("%3d%3d %-8s%s\n", outs[i].day, outs[i].month, star.c_str(), outs[i].desc.c_str());
            }
        }
    }
    return 0;
}
