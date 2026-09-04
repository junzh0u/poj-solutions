// POJ 1530 - Slurpys
// Model: claude-sonnet-5
// Approach: The grammar (Slump, Slimp, Slurpy) is unambiguous under
// single-character lookahead: a Slump's F-run must be consumed maximally
// (any leftover F cannot start a nested Slump or 'G'), and a Slimp's second
// character (H / B / D-or-E) uniquely selects one of its three productions.
// So a simple recursive-descent parser deterministically finds the unique
// parse (if any) of Slimp starting at 0, then requires the remainder to be
// exactly one Slump. No statement ambiguity found; verified against an
// interval-DP reference that tries all splits (does not assume determinism)
// on randomized small-alphabet strings.
#include <cstdio>
#include <cstring>
using namespace std;

char s[70];
int len;

int parseSlump(int pos) {
    if (pos >= len) return -1;
    if (s[pos] != 'D' && s[pos] != 'E') return -1;
    pos++;
    int cnt = 0;
    while (pos < len && s[pos] == 'F') { pos++; cnt++; }
    if (cnt == 0) return -1;
    if (pos >= len) return -1;
    if (s[pos] == 'G') return pos + 1;
    if (s[pos] == 'D' || s[pos] == 'E') return parseSlump(pos);
    return -1;
}

int parseSlimp(int pos) {
    if (pos >= len) return -1;
    if (s[pos] != 'A') return -1;
    pos++;
    if (pos >= len) return -1;
    if (s[pos] == 'H') return pos + 1;
    if (s[pos] == 'B') {
        int p2 = parseSlimp(pos + 1);
        if (p2 < 0) return -1;
        if (p2 >= len || s[p2] != 'C') return -1;
        return p2 + 1;
    }
    if (s[pos] == 'D' || s[pos] == 'E') {
        int p2 = parseSlump(pos);
        if (p2 < 0) return -1;
        if (p2 >= len || s[p2] != 'C') return -1;
        return p2 + 1;
    }
    return -1;
}

int main() {
    int n;
    scanf("%d", &n);
    printf("SLURPYS OUTPUT\n");
    while (n--) {
        scanf("%s", s);
        len = strlen(s);
        bool ok = false;
        int p1 = parseSlimp(0);
        if (p1 >= 0) {
            int p2 = parseSlump(p1);
            if (p2 == len) ok = true;
        }
        printf(ok ? "YES\n" : "NO\n");
    }
    printf("END OF OUTPUT\n");
    return 0;
}
