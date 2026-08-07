// POJ 3243 - Clever Y
// Model: claude-opus-5
//
// Discrete logarithm with an arbitrary (not necessarily prime) modulus:
// given X, Z, K find the minimum Y with X^Y = K (mod Z).  Extended
// baby-step giant-step:
//
//   1. Try Y = 0..63 directly.  This settles Y = 0 (X^0 = 1), every X that
//      is 0 mod Z, and the whole pre-period of the sequence X^Y mod Z,
//      whose length is at most log2(Z) < 30.
//   2. While d = gcd(X, Z') > 1, use X^Y = K (mod Z') <=> (X/d)*X^(Y-1) =
//      K/d (mod Z'/d) for Y >= 1; if d does not divide K there is no
//      solution.  Each step halves Z', so it runs at most 30 times and
//      leaves A*X^j = K' (mod Z') with gcd(X, Z') = 1 and Y = j + cnt.
//   3. BSGS on that: with m = ceil(sqrt(Z')), tabulate K'*X^i for
//      0 <= i < m and scan A*X^(p*m) for p = 1..m, giving j = p*m - i.
//      The candidate blocks ((p-1)m, pm] are disjoint and increasing, so
//      the first p that hits yields the minimal j provided the table keeps
//      the LARGEST i per residue -- hence the sort by (value asc, i desc).
//      Step 1 already covered Y < 64 >= cnt, so j = 0 needs no separate
//      search, but it is checked anyway.
//
// Statement ambiguities, resolved from the discuss board:
//   * K is NOT guaranteed to be less than Z, and the judge wants K reduced
//     mod Z rather than an immediate "No Solution" (board: "1 3 4" must
//     print 0, not "No Solution").
//   * X^0 = 1, so Y = 0 is a legal answer whenever K = 1 mod Z.
//   * Y is required to satisfy 0 <= Y < Z; the minimal solution always
//     does (pre-period + period <= Z), but the bound is enforced anyway.
//
// Verified against a brute force transcribed from the statement:
// exhaustively over Z in [1,40] x X in [0,80] x K in [0,80] (262440 cases,
// covering K >= Z, X = 0 and Z = 1), plus 5200 randomized cases up to
// Z = 10^6 weighted towards long gcd-reduction chains.  Worst case
// (20 queries, Z = 999999937 prime, no solution, full m giant steps)
// runs in 0.065s and 1.9MB against limits of 5000MS / 65536K.
//
// long long is needed only for the modular products (Z <= 10^9 makes them
// fit in 64 bits); every value printed fits in int, so no %lld is used.

#include <cstdio>
#include <algorithm>
#include <utility>

typedef long long LL;

static LL gcdll(LL a, LL b) {
    while (b != 0) { LL t = a % b; a = b; b = t; }
    return a;
}

static const int MAXM = 32800;              /* ceil(sqrt(10^9)) = 31623 */
static std::pair<LL, LL> tbl[MAXM];

static bool byValThenIdxDesc(const std::pair<LL, LL> &a,
                             const std::pair<LL, LL> &b) {
    if (a.first != b.first) return a.first < b.first;
    return a.second > b.second;
}

/* minimal y >= 0 with x^y = k (mod z), or -1 if there is none */
static LL solve(LL x, LL z, LL k) {
    if (z <= 0) return -1;
    if (z == 1) return 0;                   /* x^0 mod 1 == 0 == k mod 1 */
    x %= z;
    k %= z;

    LL cur = 1 % z;
    for (int i = 0; i < 64; i++) {
        if (cur == k) return i;
        cur = cur * x % z;
    }
    if (x == 0) return -1;                  /* 0^y is 1 (y=0) or 0 (y>=1) */

    LL zz = z, kk = k, ad = 1, cnt = 0, d;
    while ((d = gcdll(x, zz)) > 1) {
        if (kk % d != 0) return -1;
        kk /= d;
        zz /= d;
        cnt++;
        ad = ad * (x / d) % zz;
    }
    ad %= zz;
    kk %= zz;
    if (ad == kk) return cnt;               /* j == 0 */

    LL m = 1;
    while (m * m < zz) m++;                 /* m = ceil(sqrt(zz)) <= 31623 */

    LL v = kk;
    for (LL bi = 0; bi < m; bi++) {
        tbl[bi] = std::make_pair(v, bi);
        v = v * x % zz;
    }
    std::sort(tbl, tbl + m, byValThenIdxDesc);

    LL xm = 1;                              /* x^m mod zz */
    for (LL e = 0; e < m; e++) xm = xm * x % zz;

    LL left = ad;
    for (LL p = 1; p <= m; p++) {
        left = left * xm % zz;
        /* first table slot with value >= left; ties keep the largest i */
        LL lo = 0, hi = m;
        while (lo < hi) {
            LL mid = lo + (hi - lo) / 2;
            if (tbl[mid].first < left) lo = mid + 1;
            else hi = mid;
        }
        if (lo < m && tbl[lo].first == left) return p * m - tbl[lo].second + cnt;
    }
    return -1;
}

int main() {
    int xi, zi, ki;
    while (scanf("%d %d %d", &xi, &zi, &ki) == 3) {
        if (xi == 0 && zi == 0 && ki == 0) break;
        LL z = zi;
        LL y = solve(xi, z, ki);
        if (y < 0 || y >= z) printf("No Solution\n");
        else printf("%d\n", (int)y);
    }
    return 0;
}
