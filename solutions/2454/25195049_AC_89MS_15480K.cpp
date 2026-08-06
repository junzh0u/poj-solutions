// POJ 2454 - Jersey Politics
// Model: claude-sonnet-5
// Approach: Sort cities descending by Jersey-cow count. Take the top 2K
// cities: an exchange argument shows that if ANY partition into two winning
// K-city districts (plus a third, don't-care) exists at all, one using
// exactly these top 2K cities as the two districts also exists (swapping a
// non-top-2K city for a top-2K one never decreases a district's sum, so any
// valid solution can be transformed into one built from the top 2K without
// losing validity).
//
// Among the top 2K cities we need two disjoint size-K groups both with sum
// > 500*K (majority of the district's 1000*K cows). Equivalently, since the
// two groups partition a fixed total, we need one size-K subset whose sum S
// satisfies 500*K < S < total - 500*K. This is solved exactly with a 0/1
// subset-sum DP tracked by (count chosen, sum), using bitsets for speed and
// a parent-pointer table (recording, the first time a (count,sum) state
// becomes reachable, which item index completed it) so a witness subset can
// be reconstructed in O(1) per step afterward. Values are small (0..1000)
// and there are only up to 120 cities in play, so this is fast and uses a
// few MB of memory, well inside the limits.
//
// The remaining K cities (lowest-valued) become the third, don't-care
// district. No statement ambiguity found; problem guarantees the instance
// is solvable and grades with a special judge (any valid partition
// accepted).
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

const int MAXK = 61;              // K up to 60, rows 0..60
const int TOTAL_MAX = 2 * 60 * 1000 + 1; // 0..120000
const int WORDS = (TOTAL_MAX + 63) / 64;

static unsigned long long dp[MAXK][WORDS];
static short parentIdx[MAXK][TOTAL_MAX]; // -1 unreachable, -2 base case

inline bool getBit(const unsigned long long *row, int pos) {
    return (row[pos >> 6] >> (pos & 63)) & 1ULL;
}
inline void setBitArr(unsigned long long *row, int pos) {
    row[pos >> 6] |= (1ULL << (pos & 63));
}

// dst = src shifted left by 'shift' bits (word count 'words'), OR'd into dst
// also computes newBits = (src << shift) & ~dst_before, returned via outNew
void shiftAndMark(const unsigned long long *src, int shift, unsigned long long *dst,
                   unsigned long long *newBits, int words) {
    int wordShift = shift >> 6;
    int bitShift = shift & 63;
    for (int w = 0; w < words; ++w) {
        unsigned long long val = 0ULL;
        int srcW = w - wordShift;
        if (srcW >= 0 && srcW < words) {
            val = src[srcW] << bitShift;
            if (bitShift != 0 && srcW - 1 >= 0) {
                val |= src[srcW - 1] >> (64 - bitShift);
            }
        }
        unsigned long long nb = val & ~dst[w];
        newBits[w] = nb;
        dst[w] |= val;
    }
}

struct City {
    int value;
    int id;
};

bool cmpDesc(const City &a, const City &b) {
    return a.value > b.value;
}

int main() {
    int k;
    if (scanf("%d", &k) != 1) return 0;
    int n = 3 * k;
    City c[180];
    for (int i = 0; i < n; ++i) {
        scanf("%d", &c[i].value);
        c[i].id = i + 1;
    }
    sort(c, c + n, cmpDesc);

    int m = 2 * k; // number of "top" cities in play
    int totalMax = m * 1000; // max possible sum of these m cities (upper bound)
    int words = (totalMax + 1 + 63) / 64; // enough words to hold bits 0..totalMax
    if (words > WORDS) words = WORDS; // safety clamp, should never trigger

    // init
    for (int j = 0; j < MAXK; ++j) {
        memset(dp[j], 0, sizeof(unsigned long long) * words);
    }
    for (int j = 0; j < MAXK; ++j) {
        for (int s = 0; s <= totalMax; ++s) parentIdx[j][s] = -1;
    }
    setBitArr(dp[0], 0);
    parentIdx[0][0] = -2;

    unsigned long long newBits[WORDS];

    for (int i = 0; i < m; ++i) {
        int v = c[i].value;
        int maxJ = min(k, i + 1);
        for (int j = maxJ; j >= 1; --j) {
            shiftAndMark(dp[j - 1], v, dp[j], newBits, words);
            // record parents for newly set bits
            for (int w = 0; w < words; ++w) {
                unsigned long long bits = newBits[w];
                while (bits) {
                    int idx = __builtin_ctzll(bits);
                    int bitpos = w * 64 + idx;
                    if (bitpos <= totalMax) {
                        parentIdx[j][bitpos] = (short)i;
                    }
                    bits &= bits - 1;
                }
            }
        }
    }

    // total sum of top-m cities
    long long total = 0;
    for (int i = 0; i < m; ++i) total += c[i].value;
    long long thresh = 500LL * k;

    int chosenS = -1;
    int lo = (int)(thresh + 1);
    int hi = (int)(total - thresh - 1);
    if (lo < 0) lo = 0;
    if (hi > totalMax) hi = totalMax;
    for (int s = lo; s <= hi; ++s) {
        if (getBit(dp[k], s)) { chosenS = s; break; }
    }

    bool inGroupA[180];
    memset(inGroupA, 0, sizeof(inGroupA));

    if (chosenS >= 0) {
        int j = k, s = chosenS;
        while (j > 0) {
            int itemIdx = parentIdx[j][s];
            inGroupA[itemIdx] = true;
            s -= c[itemIdx].value;
            j -= 1;
        }
    }

    int groupA[60], groupB[60];
    int ac = 0, bc = 0;
    for (int i = 0; i < m; ++i) {
        if (inGroupA[i]) groupA[ac++] = c[i].id;
        else groupB[bc++] = c[i].id;
    }

    for (int i = 0; i < k; ++i) printf("%d\n", groupA[i]);
    for (int i = 0; i < k; ++i) printf("%d\n", groupB[i]);
    for (int i = m; i < n; ++i) printf("%d\n", c[i].id);
    return 0;
}
