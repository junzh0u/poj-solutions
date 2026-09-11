// POJ 1495 - LED Test
// Model: claude-sonnet-5
//
// A 7-segment digit is displayed as one of the 10 fixed on/off patterns.
// A displayed 'Y' can only be observed if the segment is truly ON for the
// intended digit AND not (yet) burned out. A displayed 'N' means either the
// digit's segment is genuinely off, or the segment has burned out. Burned
// segments never recover and additional segments may burn during the test,
// so the set of burned-out segments is non-decreasing over the sequence.
//
// For a candidate starting digit s (so the intended digits are
// s, s-1, ..., s-(N-1)), we can simulate feasibility directly: maintain a
// "burned" mask. At each step i with intended digit d and observed row obs:
//   - if obs has 'Y' at a position where d has 'N', impossible (a segment
//     that is truly off for this digit can never show lit).
//   - if a position is already burned and d has 'Y' there, obs must show 'N'
//     (a burned segment cannot relight).
// Then any position where d has 'Y' but obs shows 'N' newly burns (or was
// already burned); update the mask accordingly and continue.
//
// This is checked for every candidate starting digit s from 9 down to
// N-1 (so the whole run of N consecutive digits stays within 0..9); MATCH
// if any candidate simulates through cleanly, otherwise MISMATCH.
//
// Ambiguity resolved via the discuss board (message 36053/38554): a chain
// of digits must be consecutive descending by exactly 1 (e.g. 6,5,4), not
// merely three independently-plausible digits (6,5,0 is not a valid count
// down and must report MISMATCH) -- confirmed against the official sample.

#include <cstdio>
#include <cstring>

static const char *DIGIT[10] = {
    "YYYYYYN", // 0
    "NYYNNNN", // 1
    "YYNYYNY", // 2
    "YYYYNNY", // 3
    "NYYNNYY", // 4
    "YNYYNYY", // 5
    "YNYYYYY", // 6
    "YYYNNNN", // 7
    "YYYYYYY", // 8
    "YYYYNYY"  // 9
};

int main() {
    int n;
    char rows[11][8];
    while (scanf("%d", &n) == 1) {
        if (n == 0) break;
        for (int i = 0; i < n; ++i) {
            scanf("%s", rows[i]);
        }
        bool matched = false;
        for (int start = 9; start >= n - 1 && !matched; --start) {
            bool burned[7] = {false, false, false, false, false, false, false};
            bool ok = true;
            for (int i = 0; i < n && ok; ++i) {
                int d = start - i;
                const char *pat = DIGIT[d];
                const char *obs = rows[i];
                for (int j = 0; j < 7; ++j) {
                    if (obs[j] == 'Y') {
                        if (pat[j] != 'Y') { ok = false; break; }
                        if (burned[j]) { ok = false; break; }
                    }
                }
                if (!ok) break;
                for (int j = 0; j < 7; ++j) {
                    if (pat[j] == 'Y' && obs[j] == 'N') burned[j] = true;
                }
            }
            if (ok) matched = true;
        }
        printf(matched ? "MATCH\n" : "MISMATCH\n");
    }
    return 0;
}
