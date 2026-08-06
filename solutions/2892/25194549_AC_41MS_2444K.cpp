// POJ 2892 - Tunnel Warfare
//
// Villages 1..n sit in a line. "D x" destroys village x, splitting the line;
// "Q x" asks for the size of the maximal run of intact villages containing x;
// "R" rebuilds the village destroyed last (LIFO).
//
// Only the destroyed positions matter: an intact x belongs to the run bounded
// by the nearest destroyed village on each side, so with L = max{d in dead : d < x}
// (0 if none) and R = min{d in dead : d > x} (n+1 if none) the answer is
// R - L - 1. That is a predecessor/successor query, which std::set gives in
// O(log n); the ends of the line are handled by the sentinels 0 and n+1 rather
// than as special cases. "R" needs no structure beyond a stack of the D's.
//
// Statement ambiguity: nothing says a D targets an intact village, and it never
// defines what R should undo if the same village were destroyed twice. Each
// history entry therefore records whether its D actually changed the set, and R
// only rebuilds when it did -- so a doubly-destroyed village stays destroyed
// until the first of its destructions is undone. It also leaves R on an empty
// history as a no-op. The judge's data appears not to exercise either case.
//
// O((n + m) log n) time, O(n + m) memory; 41MS / 2444K against a 1000MS limit.

#include <cstdio>
#include <set>
#include <vector>
#include <utility>

int main() {
    int n, m;
    while (scanf("%d %d", &n, &m) == 2) {
        std::set<int> dead;                      /* destroyed villages */
        std::vector<std::pair<int, bool> > hist; /* (village, did this D actually destroy it) */
        hist.reserve(m);
        for (int i = 0; i < m; ++i) {
            char op[8];
            if (scanf("%s", op) != 1) return 0;
            if (op[0] == 'D') {
                int x;
                if (scanf("%d", &x) != 1) return 0;
                bool fresh = dead.insert(x).second;
                hist.push_back(std::make_pair(x, fresh));
            } else if (op[0] == 'Q') {
                int x;
                if (scanf("%d", &x) != 1) return 0;
                if (dead.count(x)) {
                    printf("0\n");
                } else {
                    /* first destroyed village strictly right of x, else n+1 */
                    std::set<int>::const_iterator it = dead.upper_bound(x);
                    int right = (it == dead.end()) ? n + 1 : *it;
                    /* last destroyed village strictly left of x, else 0 */
                    int left = 0;
                    if (it != dead.begin()) {
                        --it;
                        left = *it;
                    }
                    printf("%d\n", right - left - 1);
                }
            } else { /* 'R' */
                if (!hist.empty()) {
                    std::pair<int, bool> last = hist.back();
                    hist.pop_back();
                    if (last.second) dead.erase(last.first);
                }
            }
        }
    }
    return 0;
}
