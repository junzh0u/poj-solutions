// POJ 3666 - Making the Grade
//
// Given A[1..N] (N <= 2000, 0 <= Ai <= 1e9), find the minimum of
// sum |Ai - Bi| over sequences B that are nondecreasing OR nonincreasing.
//
// Key lemma: some optimal B uses only values that already appear in A.
// Take an optimal B and look at a maximal run of equal values v.  Holding
// everything else fixed, the cost as a function of v is sum |Ai - v| over the
// run's indices: piecewise linear and convex, so it is nonincreasing up to a
// median of those Ai and nondecreasing after.  v is free to slide within
// [value of the block before, value of the block after]; sliding it to the end
// of that interval that does not increase the cost either lands on a median
// (an element of A) or merges the run with a neighbouring block, which strictly
// reduces the number of blocks.  Iterating terminates with every value in A.
//
// So: sort A into s[], and DP over the N candidate values.
//   dp[i][j] = cheapest way to make the first i elements nondecreasing with
//              B[i] = s[j]  =  |A[i] - s[j]| + min_{k <= j} dp[i-1][k].
// The inner minimum is a running prefix minimum swept left to right, so each
// row costs O(N) and the whole DP O(N^2) = 4e6 rather than O(N^3).  Only the
// previous row is needed, so two arrays suffice.
//
// The nonincreasing case is the same DP run on the reversed array; the answer
// is the smaller of the two.  The statement says "nonincreasing or
// nondecreasing" and both directions genuinely matter.
//
// Statement caveat: it claims signed 32-bit integers suffice for the answer.
// They do not - N = 2000 alternating 0 / 1e9 costs 999000000000, well past
// 2^31.  This uses long long throughout and prints the result digit by digit
// so it does not depend on %lld being supported by the judge's compiler.
#include <cstdio>
#include <cstdlib>
#include <algorithm>

static const int MAXN = 2005;

static int a[MAXN], s[MAXN], r[MAXN];
static long long prv[MAXN], cur[MAXN];

static long long labs64(long long x) { return x < 0 ? -x : x; }

/* minimum cost to make v[0..n-1] nondecreasing, targets drawn from sorted s[] */
static long long solve(const int *v, int n)
{
    int i, j;
    long long best;

    for (j = 0; j < n; j++)
        prv[j] = labs64((long long)v[0] - (long long)s[j]);

    for (i = 1; i < n; i++) {
        long long run = prv[0];
        for (j = 0; j < n; j++) {
            if (prv[j] < run) run = prv[j];
            cur[j] = labs64((long long)v[i] - (long long)s[j]) + run;
        }
        for (j = 0; j < n; j++) prv[j] = cur[j];
    }

    best = prv[0];
    for (j = 1; j < n; j++)
        if (prv[j] < best) best = prv[j];
    return best;
}

/* print a nonnegative long long without relying on %lld portability */
static void putll(long long x)
{
    char buf[24];
    int p = 0;
    if (x == 0) {
        putchar('0');
    } else {
        while (x > 0) { buf[p++] = (char)('0' + (int)(x % 10)); x /= 10; }
        while (p > 0) putchar(buf[--p]);
    }
    putchar('\n');
}

int main(void)
{
    int n, i;
    long long up, down;

    if (scanf("%d", &n) != 1) return 0;
    for (i = 0; i < n; i++)
        if (scanf("%d", &a[i]) != 1) return 0;

    for (i = 0; i < n; i++) s[i] = a[i];
    std::sort(s, s + n);

    for (i = 0; i < n; i++) r[i] = a[n - 1 - i];

    up = solve(a, n);
    down = solve(r, n);

    putll(up < down ? up : down);
    return 0;
}
