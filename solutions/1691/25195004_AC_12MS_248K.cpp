// POJ 1691 - Painting A Board
// Model: claude-sonnet-5
// Approach: The rectangles form a DAG: rectangle B depends on rectangle A
// (A must be painted before B) whenever A lies "immediately above" B, i.e.
// A.y2 == B.y1 and their x-ranges overlap. A "brush pick-up" selects a color
// C and paints every currently-paintable rectangle of color C -- and, because
// the brush is already out, this includes chain reactions: once painting a
// color-C rectangle unblocks another color-C rectangle (whose only remaining
// unpainted predecessor was that rectangle), it gets painted in the SAME
// pick-up too. So one pick-up is the fixpoint closure of "paint any ready
// rectangle of color C" starting from the current painted set. State =
// bitmask of painted rectangles (N <= 15 so at most 2^15 states); BFS from
// the empty mask to the full mask gives the minimum number of pick-ups,
// since every transition adds bits (monotone lattice) so plain unweighted
// BFS shortest path is valid.
//
// Statement ambiguity worth recording: a naive reading of "paints all
// possible rectangles having predefined color C" as "paint the rectangles of
// color C that are ready right now, once, with no chain reaction" gives 5
// pick-ups on the sample (verified by direct simulation), not the expected
// 3. The chain-reaction (fixpoint-per-pick) reading is the one that
// reproduces the sample's answer of 3, and is the interpretation implemented
// below.
#include <cstdio>
#include <cstring>
#include <vector>
#include <queue>

using namespace std;

int main() {
    int m;
    if (scanf("%d", &m) != 1) return 0;
    while (m--) {
        int n;
        scanf("%d", &n);
        vector<int> y1(n), x1(n), y2(n), x2(n), col(n);
        for (int i = 0; i < n; ++i) {
            scanf("%d %d %d %d %d", &y1[i], &x1[i], &y2[i], &x2[i], &col[i]);
        }
        vector<int> pred(n, 0);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (i == j) continue;
                if (y2[j] == y1[i]) {
                    int lo = x1[j] > x1[i] ? x1[j] : x1[i];
                    int hi = x2[j] < x2[i] ? x2[j] : x2[i];
                    if (lo < hi) {
                        pred[i] |= (1 << j);
                    }
                }
            }
        }

        int full = (1 << n) - 1;
        vector<int> dist(1 << n, -1);
        dist[0] = 0;
        queue<int> q;
        q.push(0);
        int answer = -1;
        while (!q.empty()) {
            int s = q.front(); q.pop();
            if (s == full) { answer = dist[s]; break; }

            bool colorPresent[21];
            memset(colorPresent, 0, sizeof(colorPresent));
            for (int i = 0; i < n; ++i) {
                if (!(s & (1 << i)) && (pred[i] & s) == pred[i]) {
                    colorPresent[col[i]] = true;
                }
            }
            for (int c = 1; c <= 20; ++c) {
                if (!colorPresent[c]) continue;
                int cs = s;
                bool changed = true;
                while (changed) {
                    changed = false;
                    for (int i = 0; i < n; ++i) {
                        if (!(cs & (1 << i)) && col[i] == c && (pred[i] & cs) == pred[i]) {
                            cs |= (1 << i);
                            changed = true;
                        }
                    }
                }
                if (dist[cs] == -1) {
                    dist[cs] = dist[s] + 1;
                    q.push(cs);
                }
            }
        }
        printf("%d\n", answer);
    }
    return 0;
}
