// POJ 1633 - Gladiators
// Model: claude-sonnet-5
//
// Each obstacle occupies two platforms at its own height; obstacles nest
// (an obstacle placed between another's two platforms must be strictly
// taller) or sit one after another.  Reading a valid course left to right
// gives a sequence of 2*m platform heights; "points of difficulty" = 1 for
// the first platform plus the number of positions whose height exceeds the
// previous one.
//
// The tallest of m obstacles can never have anything nested inside it (no
// obstacle is taller), so its two platforms are always adjacent in the
// sequence.  Removing that adjacent pair from any valid m-obstacle course
// yields a valid (m-1)-obstacle course, and conversely every (m-1)-obstacle
// course can be extended by reinserting the new tallest pair "TT" into any
// of its 2(m-1)+1 = 2m-1 gaps (before the first platform, after the last,
// or between two consecutive platforms).
//
// Tracking how the difficulty score changes under each insertion gives the
// recurrence (k = difficulty of the m-obstacle course, k0 = difficulty
// before insertion):
//   - front gap, or a gap that was already an ascent (k0-1 of them): score
//     unchanged (k0 -> k0);
//   - back gap, or a gap that was a descent (2m-2-k0 of them): score
//     increases by one (k0 -> k0+1).
// That yields, with f(n,k) counting n-obstacle courses of difficulty k:
//   f(n,k) = k*f(n-1,k) + (2n-k)*f(n-1,k-1),   f(0,0) = 1.
// (Matching the standard second-order Eulerian number recurrence.)  Verified
// by hand against every value in the sample, including the ambiguous-looking
// f(3,2)=8.
//
// Answers can need up to ~80 decimal digits (m=50 gives (2*50-1)!!  ~
// 2.7e78 total courses across all k, under the stated 10^100 bound), so
// arithmetic is done with a small base-10000 bignum (add, and multiply by a
// scalar magnitude <= 2m <= 100, so every intermediate product fits easily
// in a 32-bit int -- no long long / %lld dependency at all).
//
// m <= 50 per the statement; k has no stated bound, so any k outside
// [0, m] is answered 0 without touching the DP table.

#include <cstdio>
#include <vector>
using namespace std;

typedef vector<int> Big; // base 10000, little-endian limbs

static Big fromInt(int v) {
    Big r;
    if (v == 0) { r.push_back(0); return r; }
    while (v > 0) { r.push_back(v % 10000); v /= 10000; }
    return r;
}

static Big mulInt(const Big &a, int m) {
    Big r;
    int carry = 0; // magnitude of a[i]*m+carry stays well under 2^31
                    // (limb < 10000, m <= 100), so plain int is exact
    for (size_t i = 0; i < a.size(); ++i) {
        int cur = a[i] * m + carry;
        r.push_back(cur % 10000);
        carry = cur / 10000;
    }
    while (carry > 0) { r.push_back(carry % 10000); carry /= 10000; }
    if (r.empty()) r.push_back(0);
    while (r.size() > 1 && r.back() == 0) r.pop_back();
    return r;
}

static Big addBig(const Big &a, const Big &b) {
    Big r;
    size_t n = a.size() > b.size() ? a.size() : b.size();
    int carry = 0;
    for (size_t i = 0; i < n; ++i) {
        int x = (i < a.size() ? a[i] : 0) + (i < b.size() ? b[i] : 0) + carry;
        r.push_back(x % 10000);
        carry = x / 10000;
    }
    if (carry > 0) r.push_back(carry);
    while (r.size() > 1 && r.back() == 0) r.pop_back();
    return r;
}

static void printBig(const Big &a) {
    printf("%d", a[a.size() - 1]);
    for (int i = (int)a.size() - 2; i >= 0; --i) printf("%04d", a[i]);
    printf("\n");
}

static const int MAXM = 50;
static Big f[MAXM + 1][MAXM + 1];

int main() {
    Big zero = fromInt(0);
    for (int n = 0; n <= MAXM; ++n)
        for (int k = 0; k <= MAXM; ++k)
            f[n][k] = zero;
    f[0][0] = fromInt(1);
    for (int n = 1; n <= MAXM; ++n) {
        for (int k = 1; k <= n; ++k) {
            Big term1 = mulInt(f[n - 1][k], k);
            Big term2 = mulInt(f[n - 1][k - 1], 2 * n - k);
            f[n][k] = addBig(term1, term2);
        }
    }

    int t;
    if (scanf("%d", &t) != 1) return 0;
    for (int tc = 0; tc < t; ++tc) {
        int m, k;
        scanf("%d %d", &m, &k);
        if (k < 0 || k > m || m < 0 || m > MAXM) {
            printf("0\n");
        } else {
            printBig(f[m][k]);
        }
    }
    return 0;
}
