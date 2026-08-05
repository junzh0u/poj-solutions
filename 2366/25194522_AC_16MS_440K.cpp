// POJ 2366 - Sacrament of the sum
//
// Two lists are given, one after the other (first ascending, second
// descending). Decide whether one number from the FIRST list and one from
// the SECOND add up to 10000.
//
// The two summands come from different lists, so there is no "same element
// used twice" subtlety: a value v in list 1 and 10000-v in list 2 is a valid
// pair even when v == 5000, while 5000 appearing twice inside a single list
// proves nothing. Duplicates within a list are therefore irrelevant.
//
// Values fit in [-32768, 32767], so a direct-address table beats sorting:
// stamp every value of list 1 into an array indexed by value+40000, then scan
// list 2 and look up 10000-v. O(N1+N2) time, one 100001-int table (~400K,
// well inside the 65536K limit), no sorting and no reliance on the stated
// monotonicity of the inputs.
//
// The stamp holds a test-case counter rather than a boolean so the table is
// reused without clearing; the outer while-loop makes the program tolerate an
// input file holding more than one test case, which the statement leaves open.
// scanf throughout, as the problem's hint demands.

#include <cstdio>

static const int OFF = 40000;
static const int SIZE = 100001; /* values -40000..60000 covered generously */
static int stamp[SIZE];

int main() {
    int n1, n2, i, v, tc = 0;
    while (scanf("%d", &n1) == 1) {
        ++tc;
        for (i = 0; i < n1; ++i) {
            if (scanf("%d", &v) != 1) v = 0;
            int idx = v + OFF;
            if (idx >= 0 && idx < SIZE) stamp[idx] = tc;
        }
        if (scanf("%d", &n2) != 1) n2 = 0;
        int found = 0;
        for (i = 0; i < n2; ++i) {
            if (scanf("%d", &v) != 1) v = 0;
            long need = 10000L - v;
            long idx = need + OFF;
            if (idx >= 0 && idx < SIZE && stamp[idx] == tc) found = 1;
        }
        printf(found ? "YES\n" : "NO\n");
    }
    return 0;
}
