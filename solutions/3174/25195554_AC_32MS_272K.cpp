// POJ 3174 - Alignment of the Planets
// Model: claude-opus-5
//
// Count and list every set of three collinear cows among N <= 770 points on a
// 0..15000 integer grid, sets sorted internally by ID and then lexicographically.
//
// Approach: brute force over all i < j < k with an exact integer cross product,
//   (x[j]-x[i])*(y[k]-y[i]) == (y[j]-y[i])*(x[k]-x[i]).
// Iterating in the natural i < j < k order emits the sets already in the
// required lexicographic order, so no sort is needed; the triples are buffered
// only because the count has to be printed on line 1 before them.
//
// The Hint warns off floating point, and integers dodge the question entirely:
// each factor is at most 15000 in magnitude, so |cross| <= 2*15000^2 = 4.5e8,
// well inside 32-bit int. Per-anchor differences are precomputed into ax/ay so
// the inner loop is two multiplies and a compare.
//
// No statement ambiguity: "sorting the cows in each set by their ID number,
// lowest first, then sort the sets by the three ID numbers" is exactly
// lexicographic order on ascending triples, which the loop nesting produces.

#include <cstdio>
#include <vector>

using namespace std;

static int x[800], y[800];
static int ax[800], ay[800];

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    for (int i = 0; i < n; i++) scanf("%d %d", &x[i], &y[i]);

    vector<int> out;
    for (int i = 0; i < n; i++) {
        for (int k = i + 1; k < n; k++) {
            ax[k] = x[k] - x[i];
            ay[k] = y[k] - y[i];
        }
        for (int j = i + 1; j < n; j++) {
            const int dx1 = ax[j];
            const int dy1 = ay[j];
            for (int k = j + 1; k < n; k++) {
                if (dx1 * ay[k] == dy1 * ax[k]) {
                    out.push_back(i + 1);
                    out.push_back(j + 1);
                    out.push_back(k + 1);
                }
            }
        }
    }

    printf("%d\n", (int)(out.size() / 3));

    /* manual buffered writer: the answer can run to hundreds of thousands of
       lines, and per-line printf is the only part that would be output-bound */
    char buf[1 << 16];
    int p = 0;
    for (size_t t = 0; t < out.size(); t += 3) {
        if (p > (1 << 16) - 64) { fwrite(buf, 1, p, stdout); p = 0; }
        for (int c = 0; c < 3; c++) {
            int v = out[t + c];
            char tmp[12];
            int m = 0;
            do { tmp[m++] = (char)('0' + v % 10); v /= 10; } while (v);
            while (m) buf[p++] = tmp[--m];
            buf[p++] = (c == 2) ? '\n' : ' ';
        }
    }
    fwrite(buf, 1, p, stdout);
    return 0;
}
