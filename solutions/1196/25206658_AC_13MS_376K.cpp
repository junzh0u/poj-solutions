// POJ 1196 - Twofive
// Model: claude-sonnet-5
//
// Approach: A valid 25-language word is exactly a standard Young tableau of
// the 5x5 square shape (letters 1..25 placed one per cell, rows and columns
// increasing forces the top-left cell to be 1, and every cell's value to be
// reachable by a monotone path from it -- classic SYT). We rank/unrank in
// the *row-major reading order* of the grid (i.e. lexicographic order of the
// 25-character word), which is a different order than the usual SYT growth
// order, but the standard IOI 2001 official solution's numconts trick still
// applies:
//
//   calcstate/numconts counts, given an arbitrary set of letters pinned to
//   specific (row,col) positions (the `known`/`kncol` arrays), how many full
//   valid tableaux are consistent with those pins.  It does this by growing
//   the Young diagram with letters 1,2,...,25 in *value* order: at the step
//   for letter v, if v is unpinned it may extend any row that is currently
//   shorter than the row above it (a valid Young-diagram corner); if v is
//   pinned, the only legal placement is the pinned cell, and that placement
//   is legal only if the diagram's growth has reached exactly that corner.
//   Because the letter being decided at any point of the recursion is
//   determined solely by how many cells have already been placed (=
//   sum(state)+1), the partition `state[1..5]` (row lengths so far) is a
//   sufficient memoization key for one call, given the pins are fixed for
//   the whole call.
//
// wordtonum walks the grid in row-major order; at each cell with actual
// letter cchr, for every still-unused letter k < cchr it hypothetically
// pins k to the current cell (keeping all earlier real pins) and adds
// numconts() (the count of full tableaux with that exact prefix) to the
// rank accumulator -- these are exactly the words lexicographically before
// the input sharing its prefix.  numtoword does the digit-by-digit inverse.
//
// This matches the official IOI 2001 solutions booklet's Pascal reference
// (Tero Karras), ported to C++.  No statement ambiguity found; verified
// against sample (rank 2 <-> the T/U swap word) and round-tripped locally
// for random ranks up to the full count of 701,149,020 tableaux (confirmed
// both by numconts() with no pins and by the hook-length formula for the
// 5x5 square shape: 25!/prod(hooks) = 701149020, safely under the stated
// 2^31 bound).

#include <cstdio>
#include <cstring>
#include <string>
using namespace std;

const int STATES = 6*6*6*6*6; // 7776, base-6 encoding of 5 row lengths (0..5)

long snum[STATES];
int state[6];      // state[0]=5 (virtual bound), state[1..5] = row lengths so far
int known[26];     // known[k] = row (1..5) letter k is pinned to, or -1
int kncol[26];     // column (1..5) letter k is pinned to
int reclev;

long calcstate() {
    reclev++;
    int a = 0;
    for (int i = 1; i <= 5; i++) a = a * 6 + state[i];
    if (snum[a] < 0) {
        long b = 0;
        int c = known[reclev];
        if (c < 0) {
            for (int i = 1; i <= 5; i++) {
                if (state[i-1] > state[i]) {
                    state[i]++;
                    b += calcstate();
                    state[i]--;
                }
            }
        } else {
            if (state[c-1] > state[c] && state[c] + 1 == kncol[reclev]) {
                state[c]++;
                b += calcstate();
                state[c]--;
            }
        }
        snum[a] = b;
    }
    long result = snum[a];
    reclev--;
    return result;
}

long numconts() {
    state[0] = 5;
    for (int i = 1; i <= 5; i++) state[i] = 0;
    for (int i = 0; i < STATES - 1; i++) snum[i] = -1;
    snum[STATES - 1] = 1;
    reclev = 0;
    return calcstate();
}

void clearfixed() {
    for (int i = 1; i <= 25; i++) known[i] = -1;
}

long wordtonum(const char *str) {
    // str[0..24] holds the 25 letters in row-major order
    clearfixed();
    long cnum = 1;
    for (int j = 1; j <= 5; j++) {
        for (int i = 1; i <= 5; i++) {
            int cchr = str[(i - 1) + (j - 1) * 5] - 'A' + 1;
            for (int k = 1; k < cchr; k++) {
                if (known[k] < 0) {
                    known[k] = j;
                    kncol[k] = i;
                    cnum += numconts();
                    known[k] = -1;
                }
            }
            known[cchr] = j;
            kncol[cchr] = i;
        }
    }
    return cnum;
}

string numtoword(long cnum) {
    clearfixed();
    char str[26];
    str[25] = '\0';
    for (int j = 1; j <= 5; j++) {
        for (int i = 1; i <= 5; i++) {
            for (int k = 1; k <= 25; k++) {
                if (known[k] < 0) {
                    known[k] = j;
                    kncol[k] = i;
                    str[(i - 1) + (j - 1) * 5] = char('A' + k - 1);
                    long a = numconts();
                    if (cnum - a < 1) break;
                    cnum -= a;
                    known[k] = -1;
                }
            }
        }
    }
    return string(str);
}

int main() {
    char mode[8];
    if (scanf("%s", mode) != 1) return 0;
    if (mode[0] == 'W') {
        char word[64];
        scanf("%s", word);
        printf("%ld\n", wordtonum(word));
    } else {
        long n;
        scanf("%ld", &n);
        printf("%s\n", numtoword(n).c_str());
    }
    return 0;
}
