// POJ 1683 - Puzzlestan
// Model: claude-sonnet-5
//
// N item kinds (groups), M guests, 1<=N,M<=7. Guest g (0-indexed) is
// *defined* to be the owner of the g-th item of the first group (the
// output's row order must follow the first group's letter order). For
// every other group we must find a bijection position->guest consistent
// with all "same guest" / "different guest" statements about specific
// (group,position) items.
//
// Solved by backtracking: assign group 0 to the identity permutation
// (fixed by definition), then assign the remaining groups one position at
// a time, checking every statement whose other endpoint is already
// assigned (order-independent: we look at the assign[][] array's current
// fill state, using -1 as "unknown", rather than tracking a separate
// processing-order index). Groups are visited in an order chosen greedily
// by how many statements connect them to already-placed groups, which
// keeps the search from blowing up when the input's group order doesn't
// happen to match the constraint graph's connectivity.
//
// Statement ambiguity: none found in the statement text itself. The
// discuss board (message 343211) posts 6 test cases with expected outputs
// that were used here to validate the backtracking + ordering logic
// end-to-end, including cases with only positive (R) constraints and
// cases needing several rounds of elimination once positions are filled.

#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

int N, M;
char grp[7][8];
int assign_[7][7]; // assign_[i][p] = guest index, -1 if unknown
int usedMask[7];   // bitmask of guests already used within group i

struct Stmt { int gi, gj, gk, gr; bool same; };
Stmt stmts[20000];
int numStmts;

bool isConsistent(int i, int p, int g) {
    for (int s = 0; s < numStmts; s++) {
        Stmt &st = stmts[s];
        int oi = -1, op = -1;
        if (st.gi == i && st.gj == p) { oi = st.gk; op = st.gr; }
        else if (st.gk == i && st.gr == p) { oi = st.gi; op = st.gj; }
        else continue;
        if (oi == i && op == p) {
            if (!st.same) return false; // "differs from itself" is impossible
            continue;
        }
        int ov = assign_[oi][op];
        if (ov == -1) continue;
        if (st.same && ov != g) return false;
        if (!st.same && ov == g) return false;
    }
    return true;
}

int order_[7]; // processing order of groups; order_[0] is always 0

bool solve(int idx, int p) {
    if (idx == N) return true;
    int i = order_[idx];
    if (p == M) return solve(idx + 1, 0);
    for (int g = 0; g < M; g++) {
        if (usedMask[i] & (1 << g)) continue;
        if (!isConsistent(i, p, g)) continue;
        assign_[i][p] = g;
        usedMask[i] |= (1 << g);
        if (solve(idx, p + 1)) return true;
        usedMask[i] &= ~(1 << g);
        assign_[i][p] = -1;
    }
    return false;
}

int main() {
    int T;
    scanf("%d", &T);
    for (int tc = 0; tc < T; tc++) {
        scanf("%d %d", &N, &M);
        for (int i = 0; i < N; i++) scanf("%s", grp[i]);
        numStmts = 0;
        while (true) {
            int i, j, k, r; char ch;
            scanf(" %d %d %c %d %d", &i, &j, &ch, &k, &r);
            if (i == 0 && j == 0 && k == 0 && r == 0) break;
            Stmt st;
            st.gi = i - 1; st.gj = j - 1; st.gk = k - 1; st.gr = r - 1;
            st.same = (ch == 'R');
            stmts[numStmts++] = st;
        }

        for (int i = 0; i < N; i++) {
            usedMask[i] = 0;
            for (int p = 0; p < M; p++) assign_[i][p] = -1;
        }
        for (int p = 0; p < M; p++) {
            assign_[0][p] = p;
            usedMask[0] |= (1 << p);
        }

        bool placed[7];
        memset(placed, 0, sizeof(placed));
        placed[0] = true;
        order_[0] = 0;
        int connCount[7][7];
        memset(connCount, 0, sizeof(connCount));
        for (int s = 0; s < numStmts; s++) {
            connCount[stmts[s].gi][stmts[s].gk]++;
            connCount[stmts[s].gk][stmts[s].gi]++;
        }
        for (int cnt = 1; cnt < N; cnt++) {
            int best = -1, bestScore = -1;
            for (int g = 0; g < N; g++) {
                if (placed[g]) continue;
                int score = 0;
                for (int h = 0; h < N; h++) if (placed[h]) score += connCount[g][h];
                if (score > bestScore) { bestScore = score; best = g; }
            }
            order_[cnt] = best;
            placed[best] = true;
        }

        solve(1, 0); // group order_[0]==0 is preset; start real search at idx 1

        if (tc > 0) printf("\n");
        for (int g = 0; g < M; g++) {
            for (int i = 0; i < N; i++) {
                int p = -1;
                for (int pp = 0; pp < M; pp++) if (assign_[i][pp] == g) { p = pp; break; }
                putchar(grp[i][p]);
            }
            putchar('\n');
        }
    }
    return 0;
}
