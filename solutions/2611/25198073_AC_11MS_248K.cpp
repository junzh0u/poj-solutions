// POJ 2611 - Gas Station Numbers
// Model: claude-sonnet-5
//
// Each physical digit tile can be shown right-side-up (its printed value) or
// rotated 180 degrees. Only 0,1,8 are self-symmetric under rotation, 6/9 and
// 2/5 swap with each other, and 3/4/7 have no valid rotated form (so those
// tiles are fixed to their printed value). This means the multiset of usable
// values collapses to: fixed counts for 0,1,3,4,7,8, plus a pool of tiles
// that can independently show 6 or 9, plus a pool that can independently
// show 2 or 5.
//
// The output format fixes the decimal point immediately before the last
// digit, so we just work with the digit string of length n (n-1 integer
// digits + 1 fractional digit) and look for the lexicographically smallest
// string > the input string built from the same tile pools (classic
// next-permutation scan from the right, generalized to pooled/flexible
// digit values instead of a fixed multiset).
//
// For each candidate pivot position i (scanned from n-1 down to 0), the
// "available pool" is the total pool minus the tiles forced into the
// unchanged prefix [0, i). We look for the smallest value v > digit[i]
// obtainable from that pool; if found, position i keeps everything before
// it identical to the input, sets position i to v, and fills the rest with
// the remaining tiles in ascending order (flexible tiles shown at their
// minimum value, which is provably optimal since minimizing every flexible
// tile pointwise minimizes the sorted remainder). The first (rightmost)
// feasible pivot gives the smallest possible increase, hence the answer.
// No pivot found at any position means "The price cannot be raised."
//
// Leading-zero rule needs no special casing: whenever the pivot lands on
// position 0, the chosen value v is strictly greater than the original
// digit there (which is >= 0), so v is never '0'; and for pivot at i > 0,
// position 0 is left untouched from the (already valid) input.
//
// Verified against the sample (65.2 -> 65.5, 76.7 -> 77.6, 77.7 -> cannot
// raise) and against the discuss board's own description of the algorithm
// (message 174047), which matches this approach exactly.

#include <cstdio>
#include <cstring>

int main() {
    char line[128];
    while (fgets(line, sizeof(line), stdin)) {
        int len = (int)strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r')) line[--len] = 0;
        if (len == 0) continue;
        if (len == 1 && line[0] == '.') break;

        char digits[40];
        int n = 0;
        for (int i = 0; i < len; i++) {
            if (line[i] != '.') digits[n++] = line[i];
        }

        int totalFixed[10];
        memset(totalFixed, 0, sizeof(totalFixed));
        int totalC69 = 0, totalC25 = 0;
        for (int i = 0; i < n; i++) {
            char c = digits[i];
            if (c == '6' || c == '9') totalC69++;
            else if (c == '2' || c == '5') totalC25++;
            else totalFixed[c - '0']++;
        }

        // snapFixed[i] = prefix counts over digits[0..i-1]
        static int snapFixed[31][10];
        static int snapC69[31], snapC25[31];
        for (int d = 0; d < 10; d++) snapFixed[0][d] = 0;
        snapC69[0] = 0; snapC25[0] = 0;
        for (int i = 0; i < n; i++) {
            for (int d = 0; d < 10; d++) snapFixed[i+1][d] = snapFixed[i][d];
            snapC69[i+1] = snapC69[i];
            snapC25[i+1] = snapC25[i];
            char c = digits[i];
            if (c == '6' || c == '9') snapC69[i+1]++;
            else if (c == '2' || c == '5') snapC25[i+1]++;
            else snapFixed[i+1][c - '0']++;
        }

        int pivot = -1, chosenV = -1;
        for (int i = n - 1; i >= 0; i--) {
            int avFixed[10];
            for (int d = 0; d < 10; d++) avFixed[d] = totalFixed[d] - snapFixed[i][d];
            int avC69 = totalC69 - snapC69[i];
            int avC25 = totalC25 - snapC25[i];
            int sdig = digits[i] - '0';
            int found = -1;
            for (int v = sdig + 1; v <= 9; v++) {
                bool ok;
                if (v == 6 || v == 9) ok = avC69 > 0;
                else if (v == 2 || v == 5) ok = avC25 > 0;
                else ok = avFixed[v] > 0;
                if (ok) { found = v; break; }
            }
            if (found != -1) { pivot = i; chosenV = found; break; }
        }

        if (pivot == -1) {
            printf("The price cannot be raised.\n");
            continue;
        }

        int avFixed[10];
        for (int d = 0; d < 10; d++) avFixed[d] = totalFixed[d] - snapFixed[pivot][d];
        int avC69 = totalC69 - snapC69[pivot];
        int avC25 = totalC25 - snapC25[pivot];

        if (chosenV == 6 || chosenV == 9) avC69--;
        else if (chosenV == 2 || chosenV == 5) avC25--;
        else avFixed[chosenV]--;

        int fillCnt[10];
        for (int d = 0; d < 10; d++) fillCnt[d] = avFixed[d];
        fillCnt[2] += avC25;
        fillCnt[6] += avC69;

        char result[40];
        int pos = 0;
        for (int i = 0; i < pivot; i++) result[pos++] = digits[i];
        result[pos++] = (char)('0' + chosenV);
        for (int d = 0; d < 10; d++) {
            for (int k = 0; k < fillCnt[d]; k++) result[pos++] = (char)('0' + d);
        }

        for (int i = 0; i < pos - 1; i++) putchar(result[i]);
        putchar('.');
        putchar(result[pos - 1]);
        putchar('\n');
    }
    return 0;
}
