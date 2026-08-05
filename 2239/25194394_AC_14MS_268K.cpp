// POJ 2239 - Selecting Courses
//
// A week has 7 days x 12 classes = 84 distinct time slots. Each of the n <= 300
// courses is offered in t of those slots, and taking a course means picking
// exactly one of its slots; no two selected courses may share a slot. That is a
// maximum bipartite matching between courses and the 84 slots, solved with
// Kuhn's augmenting-path algorithm. The answer is bounded by 84 regardless of n.
//
// Statement notes:
//  - Input is a sequence of test cases terminated by EOF; the count is not given.
//  - Each course line is "t p1 q1 p2 q2 ..." with p the day (1..7) and q the
//    class (1..12). t can be up to 84 and the same (p,q) may repeat, so
//    duplicate slots are dropped while building the adjacency list.
//  - "students can select any class to go" / "at different weeks a student can
//    go to a different class" is what makes one slot per course sufficient: the
//    course occupies a single slot in the schedule, chosen freely among its
//    offerings.
#include <cstdio>
#include <cstring>
#include <vector>

using namespace std;

static const int MAXC = 305;
static const int SLOTS = 7 * 12; /* 84 */

static vector<int> adj[MAXC];
static int matchSlot[SLOTS]; /* slot -> course, -1 if free */
static bool used[SLOTS];

static bool tryKuhn(int c) {
    for (size_t i = 0; i < adj[c].size(); ++i) {
        int s = adj[c][i];
        if (used[s]) continue;
        used[s] = true;
        if (matchSlot[s] == -1 || tryKuhn(matchSlot[s])) {
            matchSlot[s] = c;
            return true;
        }
    }
    return false;
}

int main() {
    int n;
    while (scanf("%d", &n) == 1) {
        int i;
        for (i = 0; i < n; ++i) adj[i].clear();
        for (i = 0; i < n; ++i) {
            int t;
            if (scanf("%d", &t) != 1) t = 0;
            bool seen[SLOTS];
            memset(seen, 0, sizeof(seen));
            for (int j = 0; j < t; ++j) {
                int p, q;
                scanf("%d %d", &p, &q);
                int s = (p - 1) * 12 + (q - 1);
                if (s >= 0 && s < SLOTS && !seen[s]) {
                    seen[s] = true;
                    adj[i].push_back(s);
                }
            }
        }
        for (i = 0; i < SLOTS; ++i) matchSlot[i] = -1;
        int res = 0;
        for (i = 0; i < n; ++i) {
            memset(used, 0, sizeof(used));
            if (tryKuhn(i)) ++res;
        }
        printf("%d\n", res);
    }
    return 0;
}
