// POJ 3104 - Drying
//
// n clothes, garment i holds a_i water. Every minute each garment loses 1 unit
// of water on its own; additionally one garment of Jane's choosing may sit on
// the radiator that minute and loses k instead (floored at zero). Minimise the
// number of minutes until every garment is dry.
//
// Approach: binary search the answer t, monotone because more time never hurts.
//
// Feasibility of a given t: during those t minutes the radiator is free for at
// most t minute-slots in total, so the question is only how many slots each
// garment demands. A garment with a_i <= t dries by air alone and needs none.
// Otherwise, a minute on the radiator removes k water where air drying would
// have removed 1, i.e. it gains k-1 over doing nothing special; the garment
// must make up (a_i - t) water beyond what the t air-drying minutes give, so it
// needs ceil((a_i - t) / (k - 1)) radiator minutes. t is feasible iff the sum
// of those counts is at most t.
//
// The floor-at-zero rule never makes this optimistic: overshooting only wastes
// water removal, so counting each radiator minute as a clean k-1 gain is exact
// for the minimum number of slots.
//
// k = 1 must be special-cased before the division: the radiator then does
// exactly what air drying does, is useless, and the answer is max(a_i).
//
// Bounds: search over [1, max a_i] (a_i >= 1, so the answer is at least 1).
// The answer fits in int, but the running slot total is up to n * 10^9, so the
// accumulator is long long and the loop bails out as soon as it exceeds t.
// O(n log(max a)) — 67MS against a 2s limit.

#include <cstdio>

static int n;
static int a[100005];
static long long k;

static int readInt(long long &out) {
    int c = getchar();
    while (c != '-' && (c < '0' || c > '9')) {
        if (c == EOF) return 0;
        c = getchar();
    }
    int neg = 0;
    if (c == '-') { neg = 1; c = getchar(); }
    long long v = 0;
    while (c >= '0' && c <= '9') { v = v * 10 + (c - '0'); c = getchar(); }
    out = neg ? -v : v;
    return 1;
}

/* can all clothes be dry within t minutes? */
static bool feasible(long long t) {
    long long used = 0;
    for (int i = 0; i < n; ++i) {
        if (a[i] <= t) continue;              /* air drying alone suffices */
        long long extra = (long long)a[i] - t; /* water the radiator must remove beyond air drying */
        long long x = (extra + (k - 1) - 1) / (k - 1); /* each radiator minute gains k-1 */
        used += x;
        if (used > t) return false;
    }
    return used <= t;
}

int main() {
    long long tmp;
    if (!readInt(tmp)) return 0;
    n = (int)tmp;
    long long mx = 0;
    for (int i = 0; i < n; ++i) {
        readInt(tmp);
        a[i] = (int)tmp;
        if (tmp > mx) mx = tmp;
    }
    readInt(k);

    if (k == 1) { printf("%d\n", (int)mx); return 0; }

    long long lo = 1, hi = mx;
    while (lo < hi) {
        long long mid = lo + (hi - lo) / 2;
        if (feasible(mid)) hi = mid; else lo = mid + 1;
    }
    printf("%d\n", (int)lo);
    return 0;
}
