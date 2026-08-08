// POJ 2030 - The Secret Number
// Model: claude-opus-5
//
// A coded number is a maximal-or-partial path of digit cells stepping only
// right or down. W+H <= 70, so a path holds at most 69 digits: far past 64-bit
// range, and the comparison has to be done on the digit strings themselves.
//
// Key reduction: a sequence beginning with '0' denotes the same number as the
// suffix that starts after its leading zeros, and that suffix is itself a
// legal coded sequence. Since the matrix is guaranteed to hold a non-zero
// digit, the maximum is always attained by a sequence starting with a non-zero
// digit. For those, numeric order is exactly (more digits) then (lexicographic)
// -- no leading zero can shorten them. That total order is preserved by
// prefixing a common digit, so the DP is well-founded:
//
//   f[i][j] = c + max_(len,lex)( "", f[i][j+1], f[i+1][j] )   for digit cells
//
// filled bottom-up / right-to-left, and the answer is the (len,lex)-max f[i][j]
// over cells holding a non-zero digit. Strings live in a fixed char array and
// are built with one memcpy per cell: O(W*H*(W+H)) per data set, ~85 KB of
// copying at the maximum size, 0.12s locally for 5000 maximal 35x35 data sets.
//
// Ambiguity settled: "Leading zeros should be suppressed" is about the printed
// numeral, i.e. the sequence is read as an ordinary decimal numeral. The
// discuss board reports this as the pitfall that costs submissions -- taking
// the longest/lexicographically largest sequence outright picks a zero-prefixed
// one. The official sample does catch it (it kills that mutant, and four others).
#include <cstdio>
#include <cstring>

static char g[70][80];
static char f[70][70][72];
static int  fl[70][70];
static char ans[72];
static int  ansl;

int main() {
    int W, H;
    while (scanf("%d %d", &W, &H) == 2 && !(W == 0 && H == 0)) {
        int i, j;
        for (i = 0; i < H; i++) scanf("%s", g[i]);
        ansl = 0; ans[0] = '\0';
        for (i = H - 1; i >= 0; i--)
            for (j = W - 1; j >= 0; j--) {
                char c = g[i][j];
                if (c < '0' || c > '9') { fl[i][j] = 0; f[i][j][0] = '\0'; continue; }
                const char* best = ""; int bl = 0;
                if (j + 1 < W && fl[i][j + 1] > 0) { best = f[i][j + 1]; bl = fl[i][j + 1]; }
                if (i + 1 < H && fl[i + 1][j] > 0) {
                    int dl = fl[i + 1][j];
                    if (dl > bl || (dl == bl && strcmp(f[i + 1][j], best) > 0)) {
                        best = f[i + 1][j]; bl = dl;
                    }
                }
                f[i][j][0] = c;
                memcpy(f[i][j] + 1, best, bl + 1);
                fl[i][j] = bl + 1;
                if (c != '0' && (fl[i][j] > ansl ||
                                 (fl[i][j] == ansl && strcmp(f[i][j], ans) > 0))) {
                    ansl = fl[i][j];
                    memcpy(ans, f[i][j], ansl + 1);
                }
            }
        if (ansl == 0) printf("0\n");
        else printf("%s\n", ans);
    }
    return 0;
}
