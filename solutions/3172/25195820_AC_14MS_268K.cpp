// POJ 3172 - Scales
// Model: claude-opus-5
//
// Maximum subset sum of the weights that is <= C.  What makes this tractable
// is the stated growth property: from the third weight on, each is at least
// the sum of the previous two, so the sequence grows at least as fast as the
// Fibonacci numbers.  With every mass fitting in 31 bits that caps the number
// of weights at 46 no matter what N claims (N <= 1000 in the statement is a
// red herring), and it also makes plain DFS with a bound prune converge fast.
//
// Search order is what matters (the discuss board reports smallest-first
// TLEs where largest-first is instant, and that reproduces here): sort
// ascending, walk from the largest weight down, and try taking a weight
// before skipping it.  Taking greedily from the top drives `best` up to
// nearly C within the first root-to-leaf path, after which the bound prune
// -- current sum plus every remaining weight cannot beat `best` -- kills
// almost the whole tree.  A second shortcut closes a subtree outright when
// all remaining weights still fit under C, and the search stops entirely
// once best == C, which is unbeatable.
//
// Arithmetic: the sum of all weights can reach ~4.8e9 and overflows 32-bit
// signed, which is the board's reported cause of repeated Wrong Answers.
// Rather than depend on `long long` (POJ's C++ compiler lacks it), the
// prefix sums saturate at 2^31-1.  Saturation is only ever an over-estimate
// of what the remaining weights can contribute, and it stays above C, so
// both prunes remain conservative and the exact shortcut never misfires.
// Every quantity then lives inside unsigned 32-bit: cur <= C < 2^30 and
// pre[i] <= 2^31-1 give cur + pre[i] < 2^32.
//
// No ambiguity in the statement.  If no weight fits under C the answer is
// the empty selection, 0.

#include <cstdio>
#include <algorithm>
using namespace std;

static const unsigned SAT = 0x7FFFFFFFu; /* saturation cap, > any legal C */

static int n;
static unsigned C;
static unsigned w[1005];   /* weights, ascending */
static unsigned pre[1005]; /* pre[i] = saturated w[0] + ... + w[i] */
static unsigned best;
static bool finished;

static void dfs(int i, unsigned cur) {
    if (cur > best) best = cur;
    if (best == C) { finished = true; return; }
    if (i < 0) return;
    if (cur + pre[i] <= best) return;          /* cannot beat the incumbent */
    if (cur + pre[i] <= C) {                   /* everything left still fits */
        best = cur + pre[i];
        if (best == C) finished = true;
        return;
    }
    if (w[i] <= C - cur) {                     /* take w[i] first */
        dfs(i - 1, cur + w[i]);
        if (finished) return;
    }
    dfs(i - 1, cur);                           /* then skip it */
}

int main() {
    int i;
    if (scanf("%d %u", &n, &C) != 2) return 0;
    for (i = 0; i < n; ++i) {
        if (scanf("%u", &w[i]) != 1) { n = i; break; }
        if (w[i] > SAT) w[i] = SAT;            /* > C anyway, keeps pre sane */
    }
    sort(w, w + n);
    for (i = 0; i < n; ++i) {
        unsigned p = (i == 0) ? 0u : pre[i - 1];
        pre[i] = (p > SAT - w[i]) ? SAT : p + w[i];
    }
    best = 0;
    finished = false;
    dfs(n - 1, 0u);
    printf("%u\n", best);
    return 0;
}
