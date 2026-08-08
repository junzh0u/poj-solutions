// POJ 3232 - Accelerator
// Model: claude-opus-5
//
// N riders have A_i meters left and cover 1 m per time unit.  M identical
// accelerators exist; in each time unit an accelerator may be handed to a
// rider, who then covers k m that unit instead of 1.  A rider holds at most
// one accelerator per time unit and the assignment is redecided every unit.
// Minimize the time until every rider has finished.
//
// Binary search the answer T.  In T units a rider given x_i accelerated units
// covers T + x_i*(k-1) meters, so it needs x_i = ceil((A_i - T)/(k - 1)) when
// A_i > T, and 0 otherwise.  Such a schedule exists iff
//     (a) x_i <= T for every rider, and
//     (b) sum x_i <= M*T.
// Both are clearly necessary.  They are sufficient by a wrap-around filling of
// the M x T grid of (accelerator, time unit) slots: list each rider's x_i units
// consecutively and fill the grid row by row.  A rider's block is contiguous of
// length x_i <= T, and two slots share a column only if their positions differ
// by a multiple of T, which needs a block of length > T.  So no rider ever
// holds two accelerators in one time unit, and (b) says the units fit.
//
// Pitfalls, all three named on the discuss board and all three verified here:
//   * k == 1 makes an accelerator useless (and would divide by zero); the
//     answer is then max A_i.  Same for M == 0.
//   * M*T overflows 32 bits (up to 1e16), so the budget and the running sum
//     are 64-bit.  This is why the submission is under G++ rather than C++.
//   * the per-rider cap x_i <= T is a real constraint, not implied by (b).
// The official sample does not distinguish any of the three, nor does it
// distinguish "rides k meters" from "rides k+1 meters"; an exhaustive sweep of
// every case with n <= 5 and small A/M/k against a direct BFS simulation does.
//
// Input framing: exactly the leading count T of test cases is consumed, and
// anything after the T-th case is ignored.  A first submission that instead
// read cases to EOF was judged Wrong Answer while agreeing with this one on
// every generated case, which pins the difference on trailing input rather
// than on the algorithm.  Input runs to ~17 MB, which the board reports as
// enough to TLE on scanf alone, so reading goes through a single fread.

#include <cstdio>
#include <cstdlib>
#include <algorithm>

static char *buf;
static size_t blen, bpos;

static void readAll() {
    size_t cap = 1 << 22;
    buf = (char *)malloc(cap);
    blen = 0;
    for (;;) {
        if (blen == cap) { cap <<= 1; buf = (char *)realloc(buf, cap); }
        size_t got = fread(buf + blen, 1, cap - blen, stdin);
        if (got == 0) break;
        blen += got;
    }
    bpos = 0;
}

static bool readInt(int &out) {
    while (bpos < blen && (buf[bpos] < '0' || buf[bpos] > '9') && buf[bpos] != '-') ++bpos;
    if (bpos >= blen) return false;
    bool neg = false;
    if (buf[bpos] == '-') { neg = true; ++bpos; }
    long long v = 0;
    while (bpos < blen && buf[bpos] >= '0' && buf[bpos] <= '9') { v = v * 10 + (buf[bpos] - '0'); ++bpos; }
    out = (int)(neg ? -v : v);
    return true;
}

static int *a = 0;
static int acap = 0;
static int n, m, k;

static bool cmpDesc(int x, int y) { return x > y; }

static bool feasible(long long T) {
    long long budget = T * (long long)m;
    long long used = 0;
    unsigned int km1 = (unsigned int)(k - 1);
    for (int i = 0; i < n; ++i) {
        if ((long long)a[i] <= T) break;          /* a[] is sorted descending */
        unsigned int need = (unsigned int)((long long)a[i] - T);
        unsigned int x = need / km1;
        if (need % km1) ++x;                      /* ceil, without overflowing */
        if ((long long)x > T) return false;       /* one accelerator per unit */
        used += x;
        if (used > budget) return false;
    }
    return used <= budget;
}

int main() {
    readAll();
    int t;
    if (!readInt(t)) return 0;
    while (t-- > 0) {
        if (!readInt(n)) break;                   /* truncated input */
        if (n < 0) break;
        if (n > acap) {
            acap = n + 16;
            a = (int *)realloc(a, (size_t)acap * sizeof(int));
        }
        for (int i = 0; i < n; ++i) {
            if (!readInt(a[i])) a[i] = 0;
        }
        if (!readInt(m)) m = 0;
        if (!readInt(k)) k = 1;
        if (n <= 0) { printf("0\n"); continue; }

        std::sort(a, a + n, cmpDesc);
        long long maxA = a[0];

        if (k <= 1 || m <= 0) {                   /* accelerator cannot help */
            printf("%d\n", (int)maxA);
            continue;
        }

        long long lo = 0, hi = maxA;              /* hi is always feasible */
        while (lo < hi) {
            long long mid = lo + (hi - lo) / 2;
            if (feasible(mid)) hi = mid; else lo = mid + 1;
        }
        printf("%d\n", (int)lo);
    }
    return 0;
}
