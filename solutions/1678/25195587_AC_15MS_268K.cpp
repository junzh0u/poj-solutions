// POJ 1678 - I Love this Game!
// Model: claude-opus-5
//
// Two players alternately pick numbers out of a pool.  The first pick x1 must
// satisfy a <= x1 <= b, and every later pick must exceed the previous pick by
// something in [a, b].  Since a > 0 the picked values strictly increase, so a
// value can never be taken twice and the multiset structure of the pool is
// irrelevant -- only the *set* of values matters.  For the same reason every
// reachable value is >= a > 0, so non-positive pool entries are decoys and are
// dropped.  A player who has a legal move MUST make it ("MUST NOT skip"), so
// there is no option to stop; the game ends only when no move exists.
//
// Negamax over the last value taken, from the current mover's point of view:
//     f(v) = max { w - f(w) : w in pool, v+a <= w <= v+b }, and 0 if empty.
// The answer is f(0), because "x1 in [a, b]" is exactly "x1 - 0 in [a, b]", so
// a virtual start at 0 needs no special case.
//
// Deduplicating the sorted values is what bounds the work: the candidates for
// one state are distinct integers inside a window of width b-a+1 <= 100, so the
// inner loop runs at most 100 times regardless of n.  Without the dedup a pool
// of n equal values would make it O(n^2).  Total O(n log n + n*(b-a+1)).
// Indexing by sorted position rather than by value also keeps the program
// independent of the stated [-9999, 9999] range.
//
// No statement ambiguity survived: the sample distinguishes this reading from
// the "a player may decline to move" reading (which yields 0 instead of -3 on
// case 1), from both-players-maximize, and from an unconstrained first pick.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    int t;
    // Read to EOF rather than trusting a single leading count.
    while (scanf("%d", &t) == 1) {
        while (t-- > 0) {
            int n, a, b;
            if (scanf("%d %d %d", &n, &a, &b) != 3) return 0;
            vector<int> v;
            for (int i = 0; i < n; i++) {
                int x;
                if (scanf("%d", &x) != 1) return 0;
                if (x > 0) v.push_back(x);   // values <= 0 are unreachable
            }
            sort(v.begin(), v.end());
            v.erase(unique(v.begin(), v.end()), v.end());
            int m = (int)v.size();

            vector<int> f(m, 0);
            for (int i = m - 1; i >= 0; i--) {
                int hi = v[i] + b;
                int j = (int)(lower_bound(v.begin(), v.end(), v[i] + a) - v.begin());
                int best = 0;
                bool any = false;
                for (; j < m && v[j] <= hi; j++) {
                    int val = v[j] - f[j];
                    if (!any || val > best) { best = val; any = true; }
                }
                f[i] = any ? best : 0;
            }

            // f(0): the first pick must lie in [a, b] itself.
            int j = (int)(lower_bound(v.begin(), v.end(), a) - v.begin());
            int ans = 0;
            bool any = false;
            for (; j < m && v[j] <= b; j++) {
                int val = v[j] - f[j];
                if (!any || val > ans) { ans = val; any = true; }
            }
            printf("%d\n", any ? ans : 0);
        }
    }
    return 0;
}
