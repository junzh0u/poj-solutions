// POJ 2941 - Homogeneous Squares
//
// A square is "homogeneous" if every set of n independent positions
// (one per row, one per column — i.e. every permutation) sums to the
// same value. Claim: a[i][j] is homogeneous iff it decomposes as
// a[i][j] = r[i] + c[j] for some vectors r, c (an "additive" matrix).
//
//   - If a[i][j] = r[i] + c[j], any permutation sum is sum(r) + sum(c),
//     independent of the permutation.
//   - Conversely, swapping just two rows' assignments in a permutation
//     (a transposition on two positions, keeping the rest fixed) forces
//     a[i1][j1] + a[i2][j2] == a[i1][j2] + a[i2][j1] for every pair of
//     rows/columns, which is exactly the condition that the matrix is
//     additive (its rank of differences from any single row is 0).
//
// So it suffices to check: for every row i, (a[i][j] - a[i][0]) is the
// same across all rows i (i.e. equal to (a[0][j] - a[0][0])) for every
// column j. This is O(n^2) instead of the O(n!) direct permutation
// check, needed since n can be up to 1000.
//
// Values fit in int (|values| <= 1e6, differences <= 2e6), so plain
// int arithmetic is enough. Input can be large (up to 1000x1000 per
// case, many cases), so a fast buffered integer reader is used instead
// of scanf/cin.
//
// Nothing ambiguous in the statement; "independent positions" is just
// a permutation matrix support, and the sample confirms the reading.
#include <cstdio>
using namespace std;

static char buf[1 << 16];
static int bufLen = 0, bufPos = 0;

inline int gc() {
    if (bufPos == bufLen) {
        bufLen = (int)fread(buf, 1, 1 << 16, stdin);
        bufPos = 0;
        if (bufLen == 0) return -1;
    }
    return buf[bufPos++];
}

inline bool readInt(int &out) {
    int c = gc();
    while (c == ' ' || c == '\n' || c == '\r' || c == '\t') c = gc();
    if (c == -1) return false;
    bool neg = false;
    if (c == '-') { neg = true; c = gc(); }
    int val = 0;
    while (c >= '0' && c <= '9') { val = val * 10 + (c - '0'); c = gc(); }
    out = neg ? -val : val;
    return true;
}

static int row0[1000];
static int row[1000];

int main() {
    int n;
    while (readInt(n) && n != 0) {
        for (int j = 0; j < n; ++j) readInt(row0[j]);
        bool homo = true;
        for (int i = 1; i < n; ++i) {
            for (int j = 0; j < n; ++j) readInt(row[j]);
            if (homo) {
                for (int j = 1; j < n; ++j) {
                    if ((row[j] - row[0]) != (row0[j] - row0[0])) { homo = false; break; }
                }
            }
        }
        printf(homo ? "homogeneous\n" : "not homogeneous\n");
    }
    return 0;
}
