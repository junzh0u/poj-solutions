// POJ 2240 - Arbitrage
//
// Detect whether a sequence of currency exchanges can turn one unit of some
// currency into more than one unit of that same currency.
//
// Max-product Floyd-Warshall. Currency names are arbitrary strings, so a
// std::map<string,int> assigns each one an index as it is first seen; r[i][j]
// holds the best known rate from i to j (0 = unreachable, r[i][i] = 1 for the
// empty exchange). The relaxation is the multiplicative analogue of the usual
// shortest-path one, r[i][j] = max(r[i][j], r[i][k] * r[k][j]), after which
// arbitrage exists iff some r[i][i] > 1. That is equivalent to "some simple
// cycle has product > 1": any closed walk decomposes into simple cycles whose
// products multiply to the walk's, so a walk above 1 forces a cycle above 1.
// n <= 30 makes the O(n^3) pass free; the cost is dominated by parsing.
//
// Statement notes and decisions:
//   - "more than one unit" is strict, so a cycle whose product is exactly 1 is
//     "No". Since the answer is a yes/no classification read off a floating
//     point product, the comparison uses r[i][i] > 1 + 1e-12 rather than > 1.
//     A reciprocal pair such as 0.1 and 10.0 has exact product 1 but evaluates
//     to 1.0000000000000002 in double, which a bare > 1 reports as "Yes".
//     Worst-case relative drift over a <= 30-factor product is only ~1e-14, so
//     1e-12 clears the noise by two orders of magnitude while still catching
//     any genuine profit above one part in 1e12 (a far tighter margin than the
//     commonly used 1e-9, which would discard real arbitrage).
//   - Duplicate table entries for one ordered pair keep the largest rate, and a
//     self-referential entry (c r c) beats the implicit r[i][i] = 1 when r > 1.
//   - A currency appearing only in the exchange table and not in the list of n
//     names is registered on the fly rather than ignored.
//   - Products are clamped at 1e100 so a long chain of large rates cannot reach
//     inf and then produce NaN via 0 * inf.
//   - Blank lines between cases need no handling: operator>> skips all
//     whitespace. Input ends at n == 0, or at EOF if the terminator is absent.

#include <iostream>
#include <string>
#include <map>

using namespace std;

static const int MAXN = 64;
static double r[MAXN][MAXN];

int main() {
    ios_base::sync_with_stdio(false);
    int n;
    int cs = 0;
    while ((cin >> n) && n != 0) {
        map<string, int> id;
        string name;
        int i, j, k;
        for (i = 0; i < n; ++i) {
            cin >> name;
            if (id.find(name) == id.end()) {
                int nid = (int)id.size();
                id[name] = nid;
            }
        }
        int cnt = (int)id.size();
        for (i = 0; i < MAXN; ++i)
            for (j = 0; j < MAXN; ++j)
                r[i][j] = (i == j) ? 1.0 : 0.0;
        int m;
        cin >> m;
        string a, b;
        double v;
        for (k = 0; k < m; ++k) {
            cin >> a >> v >> b;
            if (id.find(a) == id.end() && (int)id.size() < MAXN) {
                int nid = (int)id.size();
                id[a] = nid;
            }
            if (id.find(b) == id.end() && (int)id.size() < MAXN) {
                int nid = (int)id.size();
                id[b] = nid;
            }
            int u = id[a], w = id[b];
            if (v > r[u][w]) r[u][w] = v;
        }
        cnt = (int)id.size();

        for (k = 0; k < cnt; ++k)
            for (i = 0; i < cnt; ++i) {
                if (r[i][k] <= 0.0) continue;
                for (j = 0; j < cnt; ++j) {
                    double t = r[i][k] * r[k][j];
                    if (t > 1e100) t = 1e100;
                    if (t > r[i][j]) r[i][j] = t;
                }
            }

        bool ok = false;
        for (i = 0; i < cnt; ++i)
            if (r[i][i] > 1.0 + 1e-12) ok = true;

        ++cs;
        cout << "Case " << cs << ": " << (ok ? "Yes" : "No") << "\n";
    }
    return 0;
}
