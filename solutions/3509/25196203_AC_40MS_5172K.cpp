// POJ 3509 - Rotating Rings
// Model: claude-sonnet-5
//
// A square grid decomposes into concentric rings. Rotating a ring is a
// cyclic shift of the values on that ring; different rings rotate
// independently. The grid can be sorted iff, for every ring, the sequence
// of input values around the ring (in a fixed traversal order, e.g.
// clockwise from the top-left corner) is a cyclic rotation of the sequence
// of values the *sorted* grid would have at those same coordinates.
//
// For ring r (0-indexed from outside) with corners top=r, bottom=N-1-r,
// left=r, right=N-1-r, walk clockwise: top row (left..right-1), right
// column (top..bottom-1), bottom row (right..left+1), left column
// (bottom..top+1). At coordinate (i,j) the sorted-grid value is
// i*N+j+1 (row-major). Build A[] = input values, B[] = target values in
// that order, and test whether A is a rotation of B via standard KMP
// (search pattern A in text B+B, using B[i % L] instead of materializing
// the doubled array). Each ring costs O(L); rings partition the grid, so
// total work is O(N^2) per test case.
//
// The grid is NOT guaranteed to be a permutation of 1..N^2 (POJ discuss
// board: duplicate values occur), and one official case reportedly
// contains a -1 despite the statement calling values "natural numbers" -
// the reader must accept a leading '-'. Neither fact needs special
// casing: the per-ring rotation check naturally reports NO whenever a
// ring's multiset of values doesn't match the sorted grid's for that
// ring (e.g. a stray -1, or a value that belongs to a different ring).
// A ring of side 1 (the centre cell when N is odd) is handled by the same
// general KMP code with L=1, which degenerates correctly to a direct
// equality test - no special case needed (a known trap per the board).
//
// Ambiguity: none found in the statement itself: the board's -1 report
// and non-permutation report were the only pitfalls, both handled above.

#include <cstdio>
#include <cstdlib>

static char buf[1 << 20];
static int bufLen = 0, bufPos = 0;

static inline int gc() {
    if (bufPos == bufLen) {
        bufLen = (int)fread(buf, 1, sizeof(buf), stdin);
        bufPos = 0;
        if (bufLen <= 0) return -1;
    }
    return buf[bufPos++];
}

static inline bool readInt(int &x) {
    int c = gc();
    while (c != -1 && c != '-' && (c < '0' || c > '9')) c = gc();
    if (c == -1) return false;
    bool neg = false;
    if (c == '-') { neg = true; c = gc(); }
    int v = 0;
    while (c >= '0' && c <= '9') { v = v * 10 + (c - '0'); c = gc(); }
    x = neg ? -v : v;
    return true;
}

static const int MAXN = 1000;
static int grid[MAXN * MAXN];
static int A[4 * MAXN], B[4 * MAXN], fail_[4 * MAXN];

static bool isRotation(int L) {
    fail_[0] = 0;
    for (int i = 1; i < L; i++) {
        int j = fail_[i - 1];
        while (j > 0 && A[i] != A[j]) j = fail_[j - 1];
        if (A[i] == A[j]) j++;
        fail_[i] = j;
    }
    int j = 0;
    for (int i = 0; i < 2 * L; i++) {
        int bv = B[i % L];
        while (j > 0 && A[j] != bv) j = fail_[j - 1];
        if (A[j] == bv) j++;
        if (j == L) return true;
    }
    return false;
}

int main() {
    int N;
    int caseNum = 0;
    while (readInt(N) && N != 0) {
        caseNum++;
        int total = N * N;
        for (int i = 0; i < total; i++) readInt(grid[i]);

        bool ok = true;
        int maxR = (N - 1) / 2;
        for (int r = 0; r <= maxR && ok; r++) {
            int top = r, bottom = N - 1 - r, left = r, right = N - 1 - r;
            int s = N - 2 * r;
            int L;
            if (s == 1) {
                L = 1;
                int i = top, jj = left;
                A[0] = grid[i * N + jj];
                B[0] = i * N + jj + 1;
            } else {
                L = 4 * (s - 1);
                int seg = s - 1;
                int idx = 0;
                for (int k = 0; k < seg; k++) {
                    int i = top, jj = left + k;
                    A[idx] = grid[i * N + jj];
                    B[idx] = i * N + jj + 1;
                    idx++;
                }
                for (int k = 0; k < seg; k++) {
                    int i = top + k, jj = right;
                    A[idx] = grid[i * N + jj];
                    B[idx] = i * N + jj + 1;
                    idx++;
                }
                for (int k = 0; k < seg; k++) {
                    int i = bottom, jj = right - k;
                    A[idx] = grid[i * N + jj];
                    B[idx] = i * N + jj + 1;
                    idx++;
                }
                for (int k = 0; k < seg; k++) {
                    int i = bottom - k, jj = left;
                    A[idx] = grid[i * N + jj];
                    B[idx] = i * N + jj + 1;
                    idx++;
                }
            }
            if (!isRotation(L)) ok = false;
        }
        printf("%d. %s\n", caseNum, ok ? "YES" : "NO");
    }
    return 0;
}
