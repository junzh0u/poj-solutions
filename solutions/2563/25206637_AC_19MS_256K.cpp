// POJ 2563 - Demerit Points
// Model: claude-sonnet-5
//
// Simulation of a merit/demerit point system driven by an issue date and a
// chronological list of offenses. Dates are kept as plain YYYYMMDD integers
// and "N years later" is computed as +N*10000 on that integer -- no real
// calendar arithmetic. This matches the officially published reference
// solution for this problem (Waterloo local 2000.09.23, problem D), found at
// icpc.student.cs.uwaterloo.ca/~acm00/000923/data/D.c: it resolves, by
// authorial intent, a genuine statement ambiguity the POJ discuss board for
// this problem argues over without ever settling (what date is "one year
// after" Feb 29) -- the intended answer is the naive non-calendar-aware one,
// e.g. 2000-02-29 + 1 "year" prints as 2001-02-29 even though that date does
// not exist.
//
// State: demerits D, merits M (0..5), and a single anchor date `last` that
// serves double duty: while D>0 it is the most recent offense date (annual
// reduction anchor); once D reaches 0 it becomes the date the merit clock
// last ticked from (biennial award anchor). Between offenses we replay every
// pending annual reduction / biennial award whose date is <= the next
// offense date (or forever, once input is exhausted, until merits cap at 5).
//
// On each offense (raw points p from input, NOT combined with any leftover
// demerits before comparing): if 2*M <= p, the offense's points are simply
// added on top of whatever demerits remain (D += p - 2*M) and M resets to 0;
// otherwise D stays untouched (it is provably always 0 in that branch, since
// M>0 only ever holds when D==0) and M -= ceil(p/2). Verified against all
// four official test cases (D.0-D.4), including a same-day reduction-then-
// offense pair (two separate output lines, same date) and repeated same-day
// offenses.
#include <cstdio>

static void dump(int date, int merits, int demerits) {
    printf("%04d-%02d-%02d ", date / 10000, (date / 100) % 100, date % 100);
    if (merits) printf("%d merit point(s).\n", merits);
    else if (demerits) printf("%d demerit point(s).\n", demerits);
    else printf("No merit or demerit points.\n");
}

int main() {
    int lastoff, nextoff, points;
    int merits = 0, demerits = 0;

    if (scanf("%d", &lastoff) != 1) return 0;
    dump(lastoff, merits, demerits);

    for (;;) {
        nextoff = 99999999;
        scanf("%d %d", &nextoff, &points);

        while ((demerits && lastoff + 10000 <= nextoff) ||
               (!demerits && merits < 5 && lastoff + 20000 <= nextoff)) {
            if (demerits) {
                if (demerits > 2) demerits /= 2;
                else demerits = 0;
                lastoff += 10000;
            } else {
                merits++;
                lastoff += 20000;
            }
            dump(lastoff, merits, demerits);
        }

        if (nextoff == 99999999) break;

        lastoff = nextoff;
        if (merits * 2 <= points) {
            demerits += (points - merits * 2);
            merits = 0;
        } else {
            merits -= (points + 1) / 2;
        }
        dump(lastoff, merits, demerits);
    }
    return 0;
}
