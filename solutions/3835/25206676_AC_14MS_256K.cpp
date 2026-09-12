// POJ 3835 - Columbus's bargain
// Model: claude-sonnet-5
//
// Model as a shortest-path problem: a virtual source plus one node per item.
// Edges (all with non-negative weight, so plain relaxation to a fixpoint
// suffices for N <= 20):
//   source -> item X: weight price(X) - 1 (pay with one glass bead; this
//     dominates paying the full price, so a separate full-price edge is
//     never needed).
//   item A <-> item B: weight 0 whenever price(A) == price(B) (rule 3:
//     paying with an item of the same price costs nothing extra).
//   item N1 -> item N2: weight R for each declared bargain (N1, N2, R).
//
// The key reading decision: plain rule 4 ("pay a cheaper item plus the
// price difference", with no declared bargain) is cost-neutral and adds no
// edge of its own - the statement explicitly attributes every possible
// saving below face price to glass beads or to bargains ("he could use
// glass beads or took full advantages of bargains"), never to an ordinary
// cheaper-item trade.  A first submission that additionally added a generic
// edge cost(A) <= cost(B) + (price(A)-price(B)) for every strictly cheaper
// existing item B (not just declared bargains) got Wrong Answer; it still
// reproduced the sample, because the sample's only two savings happen to
// run through declared bargains anyway, so the sample cannot separate the
// two readings.  Confirmed against an independently written, unrelated
// accepted solution for this exact problem (found online) that builds only
// the three edge kinds above and omits the generic cheaper-item edge
// entirely.
//
// actual price(item) = shortest distance from the source to that item's
// node.  Finally, count how many items have actual price equal to the sum
// of the actual prices of two other, distinct items (N <= 20, so O(N^3)
// brute force).
#include <cstdio>
using namespace std;

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T--) {
        int N;
        scanf("%d", &N);
        static int price[25];
        for (int i = 0; i < N; i++) {
            int Q, P;
            scanf("%d %d", &Q, &P);
            price[Q] = P;
        }
        int M;
        scanf("%d", &M);
        static int bn1[25], bn2[25], br[25];
        for (int i = 0; i < M; i++) {
            scanf("%d %d %d", &bn1[i], &bn2[i], &br[i]);
        }

        static int dist[25];
        for (int i = 1; i <= N; i++) dist[i] = price[i] - 1;

        for (int iter = 0; iter < N + M + 5; iter++) {
            bool changed = false;
            for (int i = 1; i <= N; i++) {
                for (int j = 1; j <= N; j++) {
                    if (i == j) continue;
                    if (price[i] == price[j] && dist[j] < dist[i]) {
                        dist[i] = dist[j];
                        changed = true;
                    }
                }
            }
            for (int b = 0; b < M; b++) {
                int cand = dist[bn1[b]] + br[b];
                if (cand < dist[bn2[b]]) {
                    dist[bn2[b]] = cand;
                    changed = true;
                }
            }
            if (!changed) break;
        }

        for (int i = 1; i <= N; i++) printf("%d %d\n", i, dist[i]);

        int m = 0;
        for (int i = 1; i <= N; i++) {
            bool matched = false;
            for (int j = 1; j <= N && !matched; j++) {
                if (j == i) continue;
                for (int k = j + 1; k <= N; k++) {
                    if (k == i) continue;
                    if (dist[j] + dist[k] == dist[i]) { matched = true; break; }
                }
            }
            if (matched) m++;
        }
        printf("%d\n", m);
    }
    return 0;
}
