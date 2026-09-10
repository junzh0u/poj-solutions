// POJ 1628 - Deduction
// Model: claude-sonnet-5
//
// Each rule S1 => S2 means: once every declaration in S1 has been deduced,
// every declaration in S2 becomes deducible too (conjunctive antecedent,
// confirmed on the discuss board: "abc => gh" means a&&b&&c implies g&&h,
// not each of a,b,c individually implying g,h).
//
// Represent each 52-letter set (a..z then A..Z) as a 64-bit bitmask.
// For each query, repeatedly scan all m rules and OR in S2's mask whenever
// S1's mask is a subset of the current known mask, until a full pass makes
// no change (standard attribute-closure fixpoint). Since the mask can only
// grow and has at most 52 bits, at most 52 passes ever change anything, so
// worst case is about 53 * m rule checks per query -- comfortably fast for
// m < 200, n < 1000.
//
// Input is read line-by-line (not token-by-token) so that a query line with
// zero declarations (an empty line, if the judge ever sends one) still lines
// up correctly with the remaining queries instead of desyncing the reader.

#include <cstdio>
#include <cstring>

typedef unsigned long long u64;

static int bitIndex(char c) {
    if (c >= 'a' && c <= 'z') return c - 'a';
    if (c >= 'A' && c <= 'Z') return c - 'A' + 26;
    return -1;
}

static u64 stringToMask(const char *s) {
    u64 mask = 0;
    for (int i = 0; s[i]; i++) {
        int b = bitIndex(s[i]);
        if (b >= 0) mask |= (u64(1) << b);
    }
    return mask;
}

static void trimLine(char *s) {
    int len = (int)strlen(s);
    while (len > 0 && (s[len-1] == '\n' || s[len-1] == '\r' || s[len-1] == ' ' || s[len-1] == '\t')) {
        s[--len] = '\0';
    }
}

static char lineBuf[512];

static bool readLine() {
    if (!fgets(lineBuf, sizeof(lineBuf), stdin)) return false;
    trimLine(lineBuf);
    return true;
}

int main() {
    int m, n;
    if (!readLine()) return 0;
    sscanf(lineBuf, "%d %d", &m, &n);

    static u64 s1mask[205], s2mask[205];
    static char t1[128], t2[128];

    for (int i = 0; i < m; i++) {
        readLine();
        t1[0] = '\0'; t2[0] = '\0';
        sscanf(lineBuf, "%127s => %127s", t1, t2);
        s1mask[i] = stringToMask(t1);
        s2mask[i] = stringToMask(t2);
    }

    for (int q = 0; q < n; q++) {
        readLine();
        u64 mask = stringToMask(lineBuf);

        bool changed = true;
        while (changed) {
            changed = false;
            for (int i = 0; i < m; i++) {
                if ((mask & s1mask[i]) == s1mask[i]) {
                    u64 nm = mask | s2mask[i];
                    if (nm != mask) {
                        mask = nm;
                        changed = true;
                    }
                }
            }
        }

        char out[64];
        int len = 0;
        for (int b = 0; b < 26; b++) if (mask & (u64(1) << b)) out[len++] = (char)('a' + b);
        for (int b = 26; b < 52; b++) if (mask & (u64(1) << b)) out[len++] = (char)('A' + (b - 26));
        out[len] = '\0';
        printf("%s\n", out);
    }

    return 0;
}
