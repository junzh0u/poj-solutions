// POJ 1924 - The Treasure
// Model: claude-sonnet-5
//
// Time-expanded BFS. Since the answer is capped at 100 seconds (otherwise
// "impossible"), we only need each monster's position for t = 0..100, found
// directly by indexing its cyclic sequence with t % s -- no need to compute
// the true period (LCM of the s_i can be astronomically large).
//
// State[t] = set of cells the player could occupy at time t having survived
// every earlier turn.  Turn t -> t+1: monsters first jump to their time
// (t+1) positions; a cell is "unsafe" at time t+1 if a monster now occupies
// it or it is one of the 8 cells around an aggressive monster.  Any player
// currently standing on a cell that becomes unsafe dies immediately, even if
// he intended to move away this turn (POJ board msg 8168/8171 confirms the
// monsters move before the player, so this pre-move death check is real).
// Survivors then attempt: stay, walk one step in one of 8 directions, or run
// two steps in one of 8 directions.  A walk's destination, and a run's
// intermediate and destination cell, must all be non-rock and not unsafe at
// t+1 (board msg 140851 + Fig.3/Fig.4 confirm the path check covers rocks,
// monster occupancy, and aggressive attack cells alike).
//
// Reaching the treasure at any time t (0..100) is an immediate win -- no
// need to check survival past that point (board msg 140851: "as long as you
// get the chance to grab the treasure it counts, even if eaten a second
// later"). Ambiguity check: thread 76432/76433/114388/114389 reports a 7-
// second path for the sample that looked plausible but actually dies to a
// monster mid-route; the algorithm above (checking every step's safety, not
// just endpoints) reproduces the official 8-second answer.
//
// Monster type (aggressive 'a' vs non-aggressive 'n') is read off the grid
// cell at each monster's own first listed position, so it does not depend on
// assuming the input's monster-sequence order matches the "sort by row then
// column" numbering the statement describes.

#include <cstdio>
#include <cstring>
#include <vector>
using namespace std;

const int MAXT = 100;

int N, M;
char grid[105][105];
unsigned char unsafeGrid[MAXT + 1][105 * 105];
unsigned char stateGrid[MAXT + 1][105 * 105];

int idx(int r, int c) { return r * M + c; }

int main() {
    while (scanf("%d %d", &N, &M) == 2) {
        if (N == 0 && M == 0) break;
        for (int i = 0; i < N; i++) {
            scanf("%s", grid[i]);
        }
        int sr = -1, sc = -1, tr = -1, tc = -1;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                if (grid[i][j] == 'p') { sr = i; sc = j; }
                else if (grid[i][j] == 't') { tr = i; tc = j; }
            }
        }
        int p;
        scanf("%d", &p);
        vector<vector<pair<int,int> > > seqs(p);
        vector<int> aggressive(p, 0);
        for (int i = 0; i < p; i++) {
            int s;
            scanf("%d", &s);
            seqs[i].resize(s);
            for (int k = 0; k < s; k++) {
                int x, y;
                scanf("%d %d", &x, &y);
                seqs[i][k] = make_pair(x - 1, y - 1);
            }
            int r0 = seqs[i][0].first, c0 = seqs[i][0].second;
            aggressive[i] = (grid[r0][c0] == 'a') ? 1 : 0;
        }

        int cells = N * M;
        for (int t = 0; t <= MAXT; t++) {
            memset(unsafeGrid[t], 0, cells);
        }
        static const int dr8[8] = {-1,-1,-1,0,0,1,1,1};
        static const int dc8[8] = {-1,0,1,-1,1,-1,0,1};
        for (int t = 0; t <= MAXT; t++) {
            for (int i = 0; i < p; i++) {
                int s = (int)seqs[i].size();
                int mi = t % s;
                int mr = seqs[i][mi].first, mc = seqs[i][mi].second;
                unsafeGrid[t][idx(mr, mc)] = 1;
                if (aggressive[i]) {
                    for (int d = 0; d < 8; d++) {
                        int nr = mr + dr8[d], nc = mc + dc8[d];
                        if (nr >= 0 && nr < N && nc >= 0 && nc < M) {
                            unsafeGrid[t][idx(nr, nc)] = 1;
                        }
                    }
                }
            }
        }

        for (int t = 0; t <= MAXT; t++) {
            memset(stateGrid[t], 0, cells);
        }
        stateGrid[0][idx(sr, sc)] = 1;

        int answer = -1;
        for (int t = 0; t <= MAXT; t++) {
            if (stateGrid[t][idx(tr, tc)]) { answer = t; break; }
            if (t == MAXT) break;
            unsigned char *cur = stateGrid[t];
            unsigned char *nxt = stateGrid[t + 1];
            unsigned char *uns = unsafeGrid[t + 1];
            for (int r = 0; r < N; r++) {
                for (int c = 0; c < M; c++) {
                    if (!cur[idx(r, c)]) continue;
                    if (uns[idx(r, c)]) continue; // died before moving
                    // stay
                    nxt[idx(r, c)] = 1;
                    // walk
                    for (int d = 0; d < 8; d++) {
                        int nr = r + dr8[d], nc = c + dc8[d];
                        if (nr < 0 || nr >= N || nc < 0 || nc >= M) continue;
                        if (grid[nr][nc] == '#') continue;
                        if (uns[idx(nr, nc)]) continue;
                        nxt[idx(nr, nc)] = 1;
                    }
                    // run
                    for (int d = 0; d < 8; d++) {
                        int mr = r + dr8[d], mc = c + dc8[d];
                        int nr = r + 2 * dr8[d], nc = c + 2 * dc8[d];
                        if (mr < 0 || mr >= N || mc < 0 || mc >= M) continue;
                        if (nr < 0 || nr >= N || nc < 0 || nc >= M) continue;
                        if (grid[mr][mc] == '#' || grid[nr][nc] == '#') continue;
                        if (uns[idx(mr, mc)] || uns[idx(nr, nc)]) continue;
                        nxt[idx(nr, nc)] = 1;
                    }
                }
            }
        }

        static bool firstCase = true;
        if (!firstCase) printf("\n");
        firstCase = false;
        if (answer == -1) printf("impossible\n");
        else printf("%d\n", answer);
    }
    return 0;
}
