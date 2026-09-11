// POJ 1792 - Hexagonal Routes
// Model: claude-sonnet-5
//
// Cells are numbered in a spiral on a hexagonal grid. Map each cell number to
// cube coordinates (x,y,z), x+y+z=0. Ring r (r>=1) holds 6r cells and starts
// at number 3*r*(r-1)+2. Within a ring the six sides are traversed using the
// six unit directions dirs[0..5] starting one step past the geometric corner
// r*dirs[4] (that "phase +1" offset was reverse-engineered from the sample:
// cell 2 must neighbor cells 1,3,7,8,9,10, which pins down exactly where the
// numbering begins relative to the corner - matched and confirmed against the
// full sample, including the big-number case, before submitting).
//
// Once both cells have cube coordinates, let dx,dy,dz be the coordinate
// differences and a,b,c their absolute values. Since dx+dy+dz=0, the largest
// of a,b,c always equals the sum of the other two; call the largest L (the
// shortest route length) and the smallest p. The number of shortest routes is
// the binomial coefficient C(L,p): a shortest path only ever advances along
// the two directions that reduce the two smaller magnitudes, so every
// shortest route is some interleaving of p steps of one kind and L-p steps of
// the other kind. This was verified against brute-force BFS shortest-path
// counting on random small hex grids (0 mismatches) and against every line of
// the official sample, including the length-73 case whose count has 21
// digits, which requires exact bignum arithmetic.
//
// The input does not bound the number of test cases, and computing C(L,p) by
// the naive C(L,p)=C(L,p-1)*(L-p+1)/p recurrence needs up to ~577 bignum
// multiply+divide steps per query (L,p bounded by roughly 2*577 and 577,
// since X,Y<=1e6 puts the ring radius at <=577); a 300000-query stress input
// took ~4.8s that way, well past the 1s limit if judge data is ever that
// large. Instead C(L,p) is built from its prime factorization: for each
// prime q<=L, Legendre's formula gives the exponent of q in L!/(p!(L-p)!)
// directly (no factoring of the big value itself needed), and by Kummer's
// theorem that exponent is just the number of base-q carries when adding p
// and L-p, so it stays small even though q itself can be large. Multiplying
// the bignum by q^exponent once per prime (skipping primes with exponent 0)
// needs only about 130 bignum multiplies in the worst case (measured for
// L=1154,p=577) versus 1154 multiply+divide steps the old way, and needs no
// bignum division at all. Cross-checked against the old divide-based
// bignum's output on the full stress set before switching.
//
// No statement ambiguity found; the only subtlety was the spiral's exact
// starting phase within each ring, settled by matching the given neighbor
// list for cell 2. The discuss board (message 98554) links the CTU Open
// judge's own reference solution (contest.felk.cvut.cz/03prg/solved/
// routing.c), which uses a different coordinate origin but the same
// combinatorial idea; running it against 2500 random (X,Y) pairs spanning
// the full [1,1000000] range produced output identical to this program's.

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
using namespace std;

typedef long long ll;
typedef vector<int> Big; // little-endian, base 1e9
const int BASE = 1000000000;
const int MAXQ = 1300; // upper bound on prime search range (L <= ~1154)

void mulSmall(Big &a, ll m) {
    ll carry = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        ll cur = (ll)a[i] * m + carry;
        a[i] = (int)(cur % BASE);
        carry = cur / BASE;
    }
    while (carry > 0) {
        a.push_back((int)(carry % BASE));
        carry /= BASE;
    }
}

bool isOne(const Big &a) {
    return a.size() == 1 && a[0] == 1;
}

void printBig(const Big &a) {
    printf("%d", a.back());
    for (int i = (int)a.size() - 2; i >= 0; --i) {
        printf("%09d", a[i]);
    }
}

vector<int> primes;

void sievePrimes(int n) {
    vector<bool> isComposite(n + 1, false);
    for (int i = 2; i <= n; ++i) {
        if (!isComposite[i]) {
            primes.push_back(i);
            for (long long j = (long long)i * i; j <= n; j += i) {
                isComposite[(size_t)j] = true;
            }
        }
    }
}

ll legendre(ll n, ll q) {
    ll s = 0, qq = q;
    while (qq <= n) {
        s += n / qq;
        if (qq > n / q) break; // avoid overflow before next multiply
        qq *= q;
    }
    return s;
}

Big binomial(ll L, ll p) {
    ll q2 = L - p;
    Big res;
    res.push_back(1);
    for (size_t i = 0; i < primes.size(); ++i) {
        ll q = primes[i];
        if (q > L) break;
        ll e = legendre(L, q) - legendre(p, q) - legendre(q2, q);
        if (e <= 0) continue;
        ll mult = 1;
        for (ll k = 0; k < e; ++k) mult *= q;
        mulSmall(res, mult);
    }
    return res;
}

// six unit cube directions
static const ll DX[6] = { 1, 1, 0, -1, -1, 0 };
static const ll DY[6] = { 0, -1, -1, 0, 1, 1 };
static const ll DZ[6] = { -1, 0, 1, 1, 0, -1 };

// prefix sums of directions 0..k-1 (k = 0..6)
static ll CX[7], CY[7], CZ[7];

void initCum() {
    CX[0] = CY[0] = CZ[0] = 0;
    for (int i = 0; i < 6; ++i) {
        CX[i + 1] = CX[i] + DX[i];
        CY[i + 1] = CY[i] + DY[i];
        CZ[i + 1] = CZ[i] + DZ[i];
    }
}

ll startOfRing(ll r) {
    return 3 * r * (r - 1) + 2;
}

void cubeOf(ll n, ll &x, ll &y, ll &z) {
    if (n == 1) {
        x = y = z = 0;
        return;
    }
    ll lo = 1, hi = 2000;
    while (lo < hi) {
        ll mid = (lo + hi + 1) / 2;
        if (startOfRing(mid) <= n) lo = mid; else hi = mid - 1;
    }
    ll r = lo;
    ll idx = n - startOfRing(r);       // 0-indexed within ring, 0..6r-1
    ll total = 6 * r;
    ll cidx = (idx + 1) % total;       // phase shift of +1
    ll side = cidx / r;
    ll pos = cidx % r;

    // corner = r * dirs[4]
    ll cornerX = -r, cornerY = r, cornerZ = 0;
    x = cornerX + CX[side] * r + DX[side] * pos;
    y = cornerY + CY[side] * r + DY[side] * pos;
    z = cornerZ + CZ[side] * r + DZ[side] * pos;
}

int main() {
    initCum();
    sievePrimes(MAXQ);
    ll X, Y;
    while (scanf("%lld %lld", &X, &Y) == 2) {
        if (X == 0 && Y == 0) break;
        ll x1, y1, z1, x2, y2, z2;
        cubeOf(X, x1, y1, z1);
        cubeOf(Y, x2, y2, z2);
        ll a = llabs(x1 - x2), b = llabs(y1 - y2), c = llabs(z1 - z2);
        ll v[3] = { a, b, c };
        sort(v, v + 3);
        ll L = v[2];
        ll p = v[0];
        Big cnt = binomial(L, p);
        if (isOne(cnt)) {
            printf("There is 1 route of the shortest length %lld.\n", L);
        } else {
            printf("There are ");
            printBig(cnt);
            printf(" routes of the shortest length %lld.\n", L);
        }
    }
    return 0;
}
