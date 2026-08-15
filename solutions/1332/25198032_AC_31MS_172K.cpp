// POJ 1332 - Finding Liars
// Model: claude-sonnet-5
//
// n people in a circle; person i tests i+1 giving bit a_i (1 = "liar").
// If i is a truth-teller, a_i is exactly correct (a_i=1 iff i+1 is liar).
// If i is a liar, a_i is meaningless.
// A valid liar-set L (|L|<=t) satisfies: for i not in L, (i+1 in L) == a_i.
//
// Structural fact: from a liar at position i, the successor i+1 is free.
// If chosen truth, everything is forced deterministically until the first
// forced liar (found by scanning a_{i+1}, a_{i+2}, ... for the first 1,
// but the scan must find it within i+1..i+n-1, i.e. NOT wrap past i itself,
// else it would force x_i=0, contradicting x_i=1). So each liar position i
// has at most two possible "next liar" positions:
//   short: i+1  (always available)
//   long:  j = (first index i+r with r in [1,n-1] and a_{i+r}=1) + 1
// A valid liar-set is exactly a closed walk in this 2-out-degree graph
// whose total length is n (i.e. it tiles the whole cycle once).
// Person p can be a truth-teller in some valid cover with <=t liars iff
// there's an arc (i, j=long(i)) with i<p<j (interior position) such that
// the minimum total liar count of a cover including that arc is <=t.
// That minimum cost = 1 + (min #liars to go from j back around to i+n
// using short/long edges, computed by a forward DP -- this is the O(n^2)
// dp mentioned on the discuss board: "O(n2) is ok, dp, 516ms").
// A position with no such arc within budget t is a definite liar.
// Special case: if all a_i = 0, the all-truth-teller assignment (0 liars)
// is always valid (0<=t), so there are no definite liars.
#include <cstdio>

const int MAXN = 1005;

int n, t;
int a[MAXN];
int a3[3 * MAXN + 5];      // tripled bit array, 1-indexed
int nextOne[3 * MAXN + 10]; // nextOne[k] = smallest k'>=k with a3[k']==1 (or INF sentinel)
int bestCost[MAXN];
int localdp[2 * MAXN + 10];

int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        scanf("%d %d", &n, &t);
        bool allZero = true;
        for (int i = 1; i <= n; i++) {
            scanf("%d", &a[i]);
            if (a[i]) allZero = false;
        }
        if (allZero) {
            printf("0 0\n");
            continue;
        }

        int N3 = 3 * n;
        for (int copy = 0; copy < 3; copy++) {
            for (int k = 1; k <= n; k++) {
                a3[copy * n + k] = a[k];
            }
        }
        const int INF = 1000000000;
        nextOne[N3 + 1] = INF;
        for (int k = N3; k >= 1; k--) {
            if (a3[k] == 1) nextOne[k] = k;
            else nextOne[k] = nextOne[k + 1];
        }

        for (int p = 1; p <= n; p++) bestCost[p] = INF;

        for (int i = 1; i <= n; i++) {
            int lo = i + 1, hi = i + n - 1;
            int pos = nextOne[lo];
            if (pos > hi) continue; // no valid long jump from i

            int j = pos + 1;         // j in [i+2, i+n]
            int target = i + n;
            int start = j;
            int len = target - start; // 0 .. n-2

            for (int x = 0; x <= len; x++) localdp[x] = INF;
            localdp[0] = 0;
            for (int x = 0; x < len; x++) {
                if (localdp[x] == INF) continue;
                int k = start + x;
                // short jump
                if (localdp[x] + 1 < localdp[x + 1]) localdp[x + 1] = localdp[x] + 1;
                // long jump from k
                int lo2 = k + 1, hi2 = k + n - 1;
                int pos2 = nextOne[lo2];
                if (pos2 <= hi2) {
                    int jb = pos2 + 1;
                    if (jb <= target) {
                        int xb = jb - start;
                        if (localdp[x] + 1 < localdp[xb]) localdp[xb] = localdp[x] + 1;
                    }
                }
            }
            int cost = 1 + localdp[len];

            for (int p = i + 1; p <= j - 1; p++) {
                int actual = ((p - 1) % n) + 1;
                if (cost < bestCost[actual]) bestCost[actual] = cost;
            }
        }

        int cnt = 0, smallest = 0;
        for (int p = 1; p <= n; p++) {
            if (bestCost[p] > t) {
                cnt++;
                if (smallest == 0) smallest = p;
            }
        }
        printf("%d %d\n", cnt, smallest);
    }
    return 0;
}
