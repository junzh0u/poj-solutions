// POJ 3265 - Problem Solving
// Model: claude-opus-5
//
// Month 1 has 0 money available (Hint table); every later month has exactly M,
// and nothing carries over.  Problems are solved in order, so each month
// commences a contiguous run.  A run commenced in month t costs sum(B) at the
// start of t and sum(A) at the start of t+1 -- the after-payment is due the very
// next month and cannot be deferred, so the two obligations compete for the same
// month's M.  That coupling is what makes greedy (solve as many as possible each
// month) wrong: overspending on B leaves a next month with no room past its A.
//
// State (i, j): problems 1..i commenced, 1..j fully paid, and exactly j+1..i were
// commenced last month, so SA[i]-SA[j] falls due now.  From (i, j) the next month
// pays that, then commences i+1..k while SA[i]-SA[j] + SB[k]-SB[i] <= M, reaching
// (k, i).  Every edge is one month, so BFS from (0,0) with dist 1 is the shortest
// path; the answer is dist[P][P].  O(P^2) states, O(P) transitions each.
//
// A batch whose own after-payment exceeds M is a dead end rather than an illegal
// move: it simply has no outgoing edge.  (P,P) is always reachable because every
// single B_i and A_i is <= M, so one problem per two months always works -- and
// that is also the worst case, 2P+1 = 601 months, well past P+1.
//
// Ambiguity: whether the after-payment may be postponed a month.  The statement
// says "one more payment at the start of the month after the problem is solved",
// which forbids it; the discuss board asks exactly this and answers no.  Checked
// rather than believed: a mutant that defers it reports 5 on the official sample
// instead of 6.

#include <cstdio>
#include <cstring>

static int B[305], A[305], SB[305], SA[305];
static int dist_[305][305];
static int qi[100005], qj[100005];

int main() {
    int M, P;
    if (scanf("%d %d", &M, &P) != 2) return 0;
    for (int i = 1; i <= P; ++i) {
        scanf("%d %d", &B[i], &A[i]);
        SB[i] = SB[i - 1] + B[i];
        SA[i] = SA[i - 1] + A[i];
    }
    memset(dist_, -1, sizeof(dist_));
    int head = 0, tail = 0;
    dist_[0][0] = 1;
    qi[tail] = 0; qj[tail] = 0; ++tail;
    while (head < tail) {
        int i = qi[head], j = qj[head]; ++head;
        int d = dist_[i][j];
        int needA = SA[i] - SA[j];
        if (needA > M) continue;
        int rem = M - needA;
        for (int k = i; k <= P; ++k) {
            if (SB[k] - SB[i] > rem) break;
            if (k == i && i == j) continue;
            if (dist_[k][i] == -1) {
                dist_[k][i] = d + 1;
                qi[tail] = k; qj[tail] = i; ++tail;
            }
        }
    }
    printf("%d\n", dist_[P][P]);
    return 0;
}
