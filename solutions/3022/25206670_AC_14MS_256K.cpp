// POJ 3022 - The Uxuhul Voting System
// Model: claude-sonnet-5
//
// Three stones (issues) start all black (state NNN = 0). Priests vote in
// order youngest to oldest (the input order), each flipping exactly one
// stone. The state after the last (oldest) priest's flip is the final
// outcome. Every priest knows every priest's preference order and plays
// optimally, i.e. this is a finite perfect-information sequential game
// solved by backward induction over the tiny (8-state) outcome space.
//
// Encode a state as a 3-bit integer, bit b (from MSB to LSB) representing
// issue b's outcome (0=N,1=Y); this matches the input's enumeration order
// NNN,NNY,NYN,NYY,YNN,YNY,YYN,YYY = indices 0..7.
//
// final[s] = the eventual outcome reached from state s once only the
// priests processed so far (from the oldest backward) remain to move.
// Initialize final[s] = s (identity: no more moves after the last priest).
// Process priests from last (oldest) to first (youngest): for priest k with
// preference array pref[k][0..7] (lower value = more preferred), and
// current "remaining game" outcome function `final`, the new outcome
// function is
//   newFinal[s] = final[s ^ mask]  minimizing pref[k][ final[s ^ mask] ]
// over the three possible flips (mask in {4,2,1}). Since each priest's
// preferences are a strict permutation of 1..8, the minimizer is unique
// whenever the three candidate outcomes differ, and irrelevant (all three
// give the same outcome) otherwise.
//
// After folding in all m priests this way, `final[0]` (starting state NNN)
// is the answer for the round. Verified by hand against both cases of the
// official sample (state-by-state backward induction reproduces NYY and
// NNY exactly), including a case where the middle priests' choices are
// forced ties (all three flips lead to the same downstream outcome).
//
// No statement ambiguity found; the discuss board (message ids 56453,
// 107656) holds only two unanswered "what does this mean" questions with
// no worked example, so it contributed nothing beyond confirming the
// standard reading is the one people struggled to see, not a different one.

#include <cstdio>

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    while (n--) {
        int m;
        scanf("%d", &m);
        static int pref[105][8];
        for (int k = 0; k < m; ++k) {
            for (int j = 0; j < 8; ++j) {
                scanf("%d", &pref[k][j]);
            }
        }
        int final_[8];
        for (int s = 0; s < 8; ++s) final_[s] = s;
        static const int mask[3] = {4, 2, 1};
        for (int k = m - 1; k >= 0; --k) {
            int newFinal[8];
            for (int s = 0; s < 8; ++s) {
                int bestOutcome = -1, bestVal = 1000;
                for (int b = 0; b < 3; ++b) {
                    int s2 = s ^ mask[b];
                    int outcome = final_[s2];
                    int val = pref[k][outcome];
                    if (val < bestVal) {
                        bestVal = val;
                        bestOutcome = outcome;
                    }
                }
                newFinal[s] = bestOutcome;
            }
            for (int s = 0; s < 8; ++s) final_[s] = newFinal[s];
        }
        int ans = final_[0];
        char buf[4];
        buf[0] = (ans & 4) ? 'Y' : 'N';
        buf[1] = (ans & 2) ? 'Y' : 'N';
        buf[2] = (ans & 1) ? 'Y' : 'N';
        buf[3] = '\0';
        printf("%s\n", buf);
    }
    return 0;
}
