// POJ 3996 - Air Strike
// Model: claude-sonnet-5
//
// Two towers at fixed points; energy budget T is split between them as
// squared radii a=r1^2, b=r2^2 with pi*(a+b) <= T. The board (message
// 165685) confirms the statement's "use pi = 3.141" is literal: acos(-1.0)
// gives WA, only the exact rational constant 3141/1000 is accepted. A
// missile is deflected if its squared distance to tower 1 is <= a or to
// tower 2 is <= b (touching the boundary, or landing exactly on a tower
// with zero energy, counts as deflected per the statement).
//
// All coordinates have at most 3 decimal digits and magnitude <= 100, so
// they are read and rounded to integers scaled by 1000; every squared
// distance is then an exact integer in units of 1e-6. Writing S = T/pi,
// the "energy <= S" test for a value x (in those units) becomes the exact
// integer test x*3141 <= T_int*10^6 (since T is itself scaled by 1000)
// -- no floating point or epsilon is needed anywhere in the decision
// logic, which matters because two genuinely different squared distances
// can differ by as little as 1e-6, right at the scale a naive epsilon
// would blur.
//
// Coverage is monotone non-decreasing in both a and b, so an optimal
// split always spends the full budget, b = S - a. As a sweeps up from 0,
// the set tower 1 covers only grows, at a = d1_i for some missile i; the
// set tower 2 covers only shrinks, at a = S - d2_j for some missile j
// (i.e. b drops below d2_j). Between such breakpoints both sets are
// constant, so the optimum is attained exactly at one of them (plus the
// two extremes a=0 and a=S). Sweeping missiles in increasing d1 order and
// inserting them into a Fenwick tree keyed by (compressed) d2 gives, at
// each breakpoint, |A| as a running counter, |B| via a binary search over
// all d2 values shifted by the current d1 (exact-integer predicate), and
// |A ^ B| via a Fenwick prefix sum over only the inserted points. Running
// the symmetric sweep (missiles ordered by d2, checking against d1)
// covers the other family of breakpoints, including the a=S extreme as
// its own starting state (b=0).
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;
typedef long long ll;

struct BIT {
    vector<int> t;
    int n;
    void init(int n_) { n = n_; t.assign(n + 1, 0); }
    void add(int i, int v) { for (; i <= n; i += i & (-i)) t[i] += v; }
    int query(int i) const { int s = 0; for (; i > 0; i -= i & (-i)) s += t[i]; return s; }
};

struct CmpByArr {
    const vector<ll> *arr;
    bool operator()(int a, int b) const { return (*arr)[a] < (*arr)[b]; }
};

// count of elements e in sorted ascending 'arr' with (e+addend)*3141 <= BUDGET
int countLE(const vector<ll> &arr, ll addend, ll BUDGET) {
    int lo = 0, hi = (int)arr.size();
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if ((arr[mid] + addend) * 3141LL <= BUDGET) lo = mid + 1; else hi = mid;
    }
    return lo;
}

// X is the "primary" distance array (direct prefix growth as a increases),
// Y is the "secondary" one (checked via the exact budget predicate).
int sweep(const vector<ll> &X, const vector<ll> &Y, ll BUDGET) {
    int n = (int)X.size();
    vector<int> order(n);
    for (int i = 0; i < n; ++i) order[i] = i;
    CmpByArr cmp;
    cmp.arr = &X;
    sort(order.begin(), order.end(), cmp);

    vector<ll> sortedY(Y);
    sort(sortedY.begin(), sortedY.end());
    vector<ll> uniqY(sortedY);
    uniqY.erase(unique(uniqY.begin(), uniqY.end()), uniqY.end());

    vector<int> rankOfY(n);
    for (int i = 0; i < n; ++i) {
        rankOfY[i] = (int)(lower_bound(uniqY.begin(), uniqY.end(), Y[i]) - uniqY.begin()) + 1;
    }

    BIT bit;
    bit.init((int)uniqY.size());

    int best = 0;
    int cntX = 0;

    // candidate a = 0 (nothing inserted yet)
    {
        int totalY = countLE(sortedY, 0LL, BUDGET);
        int r = countLE(uniqY, 0LL, BUDGET);
        int inter = bit.query(r);
        best = max(best, cntX + totalY - inter);
    }

    int ptr = 0;
    while (ptr < n) {
        ll xv = X[order[ptr]];
        if (xv * 3141LL > BUDGET) break; // beyond S; sorted ascending, so done
        while (ptr < n && X[order[ptr]] == xv) {
            int j = order[ptr];
            bit.add(rankOfY[j], 1);
            ++cntX;
            ++ptr;
        }
        int totalY = countLE(sortedY, xv, BUDGET);
        int r = countLE(uniqY, xv, BUDGET);
        int inter = bit.query(r);
        best = max(best, cntX + totalY - inter);
    }
    return best;
}

ll toInt1000(double v) {
    if (v >= 0) return (ll)(v * 1000.0 + 0.5);
    return -(ll)(-v * 1000.0 + 0.5);
}

int main() {
    int n;
    int caseNo = 0;
    while (scanf("%d", &n) == 1) {
        if (n == 0) break;
        double x1, y1, x2, y2, T;
        scanf("%lf %lf %lf %lf %lf", &x1, &y1, &x2, &y2, &T);
        ll x1i = toInt1000(x1), y1i = toInt1000(y1);
        ll x2i = toInt1000(x2), y2i = toInt1000(y2);
        ll Ti = toInt1000(T);
        ll BUDGET = Ti * 1000000LL;

        vector<ll> d1(n), d2(n);
        for (int i = 0; i < n; ++i) {
            double mx, my;
            scanf("%lf %lf", &mx, &my);
            ll mxi = toInt1000(mx), myi = toInt1000(my);
            ll dx1 = mxi - x1i, dy1 = myi - y1i;
            ll dx2 = mxi - x2i, dy2 = myi - y2i;
            d1[i] = dx1 * dx1 + dy1 * dy1;
            d2[i] = dx2 * dx2 + dy2 * dy2;
        }

        int best = max(sweep(d1, d2, BUDGET), sweep(d2, d1, BUDGET));

        ++caseNo;
        printf("%d. %d\n", caseNo, n - best);
    }
    return 0;
}
