// POJ 3756 - Chess Game
// Model: claude-sonnet-5
//
// Absorbing Markov chain expectation. States are grids 0..N-1 (grid N is the
// goal / absorbing state; the FAQ thread confirms grids 0 and N never carry
// instructions). A dice roll of d in 1..6 moves i -> i+d, folded back into
// [0,N] by mirror reflection at both boundaries (the statement's own
// 98->103->97 example fixes the fold rule; "the same rule is adapted to grid
// 0" extends it to the low end). If the landed grid carries a forward or
// backward instruction, the same reflection is applied once more to resolve
// the final resting grid for the round ("no chain-effect": whatever
// instruction (if any) sits on that final grid is NOT triggered again). A
// "stop" instruction instead keeps you on that grid but burns one extra,
// dice-less round before you may roll again from the same square.
//
// Ambiguity / pitfall (confirmed via the discuss board, msgs 141307/142067/
// 170373/170379): naive Gauss elimination on the full N-equation system can
// be singular even though E[0] is perfectly well defined, because POJ's data
// can contain a *closed* trap component (a set of grids none of whose 6
// dice-transitions ever leaves the set) that is not connected to grid 0's
// reachable component. The fix is a two-phase reachability computation
// (equivalent to the closed-SCC characterization the board's last poster
// converges on: "no isolated dead end, only closed loops; contract SCCs"):
// first find every grid that has SOME path to the goal at all (plain graph
// reachability seeded from grids with a direct goal transition, grown via
// predecessors); grids outside that set can NEVER reach the goal, by any
// die-roll sequence, and are the true seed of "doomed". Then close that
// doomed set under "any predecessor of a doomed grid is itself doomed",
// since a single positive-probability transition into a sure trap already
// makes the expectation infinite even if other dice values would have led
// toward the goal. Solve the linear system only over the surviving good
// grids; if grid 0 itself ends up doomed, print "Impossible". A single
// forward fixpoint starting from "assume everyone reaches" (tried first)
// fails silently on a bare self-loop trap, since nothing ever disproves the
// optimistic assumption for a node whose only successor is itself.
//
// Verified against the sample (N=10 -> 8.81), a hand-built N=2 board with no
// instructions (symmetric board gives E0=E1=3, matches a closed-form check),
// a constructed self-loop trap reachable from grid 0 (backward instructions
// on grids 2..7 all folding back into a grid-1 self-loop; correctly reports
// "Impossible" instead of the earlier, buggy forward-fixpoint attempt's
// wrong finite answer), and a 30-trial randomized differential test against
// a from-scratch Monte Carlo simulator of the same rules (reflect, forward/
// backward/stop instructions, no chain-effect), all within simulation noise.

#include <cstdio>
#include <cstring>
#include <cmath>
using namespace std;

int N;
int fwd_[205], bwd_[205];
bool stopf[205];

int reflect_(int x) {
    if (N == 0) return 0;
    int period = 2 * N;
    x %= period;
    if (x < 0) x += period;
    if (x > N) x = period - x;
    return x;
}

// Resolve the grid landed on by a dice roll (pos1) into the final resting
// grid for the round. Returns -1 if the round ends at the goal (absorbed).
int resolve(int pos1, int &constAdd) {
    constAdd = 0;
    if (pos1 == N) return -1;
    if (fwd_[pos1] != 0) {
        int p2 = reflect_(pos1 + fwd_[pos1]);
        if (p2 == N) return -1;
        return p2;
    }
    if (bwd_[pos1] != 0) {
        int p2 = reflect_(pos1 - bwd_[pos1]);
        if (p2 == N) return -1;
        return p2;
    }
    if (stopf[pos1]) {
        constAdd = 1;
        return pos1;
    }
    return pos1;
}

static int edgeTarget[205][7];
static int edgeConst[205][7];
static bool good[205];
static int idx_[205];
static long double A[205][206];

// Predecessor lists for the "propagate doom backward" phase below.
static int predHead[205], predNext[205 * 7], predFrom[205 * 7], predCnt;

void addPred(int from, int to) {
    predFrom[predCnt] = from;
    predNext[predCnt] = predHead[to];
    predHead[to] = predCnt;
    predCnt++;
}

int main() {
    while (scanf("%d", &N) == 1) {
        memset(fwd_, 0, sizeof(fwd_));
        memset(bwd_, 0, sizeof(bwd_));
        memset(stopf, 0, sizeof(stopf));

        int nf;
        scanf("%d", &nf);
        for (int i = 0; i < nf; i++) {
            int g, s;
            scanf("%d %d", &g, &s);
            fwd_[g] = s;
        }
        int nb;
        scanf("%d", &nb);
        for (int i = 0; i < nb; i++) {
            int g, s;
            scanf("%d %d", &g, &s);
            bwd_[g] = s;
        }
        int ns;
        scanf("%d", &ns);
        for (int i = 0; i < ns; i++) {
            int g;
            scanf("%d", &g);
            stopf[g] = true;
        }

        if (N == 0) {
            printf("0.00\n");
            continue;
        }

        predCnt = 0;
        for (int i = 0; i < N; i++) predHead[i] = -1;

        bool directAbsorb[205];
        for (int i = 0; i < N; i++) {
            directAbsorb[i] = false;
            for (int d = 1; d <= 6; d++) {
                int pos1 = reflect_(i + d);
                int c = 0;
                int t = resolve(pos1, c);
                edgeTarget[i][d] = t;
                edgeConst[i][d] = c;
                if (t == -1) directAbsorb[i] = true;
                else addPred(i, t); // edge i -> t, recorded as predecessor of t
            }
        }

        // Phase 1: R = states that CAN reach the goal via some sequence of
        // dice rolls at all (plain graph reachability, ignoring how likely).
        // Seed with direct-to-goal states, then pull in any predecessor of
        // an already-reachable state.
        static bool reach[205];
        static int queue_[205];
        int qh = 0, qt = 0;
        for (int i = 0; i < N; i++) reach[i] = false;
        for (int i = 0; i < N; i++) {
            if (directAbsorb[i]) { reach[i] = true; queue_[qt++] = i; }
        }
        while (qh < qt) {
            int j = queue_[qh++];
            for (int e = predHead[j]; e != -1; e = predNext[e]) {
                int i = predFrom[e];
                if (!reach[i]) { reach[i] = true; queue_[qt++] = i; }
            }
        }

        // Phase 2: doomed = states with NO path to the goal at all (the seed,
        // i.e. !reach), closed under "any predecessor of a doomed state is
        // itself doomed" (a single positive-probability transition into a
        // sure trap already makes the expectation infinite).
        static bool doomed[205];
        qh = 0; qt = 0;
        for (int i = 0; i < N; i++) doomed[i] = false;
        for (int i = 0; i < N; i++) {
            if (!reach[i]) { doomed[i] = true; queue_[qt++] = i; }
        }
        while (qh < qt) {
            int j = queue_[qh++];
            for (int e = predHead[j]; e != -1; e = predNext[e]) {
                int i = predFrom[e];
                if (!doomed[i]) { doomed[i] = true; queue_[qt++] = i; }
            }
        }

        for (int i = 0; i < N; i++) good[i] = !doomed[i];

        if (!good[0]) {
            printf("Impossible\n");
            continue;
        }

        int cnt = 0;
        for (int i = 0; i < N; i++) idx_[i] = good[i] ? cnt++ : -1;

        for (int r = 0; r < cnt; r++)
            for (int c = 0; c <= cnt; c++)
                A[r][c] = 0.0L;

        for (int i = 0; i < N; i++) {
            if (!good[i]) continue;
            int r = idx_[i];
            A[r][r] += 1.0L;
            A[r][cnt] += 1.0L;
            for (int d = 1; d <= 6; d++) {
                int t = edgeTarget[i][d];
                int c = edgeConst[i][d];
                if (t == -1) continue;
                int rt = idx_[t];
                A[r][cnt] += (1.0L / 6.0L) * c;
                A[r][rt] -= (1.0L / 6.0L);
            }
        }

        for (int col = 0; col < cnt; col++) {
            int piv = col;
            long double best = fabsl(A[col][col]);
            for (int r = col + 1; r < cnt; r++) {
                if (fabsl(A[r][col]) > best) { best = fabsl(A[r][col]); piv = r; }
            }
            if (piv != col) {
                for (int c = 0; c <= cnt; c++) {
                    long double tmp = A[col][c];
                    A[col][c] = A[piv][c];
                    A[piv][c] = tmp;
                }
            }
            long double pv = A[col][col];
            for (int c = col; c <= cnt; c++) A[col][c] /= pv;
            for (int r = 0; r < cnt; r++) {
                if (r == col) continue;
                long double factor = A[r][col];
                if (factor == 0.0L) continue;
                for (int c = col; c <= cnt; c++) A[r][c] -= factor * A[col][c];
            }
        }

        long double ans = A[idx_[0]][cnt];
        printf("%.2Lf\n", ans);
    }
    return 0;
}
