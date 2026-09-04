// POJ 1718 - River Crossing
// Model: claude-sonnet-5
//
// Positions are indexed 0 (left bank), 1..n (poles), n+1 (right bank).
// A pole is periodic with period a+b: down at t=0, up for the next a
// units, down for the next b units, repeating. Banks are always usable.
// From a position the citizen may move (in one time unit) to any
// position within distance 5 (inclusive of staying), provided the
// destination is available at the new time.
//
// This is a time-layered reachability problem. The whole system's
// availability pattern is periodic with period L = lcm of all the
// per-pole periods (each in [2,10], so L <= lcm(1..10) = 2520). The
// discuss board (msg 169808) records the standard stopping rule: if the
// farthest-ever-reached position fails to advance for L consecutive
// time steps, the right bank is unreachable (the driving pattern has
// repeated a full period without progress).
//
// Reachability per time step is a "OR of all shifts by -5..5" of the
// previous reachable set, intersected with availability at the new
// time. This is implemented with a hand-rolled bitset (array of 64-bit
// words) so each time step costs O(numWords) instead of O(n), which
// matters because the total number of simulated time steps can be as
// large as roughly n*L in an adversarial case. Needing native 64-bit
// words is why this is submitted under G++ rather than POJ's old
// MSVC-based "C++" (language 4), which has no long long / no 64-bit
// integer type to build such a bitset from.
//
// Statement ambiguity: none found; the discuss board's second thread
// just clarifies that jumps may go both forward and backward within
// distance 5 (a subtlety already explicit in the statement's own
// example "from pole 5 you can reach poles 1..10 or the left bank").

#include <cstdio>
#include <cstring>

typedef unsigned long long u64;
const int W = 20; // 20*64 = 1280 bits, comfortably covers n+1+5 <= 1006
const int MAXL = 2521; // lcm of values in [2,10] is at most 2520

static u64 avail[MAXL][W];

int gcdInt(int a, int b) { while (b) { int t = a % b; a = b; b = t; } return a; }

void zeroArr(u64 arr[]) { for (int i = 0; i < W; i++) arr[i] = 0; }
void copyArr(u64 dst[], const u64 src[]) { for (int i = 0; i < W; i++) dst[i] = src[i]; }
void orEq(u64 dst[], const u64 src[]) { for (int i = 0; i < W; i++) dst[i] |= src[i]; }
void andEq(u64 dst[], const u64 src[]) { for (int i = 0; i < W; i++) dst[i] &= src[i]; }

void shl(const u64 src[], u64 dst[], int d) {
    for (int w = W - 1; w >= 0; w--) {
        u64 hi = src[w] << d;
        u64 lo = (w > 0) ? (src[w - 1] >> (64 - d)) : (u64)0;
        dst[w] = hi | lo;
    }
}
void shr(const u64 src[], u64 dst[], int d) {
    for (int w = 0; w < W; w++) {
        u64 lo = src[w] >> d;
        u64 hi = (w + 1 < W) ? (src[w + 1] << (64 - d)) : (u64)0;
        dst[w] = lo | hi;
    }
}
bool testBit(const u64 arr[], int idx) { return (arr[idx / 64] >> (idx % 64)) & 1ULL; }
void setBit(u64 arr[], int idx) { arr[idx / 64] |= (1ULL << (idx % 64)); }

int highestBit(const u64 arr[]) {
    for (int w = W - 1; w >= 0; w--) {
        if (arr[w] != 0) {
            for (int b = 63; b >= 0; b--) {
                if ((arr[w] >> b) & 1ULL) return w * 64 + b;
            }
        }
    }
    return -1;
}

static int a[1005], b[1005], period[1005];

int main() {
    int x;
    scanf("%d", &x);
    while (x--) {
        int n;
        scanf("%d", &n);
        for (int i = 1; i <= n; i++) {
            scanf("%d %d", &a[i], &b[i]);
            period[i] = a[i] + b[i];
        }
        int L = 1;
        for (int i = 1; i <= n; i++) {
            int g = gcdInt(L, period[i]);
            L = L / g * period[i];
        }
        for (int t = 0; t < L; t++) {
            zeroArr(avail[t]);
            setBit(avail[t], 0);
            setBit(avail[t], n + 1);
            for (int i = 1; i <= n; i++) {
                int r = t % period[i];
                if (r >= 1 && r <= a[i]) setBit(avail[t], i);
            }
        }

        u64 reach[W], acc[W], tmp[W];
        zeroArr(reach);
        setBit(reach, 0);
        int maxPos = 0;
        int noProgress = 0;
        long long answer = -1;
        long long safetyCap = (long long)(n + 5) * L + 10; // generous, well above proven bound
        for (long long t = 0; t < safetyCap; t++) {
            zeroArr(acc);
            orEq(acc, reach);
            for (int d = 1; d <= 5; d++) {
                shl(reach, tmp, d); orEq(acc, tmp);
                shr(reach, tmp, d); orEq(acc, tmp);
            }
            int tm1 = (int)((t + 1) % L);
            andEq(acc, avail[tm1]);
            if (testBit(acc, n + 1)) { answer = t + 1; break; }
            int newMax = highestBit(acc);
            if (newMax > maxPos) { maxPos = newMax; noProgress = 0; }
            else { noProgress++; }
            if (noProgress >= L) { answer = -1; break; }
            copyArr(reach, acc);
        }
        if (answer < 0) printf("NO\n");
        else printf("%lld\n", answer);
    }
    return 0;
}
