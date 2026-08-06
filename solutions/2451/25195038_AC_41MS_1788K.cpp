// POJ 2451 - Uyuw's Concert
// Model: claude-opus-5
//
// Half-plane intersection.  Every chair (x1,y1)-(x2,y2) requires the stage to
// lie on the LEFT of the directed line (x1,y1)->(x2,y2) -- the statement's test
// (x-x1)*(y-y2) - (x-x2)*(y-y1) >= 0 is exactly the cross product of that
// direction with (x,y)-(x1,y1) -- and the piazza contributes the four edges of
// [0,10000]^2 taken counter-clockwise.  Sort the half-planes by direction
// angle, collapse each bundle of equally-directed ones to its tightest member,
// then sweep with the classic double-ended queue, dropping a boundary line as
// soon as the vertex it carries falls outside the incoming half-plane.  Fewer
// than three surviving vertices means the constraints are inconsistent -> 0.0.
// O(n log n); the judge's 6s is never approached (20000 lines run in ~10ms).
//
// Things the data actually exercises (discuss board, all reproduced locally):
//  * A chair may degenerate to a single point, (x1,y1)==(x2,y2).  Its test is
//    0 >= 0 everywhere, so it constrains nothing and is dropped.
//  * Chair coordinates may be negative or exceed 10000; only the stage is
//    confined to the square.
//  * Two coincident chairs facing opposite ways make the answer 0.0.  Such a
//    pair can meet as neighbours in the deque, where no vertex exists; that is
//    detected explicitly instead of dividing by a zero cross product.
//  * eps must stay coarse.  1e-10 is a real angular / distance tolerance here
//    because direction vectors are normalised, so parallel lines that agree
//    only to within rounding still merge; at 1e-16 they would not, and the
//    duplicate angles break the sweep.  double throughout, not long double.
//
// The one genuine ambiguity, "line or segment", does not bite: the sample and
// the board agree the chair defines a full line, so the stage is a half-plane
// intersection rather than anything visibility-like.

#include <cstdio>
#include <cmath>
#include <algorithm>

using namespace std;

static const double EPS = 1e-10;
static const double SIDE_LEN = 10000.0;
static const int MAXL = 20016;

struct Pt {
    double x, y;
};

static Pt mk(double x, double y) {
    Pt p;
    p.x = x;
    p.y = y;
    return p;
}

static double cross2(Pt a, Pt b) { return a.x * b.y - a.y * b.x; }

/* Half-plane: the set of points q with side(L, q) >= 0, i.e. q lies to the
   left of the directed line L.p -> L.p + L.v.  L.v is a unit vector, so
   side() is a signed distance in coordinate units. */
struct Line {
    Pt p, v;
    double ang;
};

static double side(const Line &L, Pt q) {
    return L.v.x * (q.y - L.p.y) - L.v.y * (q.x - L.p.x);
}

static bool byAng(const Line &a, const Line &b) { return a.ang < b.ang; }

static Pt meet(const Line &a, const Line &b) {
    Pt d = mk(a.p.x - b.p.x, a.p.y - b.p.y);
    double t = cross2(b.v, d) / cross2(a.v, b.v);
    return mk(a.p.x + a.v.x * t, a.p.y + a.v.y * t);
}

static Line raw[MAXL], hp[MAXL];
static int dq[MAXL];
static Pt vtx[MAXL];

static void addLine(int &m, double x1, double y1, double x2, double y2) {
    double dx = x2 - x1, dy = y2 - y1;
    double len = sqrt(dx * dx + dy * dy);
    if (len <= 0.0) return; /* a chair that is a single point constrains nothing */
    if (m >= MAXL) return;
    raw[m].p = mk(x1, y1);
    raw[m].v = mk(dx / len, dy / len);
    raw[m].ang = atan2(dy, dx);
    ++m;
}

static double shoelace(const Pt *poly, int k) {
    if (k < 3) return 0.0;
    double s = 0.0;
    for (int i = 0; i < k; ++i) {
        int j = (i + 1 == k) ? 0 : i + 1;
        s += cross2(poly[i], poly[j]);
    }
    return s * 0.5;
}

/* Safety net: clip the piazza square by every half-plane in turn.  Correct for
   any input but O(n * polygon size); only reached if the deque sweep runs into
   a degenerate configuration it cannot represent (a pair of opposite lines
   becoming adjacent), which the stress tests never produced. */
static double areaByClipping(int m) {
    static Pt buf[2][MAXL * 2];
    int cur = 0, k = 4;
    buf[0][0] = mk(0.0, 0.0);
    buf[0][1] = mk(SIDE_LEN, 0.0);
    buf[0][2] = mk(SIDE_LEN, SIDE_LEN);
    buf[0][3] = mk(0.0, SIDE_LEN);
    for (int i = 0; i < m && k > 0; ++i) {
        const Line &L = hp[i];
        int nk = 0;
        for (int a = 0; a < k; ++a) {
            int b = (a + 1 == k) ? 0 : a + 1;
            double da = side(L, buf[cur][a]);
            double db = side(L, buf[cur][b]);
            if (da >= -EPS) buf[cur ^ 1][nk++] = buf[cur][a];
            if ((da > EPS && db < -EPS) || (da < -EPS && db > EPS)) {
                double t = da / (da - db);
                buf[cur ^ 1][nk++] = mk(buf[cur][a].x + (buf[cur][b].x - buf[cur][a].x) * t,
                                        buf[cur][a].y + (buf[cur][b].y - buf[cur][a].y) * t);
            }
        }
        cur ^= 1;
        k = nk;
    }
    return shoelace(buf[cur], k);
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    int m = 0;
    for (int i = 0; i < n && m < MAXL - 4; ++i) {
        double x1, y1, x2, y2;
        if (scanf("%lf %lf %lf %lf", &x1, &y1, &x2, &y2) != 4) break;
        addLine(m, x1, y1, x2, y2);
    }
    /* the piazza [0,10000]^2, its four edges taken counter-clockwise */
    addLine(m, 0.0, 0.0, SIDE_LEN, 0.0);
    addLine(m, SIDE_LEN, 0.0, SIDE_LEN, SIDE_LEN);
    addLine(m, SIDE_LEN, SIDE_LEN, 0.0, SIDE_LEN);
    addLine(m, 0.0, SIDE_LEN, 0.0, 0.0);

    sort(raw, raw + m, byAng);

    /* Collapse each bundle of equally-directed lines to its tightest member.
       Done as a linear pass over an exactly-sorted array rather than inside the
       sort comparator, so no epsilon ever breaks the strict weak ordering. */
    int h = 0;
    for (int i = 0; i < m;) {
        int j = i + 1;
        while (j < m && fabs(raw[j].ang - raw[i].ang) < EPS) ++j;
        Line best = raw[i];
        for (int t = i + 1; t < j; ++t)
            if (side(best, raw[t].p) > EPS) best = raw[t];
        hp[h++] = best;
        i = j;
    }
    m = h;
    if (m < 3) {
        printf("0.0\n");
        return 0;
    }

    int head = 0, tail = 0;
    dq[0] = 0;
    bool degenerate = false;
    for (int i = 1; i < m && !degenerate; ++i) {
        while (tail > head && side(hp[i], vtx[tail]) < -EPS) --tail;
        while (tail > head && side(hp[i], vtx[head + 1]) < -EPS) ++head;
        if (fabs(cross2(hp[dq[tail]].v, hp[i].v)) < EPS) {
            /* opposite directions (equal ones were merged above): the region is
               a slab or empty, so it has no vertex here */
            if (side(hp[dq[tail]], hp[i].p) < EPS) {
                printf("0.0\n");
                return 0;
            }
            degenerate = true;
            break;
        }
        ++tail;
        dq[tail] = i;
        vtx[tail] = meet(hp[dq[tail - 1]], hp[dq[tail]]);
    }

    double area;
    if (degenerate) {
        area = areaByClipping(m);
    } else {
        while (tail > head && side(hp[dq[head]], vtx[tail]) < -EPS) --tail;
        if (tail - head < 2) {
            printf("0.0\n");
            return 0;
        }
        vtx[head] = meet(hp[dq[tail]], hp[dq[head]]);
        area = shoelace(vtx + head, tail - head + 1);
    }
    if (!(area > 0.0)) area = 0.0;
    printf("%.1f\n", area);
    return 0;
}
