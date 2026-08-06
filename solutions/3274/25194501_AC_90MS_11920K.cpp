// POJ 3274 - Gold Balanced Lineup
//
// N <= 100000 cows, each described by a K-bit feature mask (K <= 30). A range
// i..j is balanced when every one of the K features occurs the same number of
// times in it. Find the longest such range.
//
// Let cnt[p][f] be the number of cows among the first p that show feature f.
// The range (i, j] is balanced iff cnt[j][f] - cnt[i][f] is the same for all f,
// i.e. iff the normalised vector d[p] = (cnt[p][1]-cnt[p][0], ...,
// cnt[p][K-1]-cnt[p][0]) is identical at p = i and p = j. So the answer is the
// widest gap between two prefixes carrying the same signature. The empty prefix
// p = 0 must be in the table, or a balanced range that starts at cow 1 is missed
// (and, for the same reason, a range of length 0 is the fallback answer 0).
//
// Rather than hashing the (K-1)-int signature, the K+1 rows are laid out in one
// flat array and their indices sorted with memcmp as the comparator: memcmp's
// byte order is not numeric but it is a total order, which is all that is needed
// to bring equal signatures together. Each group then contributes max index -
// min index. O(N K log N), 12 MB, well inside the 2s / 64 MB limits.
//
// Ambiguities: the features arrive packed as one integer per cow, not as K
// separate numbers - bit f of the value means feature f+1. K = 1 makes the
// signature empty, so every prefix matches every other and the answer is N; the
// width-0 comparator handles that without a special case.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>

static const int MAXN = 100005;
static const int MAXK = 30;

static int rowdat[MAXN * (MAXK - 1)];
static int idx[MAXN];
static int W; /* width of a row = K-1 */

struct Cmp {
    bool operator()(int a, int b) const {
        if (W == 0) return false;
        return memcmp(rowdat + (size_t)a * W, rowdat + (size_t)b * W,
                      (size_t)W * sizeof(int)) < 0;
    }
};

static bool sameRow(int a, int b) {
    if (W == 0) return true;
    return memcmp(rowdat + (size_t)a * W, rowdat + (size_t)b * W,
                  (size_t)W * sizeof(int)) == 0;
}

int main() {
    int n, k;
    if (scanf("%d %d", &n, &k) != 2) return 0;
    W = k - 1;

    int cnt[MAXK];
    for (int j = 0; j < k; ++j) cnt[j] = 0;

    /* row 0 = empty prefix: all differences zero */
    for (int j = 0; j < W; ++j) rowdat[j] = 0;

    for (int i = 1; i <= n; ++i) {
        unsigned int v;
        if (scanf("%u", &v) != 1) v = 0u;
        for (int j = 0; j < k; ++j)
            if (v & (1u << j)) ++cnt[j];
        int *row = rowdat + (size_t)i * W;
        for (int j = 1; j < k; ++j) row[j - 1] = cnt[j] - cnt[0];
    }

    for (int i = 0; i <= n; ++i) idx[i] = i;
    std::sort(idx, idx + n + 1, Cmp());

    int best = 0;
    int s = 0;
    for (int i = 1; i <= n + 1; ++i) {
        if (i == n + 1 || !sameRow(idx[s], idx[i])) {
            int lo = idx[s], hi = idx[s];
            for (int t = s; t < i; ++t) {
                if (idx[t] < lo) lo = idx[t];
                if (idx[t] > hi) hi = idx[t];
            }
            if (hi - lo > best) best = hi - lo;
            s = i;
        }
    }

    printf("%d\n", best);
    return 0;
}
