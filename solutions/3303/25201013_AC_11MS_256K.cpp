// POJ 3303 - Halls
// Model: claude-sonnet-5
// Approach: each request must be assigned exactly one hall from its candidate
// list such that no two requests with overlapping time periods [A,B] share
// the same hall. R <= 12 requests, halls indexed 1..8. This is exact list
// coloring on a (small) interval graph: precompute, for each request, a
// bitmask of the other requests it conflicts with (overlapping intervals),
// then backtrack assigning halls to requests, at every node dynamically
// picking the still-unassigned request with fewest remaining legal halls
// (MRV) and pruning immediately if any unassigned request has zero legal
// halls left (forward checking). For O(1) legality checks, maintain per-hall
// a bitmask of which requests currently occupy it; hall h is legal for
// request i iff (conflict[i] & users[h]) == 0. Dynamic MRV + forward
// checking is what keeps this fast on adversarial input. Forward checking
// alone still needs to fully permute distinct halls among 8 mutually
// overlapping requests before it can discover a 9th, also-overlapping
// request has zero halls left (an inherent 8! blowup to prove that
// particular infeasibility by search). So first apply a cheap necessary
// condition via a sweep line: the maximum number of requests simultaneously
// overlapping in time is a lower bound on the halls needed, regardless of
// candidate lists; if it exceeds 8, answer NO immediately without search.
//
// This is deliberately a genuine exhaustive-assignment search rather than
// the bitmask-DP-over-timeline-events shortcut discussed on POJ's board:
// that DP's state is only the union of currently occupied halls, with no
// record of which request holds which hall, so at a request's end event it
// can "free" a hall that is actually still held by a different, still-live
// request that merely shares it as a candidate. That produces a phantom
// state (a hall marked free while a live request still holds it) that can
// later let a genuinely conflicting request be accepted, i.e. a false YES.
// Board threads for 3303 explicitly note the judge data is weak (some
// posters passed with a hard iteration cap / timing-based cutoff on plain
// DFS), which is consistent with that DP shortcut only working by luck of
// weak data rather than by construction. This solution does not rely on
// that.
#include <cstdio>
#include <cstring>
using namespace std;

int R;
int candMask[12];
int conflictMask[12];
int hallUsers[8]; // bit i set => request i currently occupies this hall
bool used[12];    // request already assigned in this search branch

bool assignReq(int remaining) {
    if (remaining == 0) return true;

    // Forward checking + MRV: find the unassigned request with fewest
    // legal halls remaining; bail immediately if any has zero.
    int best = -1, bestMask = 0, bestCnt = 100;
    for (int i = 0; i < R; i++) {
        if (used[i]) continue;
        int forbidden = 0;
        for (int h = 0; h < 8; h++) {
            if (hallUsers[h] & conflictMask[i]) forbidden |= (1 << h);
        }
        int legal = candMask[i] & ~forbidden;
        int cnt = __builtin_popcount(legal);
        if (cnt == 0) return false; // dead end, prune now
        if (cnt < bestCnt) {
            bestCnt = cnt;
            best = i;
            bestMask = legal;
        }
    }

    used[best] = true;
    int cm = bestMask;
    while (cm) {
        int h = __builtin_ctz(cm);
        cm &= cm - 1;
        hallUsers[h] |= (1 << best);
        if (assignReq(remaining - 1)) {
            used[best] = false;
            return true;
        }
        hallUsers[h] &= ~(1 << best);
    }
    used[best] = false;
    return false;
}

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T--) {
        scanf("%d", &R);
        int A[12], B[12];
        for (int i = 0; i < R; i++) {
            int K;
            scanf("%d %d %d", &A[i], &B[i], &K);
            candMask[i] = 0;
            for (int k = 0; k < K; k++) {
                int h;
                scanf("%d", &h);
                candMask[i] |= (1 << (h - 1));
            }
        }
        for (int i = 0; i < R; i++) {
            conflictMask[i] = 0;
            for (int j = 0; j < R; j++) {
                if (i == j) continue;
                if (A[i] <= B[j] && A[j] <= B[i]) conflictMask[i] |= (1 << j);
            }
        }

        // Necessary-condition fast path: max simultaneous overlap via sweep.
        // Events: +1 at A[i], -1 at B[i]+1 (one past the inclusive end).
        // Ends must be processed before starts when their trigger times
        // coincide (e.g. one request active through time 5, encoded as an
        // end-trigger at 6, and another starting at time 6, do NOT overlap;
        // processing the end first avoids double counting them).
        int maxOverlap = 0;
        {
            // events: (time, type) type 0 = end (process first at a tie),
            // type 1 = start (process second at a tie)
            int evTime[24], evType[24];
            int ne = 0;
            for (int i = 0; i < R; i++) {
                evTime[ne] = A[i]; evType[ne] = 1; ne++;
                evTime[ne] = B[i] + 1; evType[ne] = 0; ne++;
            }
            // simple insertion sort by (time, type) ascending, R<=12 so ne<=24
            for (int a = 1; a < ne; a++) {
                int kt = evTime[a], kp = evType[a];
                int b = a - 1;
                while (b >= 0 && (evTime[b] > kt || (evTime[b] == kt && evType[b] > kp))) {
                    evTime[b+1] = evTime[b]; evType[b+1] = evType[b];
                    b--;
                }
                evTime[b+1] = kt; evType[b+1] = kp;
            }
            int cur = 0;
            for (int e = 0; e < ne; e++) {
                if (evType[e] == 1) { cur++; if (cur > maxOverlap) maxOverlap = cur; }
                else cur--;
            }
        }

        bool ok;
        if (maxOverlap > 8) {
            ok = false;
        } else {
            memset(hallUsers, 0, sizeof(hallUsers));
            memset(used, 0, sizeof(used));
            ok = assignReq(R);
        }
        printf(ok ? "YES\n" : "NO\n");
    }
    return 0;
}
