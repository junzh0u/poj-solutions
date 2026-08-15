// POJ 2737 - Swamp Things
// Model: claude-sonnet-5
//
// Classic maximum-points-on-a-line. For each anchor point i, compute the
// direction (normalized reduced (dx,dy) with a canonical sign) to every
// LATER point j > i only, sort those directions, and the largest run of
// equal directions plus 1 (for point i itself) is the size of a line
// passing through i and consisting of i plus some points to its right in
// input order. This is exact and complete: for any collinear set, the
// point among them with the smallest index acts as the anchor that finds
// the whole set (all others have larger index), so scanning j>i only
// (instead of all j != i) still finds the true global maximum while
// halving the work versus the naive all-pairs version. The board's own
// optimization thread (message 44134) recommends exactly this pruning.
// The global maximum over all i is the largest set of collinear points in
// the whole photo. The board (message 350385) confirms the process is a
// single find-and-delete pass, not iterative removal: find the single best
// line; if it has >= 4 points, eliminate exactly those points once, else
// eliminate nothing. That matches a direct reading of the statement.
//
// To avoid the classic TLE noted repeatedly on the board (floating point
// angle/atan2 sorts with epsilon comparisons), directions are normalized as
// exact integer fractions in lowest terms with a canonical sign, so equality
// testing is an exact integer comparison. Rather than sorting the directions
// per anchor (O(n log n) each, O(n^2 log n) total), counts are tallied in an
// open-addressing hash table keyed by the packed (dx,dy), reused across
// anchors via a per-slot "stamp" so it never needs clearing; this makes each
// anchor's pass O(rightCount) amortized, O(n^2) total. An early-exit prune
// (remaining points to the right can't produce a line beating the current
// best) stops the anchor loop as soon as it can no longer help.
#include <cstdio>

static int X[1000], Y[1000];

// Hash table for direction -> count, reused across anchors via stamps.
// Capacity is a power of two comfortably above the max distinct directions
// per anchor (< 1000), keeping the load factor low for cheap probing.
static const int HCAP = 4096;
static const int HMASK = HCAP - 1;
static int hKey[HCAP];
static int hCnt[HCAP];
static int hStamp[HCAP];
static int curStamp = 0;

static int igcd(int a, int b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b) { int t = a % b; a = b; b = t; }
    return a;
}

int main() {
    int n;
    int photo = 1;
    while (scanf("%d", &n) == 1 && n) {
        for (int i = 0; i < n; ++i) scanf("%d %d", &X[i], &Y[i]);

        int bestTotal = (n > 0) ? 1 : 0; // largest collinear set found so far

        for (int i = 0; i < n; ++i) {
            int rightCount = n - 1 - i;
            // prune: even if all points to the right of i are collinear
            // with i, that line has at most rightCount + 1 points; if that
            // can't beat bestTotal already found, no later anchor can
            // either (rightCount only shrinks), so stop entirely.
            if (rightCount + 1 <= bestTotal) break;

            ++curStamp;
            int run = 0;
            for (int j = i + 1; j < n; ++j) {
                int dx = X[j] - X[i];
                int dy = Y[j] - Y[i];
                int g = igcd(dx, dy);
                if (g != 0) { dx /= g; dy /= g; }
                if (dx < 0 || (dx == 0 && dy < 0)) { dx = -dx; dy = -dy; }
                // dx in [0,10000], dy in [-10000,10000] after canonicalizing
                // sign; pack into one int key.
                int key = dx * 20001 + (dy + 10000);
                unsigned h = (unsigned)key * 2654435761u;
                int idx = (int)(h & HMASK);
                while (hStamp[idx] == curStamp && hKey[idx] != key) idx = (idx + 1) & HMASK;
                if (hStamp[idx] != curStamp) {
                    hStamp[idx] = curStamp;
                    hKey[idx] = key;
                    hCnt[idx] = 0;
                }
                int c = ++hCnt[idx];
                if (c > run) run = c;
            }
            int totalHere = run + 1;
            if (totalHere > bestTotal) bestTotal = totalHere;
        }

        int total = (bestTotal >= 4) ? bestTotal : 0;
        printf("Photo %d: %d points eliminated\n", photo++, total);
    }
    return 0;
}
