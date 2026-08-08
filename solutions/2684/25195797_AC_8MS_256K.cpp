// POJ 2684 - Polygonal Line Search
// Model: claude-opus-5
//
// Each dataset is "n" followed by n+1 polygonal lines: line 0 is the template,
// lines 1..n are the candidates (the count line names the candidates only).
//
// A polygonal line is axis-parallel and turns 90 degrees at every vertex, so it
// is completely described up to rotation and translation by the sequence of
// segment lengths together with the sequence of turn directions (sign of the
// cross product of consecutive direction vectors). Rotation leaves both
// sequences fixed; a reflection would negate every turn sign, and reflections
// are not allowed here, which is why the turn signs are kept and not their
// absolute values.
//
// The statement also says a vertex list read backwards denotes the same line.
// Walking a path backwards reverses the length sequence and reverses the turn
// sequence *with every sign flipped*: for consecutive directions d[j], d[j+1]
// the reversed walk uses -d[j+1] then -d[j], and cross(-d[j+1], -d[j]) =
// -cross(d[j], d[j+1]). So two lines match iff their encodings are equal, or
// equal after that reversal. Those are the only two possibilities.
//
// Both length sequences must have the same size: a candidate with more vertices
// whose first segments happen to agree is not a match. Comparing only a common
// prefix is the trap the discuss board reports, and the official sample cannot
// catch it because every polygonal line inside one of its datasets has the same
// vertex count.
//
// All arithmetic is exact 32-bit integer: |coordinate| < 10000, so a cross
// product stays below 4e8.

#include <cstdio>
#include <cstdlib>
#include <vector>

using namespace std;

static const int MAXM = 64;

struct Poly {
    int m;
    int x[MAXM], y[MAXM];
};

static bool readPoly(Poly& p) {
    if (scanf("%d", &p.m) != 1) return false;
    if (p.m < 1 || p.m >= MAXM) return false;
    for (int i = 0; i < p.m; i++)
        if (scanf("%d %d", &p.x[i], &p.y[i]) != 2) return false;
    return true;
}

static void encode(const Poly& p, vector<int>& len, vector<int>& turn) {
    len.clear();
    turn.clear();
    for (int i = 0; i + 1 < p.m; i++) {
        int dx = p.x[i + 1] - p.x[i];
        int dy = p.y[i + 1] - p.y[i];
        len.push_back(abs(dx) + abs(dy));
    }
    for (int i = 0; i + 2 < p.m; i++) {
        int ax = p.x[i + 1] - p.x[i], ay = p.y[i + 1] - p.y[i];
        int bx = p.x[i + 2] - p.x[i + 1], by = p.y[i + 2] - p.y[i + 1];
        int cr = ax * by - ay * bx;
        turn.push_back(cr > 0 ? 1 : (cr < 0 ? -1 : 0));
    }
}

static bool same(const vector<int>& la, const vector<int>& ta,
                 const vector<int>& lb, const vector<int>& tb) {
    if (la.size() != lb.size()) return false;
    size_t n = la.size(), k = ta.size();
    bool ok = true;
    for (size_t i = 0; i < n && ok; i++)
        if (la[i] != lb[i]) ok = false;
    for (size_t i = 0; i < k && ok; i++)
        if (ta[i] != tb[i]) ok = false;
    if (ok) return true;
    for (size_t i = 0; i < n; i++)
        if (la[i] != lb[n - 1 - i]) return false;
    for (size_t i = 0; i < k; i++)
        if (ta[i] != -tb[k - 1 - i]) return false;
    return true;
}

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        Poly t;
        readPoly(t);
        vector<int> lt, tt;
        encode(t, lt, tt);
        for (int i = 1; i <= n; i++) {
            Poly c;
            readPoly(c);
            vector<int> lc, tc;
            encode(c, lc, tc);
            if (same(lt, tt, lc, tc)) printf("%d\n", i);
        }
        printf("+++++\n");
    }
    return 0;
}
