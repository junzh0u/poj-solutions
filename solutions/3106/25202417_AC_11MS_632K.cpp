// POJ 3106 - Flip and Turn
// Model: claude-sonnet-5
//
// Approach: the 10 operation identifiers realize only 8 distinct symmetries
// of a rectangle (X=C, Y=B, Z=A, since 270ccw=90cw, 180ccw=180cw, 90ccw=270cw).
// This is exactly the hyperoctahedral group of rank 2 (order 8): every
// element is (swap axes?, flip axis1?, flip axis2?). Track only that state
// (swap,fr,fc) while scanning the up-to-100000 operations in O(1) each,
// then apply the single composed transform to the original matrix once in
// O(m*n). This avoids ever materializing an intermediate matrix.
//
// Derivation: maintain g(I,J) = (i,j), the original-matrix cell shown at
// current position (I,J), in the form:
//   not swap: i = fr? (m0-1-I) : I ;  j = fc? (n0-1-J) : J
//   swap:     i = fc? (m0-1-J) : J ;  j = fr? (n0-1-I) : I
// (mCur = swap? n0:m0, nCur = swap? m0:n0 are always consistent with this).
// For each op, inverting its given forward formula and substituting into g
// yields the following O(1) state-transition rules (verified against the
// sample and against a literal brute-force simulator on randomized cases):
//   H:  fr ^= 1
//   V:  fc ^= 1
//   '1' (main-diag transpose):  swap^=1; swap(fr,fc)
//   '2' (anti-diag transpose):  swap^=1; (fr,fc) = (!fc, !fr)
//   A/Z (rot90 cw / rot270 ccw): swap^=1; (fr,fc) = (fc, !fr)
//   C/X (rot270 cw / rot90 ccw): swap^=1; (fr,fc) = (!fc, fr)
//   B/Y (rot180 cw / rot180 ccw): fr^=1; fc^=1
// (all updates use the OLD fr/fc values simultaneously).
//
// No statement ambiguity found beyond the well-known X/Y/Z-vs-A/B/C
// redundancy; the discuss board's only substantive note is that a naive
// per-operation O(m*n) simulation over 100000 ops TLEs, which this avoids.
#include <cstdio>
#include <cstring>

static char A[305][305];
static char out[305][306];
static char ops[100005];

int main() {
    int m0, n0;
    if (scanf("%d %d", &m0, &n0) != 2) return 0;
    for (int i = 0; i < m0; i++) {
        scanf("%s", A[i]);
    }
    if (scanf("%s", ops) != 1) {
        ops[0] = '\0';
    }

    bool swapAxes = false, fr = false, fc = false;
    for (int k = 0; ops[k]; k++) {
        char c = ops[k];
        bool ofr = fr, ofc = fc;
        switch (c) {
            case 'H':
                fr = !fr;
                break;
            case 'V':
                fc = !fc;
                break;
            case '1':
                fr = ofc;
                fc = ofr;
                swapAxes = !swapAxes;
                break;
            case '2':
                fr = !ofc;
                fc = !ofr;
                swapAxes = !swapAxes;
                break;
            case 'A':
            case 'Z':
                fr = ofc;
                fc = !ofr;
                swapAxes = !swapAxes;
                break;
            case 'C':
            case 'X':
                fr = !ofc;
                fc = ofr;
                swapAxes = !swapAxes;
                break;
            case 'B':
            case 'Y':
                fr = !fr;
                fc = !fc;
                break;
            default:
                break;
        }
    }

    int mCur = swapAxes ? n0 : m0;
    int nCur = swapAxes ? m0 : n0;

    for (int i = 0; i < m0; i++) {
        for (int j = 0; j < n0; j++) {
            int I, J;
            if (!swapAxes) {
                I = fr ? (m0 - 1 - i) : i;
                J = fc ? (n0 - 1 - j) : j;
            } else {
                J = fc ? (m0 - 1 - i) : i;
                I = fr ? (n0 - 1 - j) : j;
            }
            out[I][J] = A[i][j];
        }
    }

    printf("%d %d\n", mCur, nCur);
    for (int i = 0; i < mCur; i++) {
        out[i][nCur] = '\0';
        printf("%s\n", out[i]);
    }
    return 0;
}
