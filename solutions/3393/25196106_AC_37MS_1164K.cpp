// POJ 3393 - Lucky and Good Months by Gregorian Calendar
// Model: claude-fable-5
//
// Calendar as defined by the statement: years 1..9999; a year y < 1582 is leap
// iff y%4==0 (Julian); a year y >= 1582 is leap iff the Gregorian rule holds,
// with 1700 declared leap by fiat (1582 itself is not divisible by 4, so the
// boundary is moot).  Eleven days, September 3..13 of 1752, do not exist; the
// day-of-week sequence is continuous across the gap (Sep 2 1752 is Wednesday,
// Sep 14 Thursday - confirmed on the discuss board and by back-counting from
// the statement's anchor, Sep 29 2006 = Friday).
//
// A month is lucky iff its last working day (Mon-Fri) is Friday, good iff its
// first working day is Monday.  Iterate every existing day of years 1..9999
// (~3.65M days), tracking the weekday, record first/last working weekday per
// month, and build prefix sums over the linear month index; each query is two
// subtractions.  The weekday of Jan 1 year 1 is derived at startup by counting
// days (minus the 11 eliminated) up to the Friday anchor.
#include <stdio.h>

static int mdays[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

static int isLeap(int y) {
    if (y < 1582) return y % 4 == 0;
    if (y == 1700) return 1;
    return y % 4 == 0 && (y % 100 != 0 || y % 400 == 0);
}

/* month index 1..119988 */
static int luckyPre[120001];
static int goodPre[120001];

int main() {
    /* --- derive weekday of Jan 1, year 1 ---------------------------------- */
    /* count existing days from Jan 1 year 1 (inclusive) up to Sep 29 2006
       (exclusive); that date is Friday (=5, with Sunday=0). */
    long total = 0;
    int y, m, d;
    for (y = 1; y < 2006; y++) total += 365 + isLeap(y);
    for (m = 0; m < 8; m++) total += mdays[m] + (m == 1 && isLeap(2006));
    total += 29 - 1;
    total -= 11;                       /* the eliminated Sep 3..13, 1752 */
    int w0 = (int)((5 - total % 7 + 7) % 7);   /* weekday of Jan 1, year 1 */

    /* --- sweep every existing day, classify months ------------------------ */
    int w = w0;
    int idx = 0;
    int nLucky = 0, nGood = 0;
    for (y = 1; y <= 9999; y++) {
        for (m = 1; m <= 12; m++) {
            int dm = mdays[m-1] + (m == 2 && isLeap(y));
            int firstWork = -1, lastWork = -1;
            for (d = 1; d <= dm; d++) {
                if (y == 1752 && m == 9 && d >= 3 && d <= 13) continue;
                if (w >= 1 && w <= 5) {           /* Mon..Fri */
                    if (firstWork < 0) firstWork = w;
                    lastWork = w;
                }
                w = (w + 1) % 7;
            }
            idx++;
            if (lastWork == 5) nLucky++;
            if (firstWork == 1) nGood++;
            luckyPre[idx] = nLucky;
            goodPre[idx] = nGood;
        }
    }

    int t;
    if (scanf("%d", &t) != 1) return 0;
    while (t-- > 0) {
        int ys, ms, ye, me;
        scanf("%d %d %d %d", &ys, &ms, &ye, &me);
        int s = (ys - 1) * 12 + ms;
        int e = (ye - 1) * 12 + me;
        printf("%d %d\n", luckyPre[e] - luckyPre[s-1], goodPre[e] - goodPre[s-1]);
    }
    return 0;
}
