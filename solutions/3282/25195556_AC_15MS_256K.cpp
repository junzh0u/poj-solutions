// POJ 3282 - Ferry Loading IV
// Model: claude-opus-5
//
// Direct simulation.  All cars are already queued when the ferry is repaired,
// so the two banks are static FIFO queues.  The ferry starts on the left and
// keeps shuttling while either queue is non-empty; on each arrival it loads
// the current bank's cars strictly in arrival order, stopping at the first
// car that does not fit in the remaining deck length.  Capacity is l meters,
// car lengths are centimetres, so the deck holds l*100 cm.
//
// Ambiguity: "as long as they fit" could mean skipping a too-long car and
// taking a later shorter one.  "The cars are loaded in the order of their
// arrival" forbids that, and the discuss board reports Wrong Answer for the
// greedy/sorted readings; the statement's own sample does not separate them.
//
// Cross-check: left load k departs on crossing 2k-1 and right load k on
// crossing 2k, so the answer is also max(2*L-1, 2*R) for L and R load counts.

#include <cstdio>
#include <vector>

using namespace std;

int main() {
    int c;
    if (scanf("%d", &c) != 1) return 0;
    while (c-- > 0) {
        double lmeters;
        int m;
        if (scanf("%lf %d", &lmeters, &m) != 2) break;
        long cap = (long)(lmeters * 100.0 + 0.5);

        vector<int> q[2];
        for (int i = 0; i < m; i++) {
            int len;
            char side[16];
            if (scanf("%d %15s", &len, side) != 2) return 0;
            q[side[0] == 'l' ? 0 : 1].push_back(len);
        }

        size_t idx[2];
        idx[0] = 0;
        idx[1] = 0;
        int cur = 0, crossings = 0, idleRuns = 0;
        while (idx[0] < q[0].size() || idx[1] < q[1].size()) {
            long load = 0;
            size_t before = idx[cur];
            while (idx[cur] < q[cur].size() &&
                   load + q[cur][idx[cur]] <= cap) {
                load += q[cur][idx[cur]];
                idx[cur]++;
            }
            if (idx[cur] == before) {
                /* Defensive: a car longer than the deck can never be loaded.
                   Two empty crossings in a row cannot happen on valid data. */
                if (++idleRuns >= 2) break;
            } else {
                idleRuns = 0;
            }
            crossings++;
            cur ^= 1;
        }
        printf("%d\n", crossings);
    }
    return 0;
}
