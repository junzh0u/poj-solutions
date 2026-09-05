// POJ 3293 - Rectilinear polygon
// Model: claude-sonnet-5
//
// Each vertex has exactly one horizontal and one vertical incident edge.
// Vertical edges connect points sharing an x-coordinate; horizontal edges
// connect points sharing a y-coordinate. To avoid two collinear edges
// overlapping, within each x-group (sorted by y) points must pair up
// consecutively (1st-2nd, 3rd-4th, ...), and likewise within each y-group
// (sorted by x). This pairing is forced: any non-adjacent pairing would
// make two same-line segments overlap, which is never allowed.
//
// Necessary/sufficient conditions for a valid simple rectilinear polygon:
//   1. n even, and every x-group / y-group has even size (else -1).
//   2. No horizontal segment properly crosses a vertical segment away
//      from a shared endpoint ("+"-crossing). T-junction touches (an
//      endpoint of one segment landing on the open interior of the
//      other) are provably impossible here: such a touching point would
//      itself be an input point belonging to the same x- or y-group as
//      the segment it touches, and lying strictly between two adjacent
//      paired points of that group would contradict the adjacency of
//      the pairing. So checking only true crossings suffices.
//   3. The resulting 2-regular graph (alternating vertical/horizontal
//      edges) is a single cycle covering all n vertices, not several
//      disjoint cycles.
// The answer is the sum of all edge lengths (the perimeter).
//
// Crossing check done with a sweep over x: horizontal segments are
// "active" for x strictly between their endpoints; a Fenwick tree over
// compressed y keeps a count of active horizontal y's, and a vertical
// segment at x=vx queries the open y-range (vy1,vy2) for any active
// count. Events at equal x are ordered remove < query < add so the
// open-interval semantics (strict inequalities) hold exactly.
//
// Sample input's two plausible cost-orderings are not an issue here
// (no reading ambiguity in this statement); the sample was verified
// directly (expected 12).

#include <cstdio>
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

static int X[100005], Y[100005];
static int idxX[100005], idxY[100005];
static int vOther[100005], hOther[100005];

struct Fenwick {
    vector<int> t;
    int n;
    void init(int n_) { n = n_; t.assign(n + 1, 0); }
    void add(int i, int delta) {
        for (; i <= n; i += i & (-i)) t[i] += delta;
    }
    int sum(int i) const {
        int s = 0;
        for (; i > 0; i -= i & (-i)) s += t[i];
        return s;
    }
    int rangeSum(int l, int r) const { // inclusive, 1-based; if l>r -> 0
        if (l > r) return 0;
        return sum(r) - sum(l - 1);
    }
};

struct Event {
    long long x;
    int type; // 0 = remove, 1 = query, 2 = add
    int a, b; // for add/remove: a = y-index; for query: a=lowIdx, b=highIdx
};

bool cmpEvent(const Event &e1, const Event &e2) {
    if (e1.x != e2.x) return e1.x < e2.x;
    return e1.type < e2.type;
}

bool cmpX(int a, int b) {
    if (X[a] != X[b]) return X[a] < X[b];
    return Y[a] < Y[b];
}

bool cmpY(int a, int b) {
    if (Y[a] != Y[b]) return Y[a] < Y[b];
    return X[a] < X[b];
}

int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        int n;
        scanf("%d", &n);
        for (int i = 0; i < n; ++i) {
            scanf("%d %d", &X[i], &Y[i]);
            idxX[i] = idxY[i] = i;
        }

        if (n % 2 != 0) {
            cout << -1 << "\n";
            continue;
        }

        sort(idxX, idxX + n, cmpX);
        sort(idxY, idxY + n, cmpY);

        bool ok = true;
        long long answer = 0;

        // vertical segments: (p, q) with X[p]==X[q], from idxX pairing
        vector<int> vP, vQ; // point indices of each vertical segment's endpoints (lower y first)
        for (int i = 0; i < n && ok; i += 2) {
            int p = idxX[i], q = idxX[i + 1];
            if (X[p] != X[q]) { ok = false; break; }
            vOther[p] = q;
            vOther[q] = p;
            vP.push_back(p);
            vQ.push_back(q);
            answer += (long long)(Y[q] - Y[p]); // Y[q] >= Y[p] since sorted
        }
        if (!ok) { cout << -1 << "\n"; continue; }

        vector<int> hP, hQ; // point indices of each horizontal segment's endpoints (lower x first)
        for (int i = 0; i < n && ok; i += 2) {
            int p = idxY[i], q = idxY[i + 1];
            if (Y[p] != Y[q]) { ok = false; break; }
            hOther[p] = q;
            hOther[q] = p;
            hP.push_back(p);
            hQ.push_back(q);
            answer += (long long)(X[q] - X[p]); // X[q] >= X[p] since sorted
        }
        if (!ok) { cout << -1 << "\n"; continue; }

        // Compress y coordinates for the Fenwick tree.
        vector<int> ys(Y, Y + n);
        sort(ys.begin(), ys.end());
        ys.erase(unique(ys.begin(), ys.end()), ys.end());

        vector<Event> events;
        events.reserve(hP.size() * 2 + vP.size());

        for (size_t i = 0; i < hP.size(); ++i) {
            int p = hP[i], q = hQ[i];
            if (X[p] == X[q]) continue; // degenerate, cannot cross anything
            int yidx = (int)(lower_bound(ys.begin(), ys.end(), Y[p]) - ys.begin()) + 1;
            Event eAdd; eAdd.x = X[p]; eAdd.type = 2; eAdd.a = yidx; eAdd.b = 0;
            Event eRem; eRem.x = X[q]; eRem.type = 0; eRem.a = yidx; eRem.b = 0;
            events.push_back(eAdd);
            events.push_back(eRem);
        }
        for (size_t i = 0; i < vP.size(); ++i) {
            int p = vP[i], q = vQ[i];
            if (Y[p] == Y[q]) continue; // degenerate, cannot be crossed
            // open interval (Y[p], Y[q])
            int lowIdx = (int)(upper_bound(ys.begin(), ys.end(), Y[p]) - ys.begin()) + 1;
            int highIdx = (int)(lower_bound(ys.begin(), ys.end(), Y[q]) - ys.begin()); // last strictly < Y[q], 0-based count -> convert
            // lower_bound gives first index with ys[idx] >= Y[q]; elements strictly < Y[q] are indices [0, idx-1] -> 1-based [1, idx]
            Event eq; eq.x = X[p]; eq.type = 1; eq.a = lowIdx; eq.b = highIdx;
            events.push_back(eq);
        }

        sort(events.begin(), events.end(), cmpEvent);

        Fenwick fen;
        fen.init((int)ys.size());

        for (size_t i = 0; i < events.size() && ok; ++i) {
            const Event &e = events[i];
            if (e.type == 2) {
                fen.add(e.a, 1);
            } else if (e.type == 0) {
                fen.add(e.a, -1);
            } else {
                if (fen.rangeSum(e.a, e.b) > 0) { ok = false; }
            }
        }

        if (!ok) { cout << -1 << "\n"; continue; }

        // Check single cycle covering all n vertices.
        int cur = 0;
        bool useVert = true;
        int cnt = 1;
        while (true) {
            int nxt = useVert ? vOther[cur] : hOther[cur];
            cur = nxt;
            useVert = !useVert;
            if (cur == 0) break;
            ++cnt;
            if (cnt > n) break; // safety, shouldn't happen
        }

        if (cnt != n) {
            cout << -1 << "\n";
        } else {
            cout << answer << "\n";
        }
    }
    return 0;
}
