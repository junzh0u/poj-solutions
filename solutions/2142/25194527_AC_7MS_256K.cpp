// POJ 2142 - The Balance
//
// Weights may go on EITHER pan, so a pair (x, y) of a-mg and b-mg weights can
// realise d in three ways -- and the sample's "275 110 385 -> 1 1" is the case
// that is easy to miss, both weights opposite the medicine:
//
//   (1) a*x - b*y = d      (2) b*y - a*x = d      (3) a*x + b*y = d
//
// Putting the same denomination on both pans only wastes weights, so these three
// exhaust the useful configurations. Each is a linear Diophantine equation; with
// g = gcd(a, b) the extended Euclidean algorithm gives the residue class
// x == (d/g)*p (mod b/g) of every solution of a*x == d (mod b), and symmetrically
// for y. Within one case x+y is monotone in x, so the optimum is an endpoint of
// the feasible range and no scanning is needed:
//
//   case 1: x+y = (x*(a+b) - d)/b, increasing -> smallest x in the class with
//           a*x >= d (so that y >= 0).
//   case 2: mirror image with a and b swapped.
//   case 3: x+y = (x*(b-a) + d)/b, so with a < b take the smallest x in the
//           class, and with a > b the largest, subject to a*x <= d. a != b is
//           guaranteed, so the coefficient never vanishes.
//
// The three candidates are then compared on (x+y, a*x+b*y). Note the mass
// tie-break is not symmetric in a and b and does real work: case 3 always has
// mass exactly d while cases 1 and 2 have d + 2*b*y and d + 2*a*x. Two distinct
// candidates can never tie on both keys -- equal counts and equal masses force
// (a-b)*(x1-x2) = 0 -- so the answer is unique.
//
// O(log min(a,b)) per dataset.

#include <cstdio>

typedef long long ll;

static ll exgcd(ll a, ll b, ll &x, ll &y) {
    if (b == 0) { x = 1; y = 0; return a; }
    ll x1, y1;
    ll g = exgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

static ll bestX, bestY;
static bool have;

static void offer(ll x, ll y, ll a, ll b) {
    if (x < 0 || y < 0) return;
    ll cnt = x + y, mass = a * x + b * y;
    if (!have) { have = true; bestX = x; bestY = y; return; }
    ll bc = bestX + bestY, bm = a * bestX + b * bestY;
    if (cnt < bc || (cnt == bc && mass < bm)) { bestX = x; bestY = y; }
}

/* smallest non-negative r with A*r == d (mod B); m = B/g is the modulus */
static void residue(ll A, ll B, ll d, ll &r, ll &m) {
    ll p, q;
    ll g = exgcd(A, B, p, q);   /* A*p + B*q = g */
    m = B / g;
    ll t = d / g;
    r = ((p % m) * (t % m)) % m;
    r = ((r % m) + m) % m;
}

int main() {
    int ia, ib, id;
    while (scanf("%d %d %d", &ia, &ib, &id) == 3) {
        if (ia == 0 && ib == 0 && id == 0) break;
        ll a = ia, b = ib, d = id;
        have = false;
        bestX = bestY = 0;

        ll r, m;
        residue(a, b, d, r, m);          /* a*x == d (mod b) */

        /* case 1: a*x - b*y = d, y >= 0  =>  a*x >= d; x+y grows with x */
        {
            ll need = (d + a - 1) / a;
            ll x = r;
            if (x < need) x += ((need - x + m - 1) / m) * m;
            offer(x, (a * x - d) / b, a, b);
        }

        /* case 3: a*x + b*y = d, needs a*x <= d */
        {
            ll xmax = d / a;
            if (r <= xmax) {
                ll x;
                if (a < b) x = r;                       /* x+y grows with x */
                else       x = r + ((xmax - r) / m) * m; /* x+y falls with x */
                offer(x, (d - a * x) / b, a, b);
            }
        }

        /* case 2: b*y - a*x = d, x >= 0  =>  b*y >= d */
        {
            ll r2, m2;
            residue(b, a, d, r2, m2);     /* b*y == d (mod a) */
            ll need = (d + b - 1) / b;
            ll y = r2;
            if (y < need) y += ((need - y + m2 - 1) / m2) * m2;
            offer((b * y - d) / a, y, a, b);
        }

        printf("%d %d\n", (int)bestX, (int)bestY);
    }
    return 0;
}
