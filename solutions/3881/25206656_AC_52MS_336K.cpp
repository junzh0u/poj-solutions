// POJ 3881 - Happy Telephones
// Model: claude-sonnet-5
// Approach: each call occupies the half-open second-interval [Start, Start+Duration).
// A call is "active during at least one second" of a query interval [Qs, Qe) iff the
// two half-open integer intervals intersect, i.e. NOT (call ends before Qs starts, or
// call starts at/after Qe). So for each query: answer = N - (# calls with end <= Qs)
// - (# calls with start >= Qe). Sorting call starts and ends once per test case and
// binary-searching per query gives O((N+M) log N) per case, well inside the limits
// (N < 10000, M < 100). Source/Destination are read but unused (confirmed by the
// problem's discuss board: they carry no information relevant to the answer).
#include <cstdio>
#include <algorithm>
using namespace std;

int starts[10005], ends[10005];

int main() {
    int n, m;
    while (scanf("%d %d", &n, &m) == 2) {
        if (n == 0 && m == 0) break;
        for (int i = 0; i < n; ++i) {
            int src, dst, s, d;
            scanf("%d %d %d %d", &src, &dst, &s, &d);
            starts[i] = s;
            ends[i] = s + d;
        }
        sort(starts, starts + n);
        sort(ends, ends + n);
        for (int q = 0; q < m; ++q) {
            int qs, qd;
            scanf("%d %d", &qs, &qd);
            int qe = qs + qd;
            // count ends[i] <= qs
            int endLE = (int)(upper_bound(ends, ends + n, qs) - ends);
            // count starts[i] >= qe
            int startGE = n - (int)(lower_bound(starts, starts + n, qe) - starts);
            int ans = n - endLE - startGE;
            printf("%d\n", ans);
        }
    }
    return 0;
}
