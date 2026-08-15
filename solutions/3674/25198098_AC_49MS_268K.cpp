// POJ 3674 - Super Assassin
// Model: claude-sonnet-5
// Total damage = sum(D_i for chosen skills) + 0.1 * sum over consecutive pairs
// (prev,next) in the chosen order of E_prev * D_next (each skill's enhance only
// boosts the immediately following strike, per the statement, no compounding).
// Bucket skills by E (0..10), sort each bucket descending by D. For a *fixed*
// sequence of E-buckets chosen for chain positions 1..k (k<=6), the per-position
// multiplier (mult10 = 10 for position 1, 10+E_prev for later positions) is
// completely determined by the bucket sequence, independent of which specific
// skill within a bucket fills that slot. So for a bucket used c times, the optimal
// choice is the c largest D values in that bucket, paired by the rearrangement
// inequality (largest D with the largest multiplier among that bucket's slots) --
// confirmed by discussion board post 143511 ("give the earlier/larger E slot the
// larger D when E values tie"). We DFS over all bucket sequences of length 0..6
// (11 choices per slot, pruned by remaining supply), maintaining per-bucket sorted
// multiplier lists incrementally, and take the global max over every prefix (chain
// length is implicitly optimized, not just length 6).
// All arithmetic is exact integers scaled by 10 (mult10 in [10,20]); D_i is always
// a multiple of 10 so every partial sum stays a multiple of 10, and dividing the
// final max by 10 is exact (problem guarantees a 32-bit integer answer). Uses
// long long for the scaled running sums since D up to 1e8 times mult10 up to 20,
// summed over 6 terms, can reach ~1.1e10 before the final /10 -- requires 64-bit,
// hence G++ rather than the 32-bit-long MSVC compiler.
// Board evidence (buggy brute-force posts) shows multiple datasets are read until
// EOF even though the statement only describes a single N; read-to-EOF matches
// that evidence.
#include <cstdio>
#include <algorithm>
using namespace std;

int cntE[11];
int D[11][305];
int used[11];
int multlist[11][7];
int listlen[11];
long long bucketContrib[11];
long long totalSum;
long long globalMax;

void recompute(int e) {
    long long s = 0;
    for (int j = 0; j < listlen[e]; j++) s += (long long)D[e][j] * multlist[e][j];
    bucketContrib[e] = s;
}

void dfs(int depth, int prevE) {
    if (totalSum > globalMax) globalMax = totalSum;
    if (depth == 6) return;
    for (int e = 0; e <= 10; e++) {
        if (used[e] < cntE[e]) {
            int mult10 = (depth == 0) ? 10 : (10 + prevE);
            long long oldContrib = bucketContrib[e];
            int pos = listlen[e];
            while (pos > 0 && multlist[e][pos - 1] < mult10) {
                multlist[e][pos] = multlist[e][pos - 1];
                pos--;
            }
            multlist[e][pos] = mult10;
            listlen[e]++;
            recompute(e);
            totalSum += bucketContrib[e] - oldContrib;
            used[e]++;

            dfs(depth + 1, e);

            used[e]--;
            totalSum -= bucketContrib[e] - oldContrib;
            for (int j = pos; j < listlen[e] - 1; j++) multlist[e][j] = multlist[e][j + 1];
            listlen[e]--;
            bucketContrib[e] = oldContrib;
        }
    }
}

int main() {
    int n;
    while (scanf("%d", &n) == 1) {
        for (int e = 0; e <= 10; e++) { cntE[e] = 0; used[e] = 0; listlen[e] = 0; bucketContrib[e] = 0; }
        for (int i = 0; i < n; i++) {
            int d, e;
            scanf("%d %d", &d, &e);
            D[e][cntE[e]++] = d;
        }
        for (int e = 0; e <= 10; e++) sort(D[e], D[e] + cntE[e], greater<int>());
        totalSum = 0;
        globalMax = 0;
        dfs(0, -1);
        printf("%lld\n", globalMax / 10);
    }
    return 0;
}
