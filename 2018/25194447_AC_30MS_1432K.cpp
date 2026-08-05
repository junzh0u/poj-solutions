// POJ 2018 - Best Cow Fences
//
// Pick a contiguous block of at least F of the N fields maximising the average
// number of cows per field; print 1000*average TRUNCATED (the statement says
// "Do not perform rounding, just print the integer that is 1000*ncows/nfields").
//
// Parametric search, but on the *integer answer* rather than on a real average,
// so no floating point is involved anywhere. Let k be a candidate value of
// 1000*avg. Some block of length >= F has 1000*sum/len >= k exactly when that
// block's sum of (1000*a_i - k) is >= 0, which a prefix-sum sweep decides in
// O(n): walking j from F to n, keep the minimum prefix among indices <= j-F and
// test pre[j] - min >= 0. Binary search k over [0, 2000*1000] -- 21 sweeps,
// O(n log(1000*maxA)) overall.
//
// The largest feasible integer k is precisely floor(1000*maxAverage), so the
// required truncation falls out of the search instead of being applied to a
// float afterwards. Prefixes reach 1e5 * 2e6 = 2e11, hence long long; the
// answer itself fits in an int, so it is printed with %d (POJ's MinGW-era
// runtime is unreliable with %lld).
//
// Verified against an O(n^2) exact-fraction brute force on 1600 random cases.
#include <cstdio>

static int n, f;
static int a[100005];
static long long pre[100005];

/* Is there a contiguous block of >= f fields whose 1000*sum/len >= k ?
   Equivalent to: some such block has sum of (1000*a_i - k) >= 0. */
static bool feasible(long long k)
{
    int j;
    long long best;
    long long mn;
    pre[0] = 0;
    for (j = 1; j <= n; ++j)
        pre[j] = pre[j - 1] + (long long)1000 * a[j] - k;
    mn = pre[0];
    for (j = f; j <= n; ++j) {
        if (pre[j - f] < mn)
            mn = pre[j - f];
        best = pre[j] - mn;
        if (best >= 0)
            return true;
    }
    return false;
}

int main()
{
    int i;
    long long lo, hi, mid;
    if (scanf("%d %d", &n, &f) != 2)
        return 0;
    for (i = 1; i <= n; ++i)
        scanf("%d", &a[i]);
    lo = 0;               /* a_i >= 1, so 1000*avg >= 1000 > 0 : always feasible */
    hi = 2000000;         /* a_i <= 2000 */
    while (lo < hi) {
        mid = lo + (hi - lo + 1) / 2;
        if (feasible(mid))
            lo = mid;
        else
            hi = mid - 1;
    }
    printf("%d\n", (int)lo);
    return 0;
}
