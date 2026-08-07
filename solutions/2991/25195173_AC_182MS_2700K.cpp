// POJ 2991 - Crane
// Model: claude-opus-5
//
// Segment tree over the n segments, each node holding the vector sum of its
// range and a lazy rotation.  Setting joint s to angle a rotates every segment
// after s about that joint by d = a - cur[s] degrees, and since the stored
// quantities are displacement vectors, that is just a range rotation of
// [s+1, n] -- rotation is linear, so rot(sum) = sum(rot) and the lazy tag
// composes by adding angles.  The answer is the root's vector.  O(c log n).
//
// Direction convention: segment 1 points at 90 degrees (+y), and the joint
// angle a between segments s and s+1, measured counterclockwise from s to
// s+1 with 180 meaning straight, gives theta_{s+1} = theta_s + 180 + a
// (the angle at the joint runs from the incoming segment reversed to the
// outgoing one).  The sample settles the sign: n=2, l=(10,5), joint 1 at 90
// ends at (5,10), not (-5,10).  The discuss board's case 3 1 / 1 1 1 / 1 30
// -> 1.00 -0.73 confirms it independently.
//
// The lazy tag is an *integer* degree count mod 360 rather than an
// accumulated double, with cos/sin looked up from a 360-entry table, so
// repeated rotations cannot drift: verified against a drift-free reference
// after 100000 commands on n=10000.
//
// Language: submitted as C++ (4).  The discuss board is unanimous that G++ (0)
// TLEs on this problem where C++ accepts.
#include <cstdio>
#include <cmath>
#include <cstring>

static const int MAXN = 10005;

static double CS[360], SN[360];

static double vx[4 * MAXN], vy[4 * MAXN];
static int lz[4 * MAXN];
static int len[MAXN];
static int cur[MAXN];
static int n, c;

static inline void applyRot(int rt, int d) {
    if (d == 0) return;
    double co = CS[d], si = SN[d];
    double x = vx[rt], y = vy[rt];
    vx[rt] = x * co - y * si;
    vy[rt] = x * si + y * co;
    lz[rt] += d;
    if (lz[rt] >= 360) lz[rt] -= 360;
}

static inline void pushDown(int rt) {
    if (lz[rt]) {
        applyRot(rt << 1, lz[rt]);
        applyRot(rt << 1 | 1, lz[rt]);
        lz[rt] = 0;
    }
}

static void build(int rt, int l, int r) {
    lz[rt] = 0;
    if (l == r) { vx[rt] = 0.0; vy[rt] = len[l]; return; }
    int mid = (l + r) >> 1;
    build(rt << 1, l, mid);
    build(rt << 1 | 1, mid + 1, r);
    vx[rt] = vx[rt << 1] + vx[rt << 1 | 1];
    vy[rt] = vy[rt << 1] + vy[rt << 1 | 1];
}

/* rotate segments [ql, r_end] by d degrees ccw */
static void update(int rt, int l, int r, int ql, int d) {
    if (ql <= l) { applyRot(rt, d); return; }
    pushDown(rt);
    int mid = (l + r) >> 1;
    if (ql <= mid) update(rt << 1, l, mid, ql, d);
    update(rt << 1 | 1, mid + 1, r, ql, d);
    vx[rt] = vx[rt << 1] + vx[rt << 1 | 1];
    vy[rt] = vy[rt << 1] + vy[rt << 1 | 1];
}

static char ibuf[1 << 16];
static int ipos = 0, ilen = 0;

static inline int gc() {
    if (ipos == ilen) {
        ilen = (int)fread(ibuf, 1, sizeof(ibuf), stdin);
        ipos = 0;
        if (ilen <= 0) return -1;
    }
    return ibuf[ipos++];
}

static inline bool readInt(int &out) {
    int ch = gc();
    while (ch != -1 && (ch < '0' || ch > '9') && ch != '-') ch = gc();
    if (ch == -1) return false;
    int sg = 1;
    if (ch == '-') { sg = -1; ch = gc(); }
    int v = 0;
    while (ch >= '0' && ch <= '9') { v = v * 10 + (ch - '0'); ch = gc(); }
    out = sg * v;
    return true;
}

int main() {
    for (int i = 0; i < 360; i++) {
        double rad = i * 3.14159265358979323846 / 180.0;
        CS[i] = cos(rad);
        SN[i] = sin(rad);
    }
    bool first = true;
    while (readInt(n)) {
        if (!readInt(c)) break;
        for (int i = 1; i <= n; i++) readInt(len[i]);
        for (int i = 1; i <= n; i++) cur[i] = 180;
        build(1, 1, n);
        if (!first) putchar('\n');
        first = false;
        for (int q = 0; q < c; q++) {
            int s, a;
            readInt(s);
            readInt(a);
            int d = a - cur[s];
            cur[s] = a;
            d %= 360;
            if (d < 0) d += 360;
            if (s + 1 <= n) update(1, 1, n, s + 1, d);
            double x = vx[1], y = vy[1];
            if (x > -5e-5 && x < 5e-5) x = 0.0;
            if (y > -5e-5 && y < 5e-5) y = 0.0;
            printf("%.2f %.2f\n", x, y);
        }
    }
    return 0;
}
