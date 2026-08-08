// POJ 1414 - Life Line
// Model: claude-opus-5
//
// Triangular Go-like board: row i (0-based) holds i+1 vertices and vertex
// (i,j) has up to six neighbours -- (i,j-1), (i,j+1), (i-1,j-1), (i-1,j),
// (i+1,j), (i+1,j+1).  Figure 1's edge drawing and figure 2's group outlines
// confirm that lattice.  A group is a maximal set of adjacent equal-numbered
// stones; a group with no adjacent empty vertex dies.
//
// The board has at most 55 vertices, so the answer is found by brute force:
// try player C's stone on every empty vertex, flood fill the whole board, and
// score (dead stones of others) - (dead stones of C).
//
// Ambiguity: whether removal is simultaneous or Go-style (opponent groups
// resolved first, so a suicidal placement survives).  The statement's rule 2
// is unconditional, and the official sample settles it -- case 5 (board full
// of 1s, player 2 fills the last vertex) answers 8 = 9 captured - 1 own,
// whereas the Go-style reading answers 9 and the own-group-first reading -1.
// Cases 2, 3 and 10 separate them too.
//
// A player with no legal move (full board) cannot occur in a game "still in
// progress"; 0 is printed defensively rather than leaving `best` unset.

#include <cstdio>
#include <cstring>

static int N, C;
static int b[12][12];      /* b[i][j], 0-based, valid for 0<=j<=i<N */
static int comp[12][12];
static int stk[128][2];

static const int DI[6] = { 0,  0, -1, -1,  1,  1};
static const int DJ[6] = {-1,  1, -1,  0,  0,  1};

static bool inside(int i, int j) { return i >= 0 && i < N && j >= 0 && j <= i; }

/* Score of the position in `b`: (removed stones not owned by C) - (removed
 * stones owned by C).  All zero-liberty groups are removed simultaneously. */
static int evaluate()
{
    int i, j, k, d, top, gain = 0, loss = 0;
    memset(comp, 0, sizeof(comp));
    for (i = 0; i < N; i++) {
        for (j = 0; j <= i; j++) {
            if (b[i][j] == 0 || comp[i][j]) continue;
            /* flood fill the group of equal-numbered adjacent stones */
            int color = b[i][j], size = 0;
            bool alive = false;
            top = 0;
            stk[top][0] = i; stk[top][1] = j; top++;
            comp[i][j] = 1;
            while (top > 0) {
                top--;
                int ci = stk[top][0], cj = stk[top][1];
                size++;
                for (d = 0; d < 6; d++) {
                    int ni = ci + DI[d], nj = cj + DJ[d];
                    if (!inside(ni, nj)) continue;
                    if (b[ni][nj] == 0) { alive = true; continue; }
                    if (b[ni][nj] != color || comp[ni][nj]) continue;
                    comp[ni][nj] = 1;
                    stk[top][0] = ni; stk[top][1] = nj; top++;
                }
            }
            if (!alive) {
                if (color == C) loss += size; else gain += size;
            }
        }
    }
    k = gain - loss;
    return k;
}

int main()
{
    while (scanf("%d %d", &N, &C) == 2 && (N != 0 || C != 0)) {
        int i, j;
        for (i = 0; i < N; i++)
            for (j = 0; j <= i; j++)
                scanf("%d", &b[i][j]);
        int best = 0;
        bool any = false;
        for (i = 0; i < N; i++) {
            for (j = 0; j <= i; j++) {
                if (b[i][j] != 0) continue;
                b[i][j] = C;
                int v = evaluate();
                b[i][j] = 0;
                if (!any || v > best) { best = v; any = true; }
            }
        }
        if (!any) best = 0;
        printf("%d\n", best);
    }
    return 0;
}
