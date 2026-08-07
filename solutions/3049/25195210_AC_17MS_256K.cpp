// POJ 3049 - Securing the Barn
// Model: claude-opus-5
//
// Enumerate every L-subset of the given C letters. Sort (and de-duplicate) the
// letters first, then a DFS that always picks a strictly larger index emits the
// subsets in lexicographic order, which is exactly the required output order.
// A subset is printed when it holds at least one vowel and at least two
// consonants; the "characters appear in alphabetical order" rule is automatic
// because the letters are drawn in sorted order.
//
// Input framing: the discuss board reports that the C letters are not laid out
// with exactly one blank between them, so they are read with whitespace-skipping
// (`scanf(" %c")`) rather than a fixed format.
//
// Duplicate letters in the input set are collapsed: a password uses distinct
// characters, so a repeated letter could only ever produce a duplicate line.
//
// Output is emitted through a manual buffer since the number of passwords can be
// large (C(26,13) in the worst case the bounds allow).

#include <cstdio>
#include <cstring>
#include <algorithm>

static const int BUFSZ = 1 << 16;
static char obuf[BUFSZ];
static int opos = 0;

static void oflush() {
    if (opos > 0) {
        fwrite(obuf, 1, (size_t)opos, stdout);
        opos = 0;
    }
}

static void oput(const char *s, int n) {
    if (opos + n > BUFSZ) oflush();
    memcpy(obuf + opos, s, (size_t)n);
    opos += n;
}

static int L, C;
static char letters[64];
static char cur[32];

static bool isVowel(char ch) {
    return ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u';
}

static void dfs(int start, int depth, int vowels) {
    if (depth == L) {
        if (vowels >= 1 && L - vowels >= 2) {
            cur[L] = '\n';
            oput(cur, L + 1);
        }
        return;
    }
    // Need L - depth more letters out of C - start available.
    for (int i = start; i <= C - (L - depth); ++i) {
        cur[depth] = letters[i];
        dfs(i + 1, depth + 1, vowels + (isVowel(letters[i]) ? 1 : 0));
    }
}

int main() {
    if (scanf("%d %d", &L, &C) != 2) return 0;
    int n = 0;
    for (int i = 0; i < C; ++i) {
        char ch;
        if (scanf(" %c", &ch) != 1) break;
        letters[n++] = ch;
    }
    C = n;
    std::sort(letters, letters + C);
    C = (int)(std::unique(letters, letters + C) - letters);

    if (L <= C && L <= 26) dfs(0, 0, 0);
    oflush();
    return 0;
}
