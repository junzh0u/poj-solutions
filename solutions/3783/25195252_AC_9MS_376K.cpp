// POJ 3783 - Balls
// Model: claude-opus-5
//
// Classic two-glass-balls (egg drop) worst-case problem: with B balls and an
// M-story building, find the minimum number of drops d that suffices in the
// worst case.
//
// Approach: invert the objective. Let cover[b][d] be the largest number of
// floors that can be resolved with b balls and d drops. Dropping the first of
// the b balls at the right floor splits the range into
//   - cover[b-1][d-1] floors below it (ball broke, one ball spent), plus
//   - the floor dropped from itself, plus
//   - cover[b][d-1] floors above it (ball survived),
// so cover[b][d] = cover[b-1][d-1] + cover[b][d-1] + 1, with cover[b][0] = 0
// and cover[0][d] = 0. The answer is the least d with cover[B][d] >= M.
// cover grows like 2^d, so it is clamped at MAXM (1000); the clamp is safe
// because every value at or above MAXM is equally "enough" and the recurrence
// is monotone in its inputs. Everything stays in int.
//
// Table is precomputed once for B <= 50, d <= 1000 (d = M = 1000 is the worst
// case, reached only at B = 1), then each query is a linear scan. No recursion.
//
// Note on the input format: each data set line begins with a "problem number"
// that is echoed verbatim as the data set number in the output; it is not
// necessarily the line's index.

#include <cstdio>

static const int MAXB = 50;
static const int MAXM = 1000;

static int cover[MAXB + 1][MAXM + 1];

int main() {
    int b, d;
    for (b = 0; b <= MAXB; ++b) cover[b][0] = 0;
    for (d = 0; d <= MAXM; ++d) cover[0][d] = 0;
    for (b = 1; b <= MAXB; ++b) {
        for (d = 1; d <= MAXM; ++d) {
            int v = cover[b - 1][d - 1] + cover[b][d - 1] + 1;
            if (v > MAXM) v = MAXM;
            cover[b][d] = v;
        }
    }

    int p;
    if (scanf("%d", &p) != 1) return 0;
    while (p-- > 0) {
        int id, balls, floors;
        if (scanf("%d %d %d", &id, &balls, &floors) != 3) break;
        int ans = 0;
        for (d = 0; d <= MAXM; ++d) {
            if (cover[balls][d] >= floors) { ans = d; break; }
        }
        printf("%d %d\n", id, ans);
    }
    return 0;
}
