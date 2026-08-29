// POJ 2036 - I Conduit!
// Model: claude-sonnet-5
//
// Group segments by the infinite line they lie on, using exact integer
// arithmetic (coordinates have at most 2 decimal places, so scale by 100
// and round to integers -- avoids the floating point precision issues
// the discuss board repeatedly reports for this problem). A line is
// identified by a normalized (A,B,C) with A*x+B*y+C=0, reduced by gcd
// and sign-canonicalized. Within a line, segments are parametrized by
// one coordinate (x if the line is not vertical, else y) and merged as
// closed intervals: touching endpoints count as overlap (the sample
// with segments sharing exactly one endpoint on a line answers 1). The
// answer is the total number of merged intervals summed over all lines.
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;
typedef long long ll;

static ll igcd(ll a, ll b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b) { ll t = a % b; a = b; b = t; }
    return a;
}

struct Key {
    ll A, B, C;
    bool operator<(const Key& o) const {
        if (A != o.A) return A < o.A;
        if (B != o.B) return B < o.B;
        return C < o.C;
    }
};

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n) {
        map<Key, vector<pair<ll, ll> > > lines;
        for (int i = 0; i < n; ++i) {
            double x1, y1, x2, y2;
            scanf("%lf %lf %lf %lf", &x1, &y1, &x2, &y2);
            // Coordinates are in [0, 1000], so plain truncation after adding
            // 0.5 is a safe round-to-nearest (no negative-sign handling
            // needed, and no dependency on C99's llround).
            ll X1 = (ll)(x1 * 100.0 + 0.5);
            ll Y1 = (ll)(y1 * 100.0 + 0.5);
            ll X2 = (ll)(x2 * 100.0 + 0.5);
            ll Y2 = (ll)(y2 * 100.0 + 0.5);

            ll dx = X2 - X1, dy = Y2 - Y1;
            ll A = dy, B = -dx;
            ll C = -(A * X1 + B * Y1);
            ll g = igcd(igcd(A, B), C);
            if (g != 0) { A /= g; B /= g; C /= g; }
            if (A < 0 || (A == 0 && B < 0)) { A = -A; B = -B; C = -C; }

            ll t1, t2;
            if (B != 0) { t1 = X1; t2 = X2; }
            else { t1 = Y1; t2 = Y2; }
            if (t1 > t2) swap(t1, t2);

            Key k; k.A = A; k.B = B; k.C = C;
            lines[k].push_back(make_pair(t1, t2));
        }

        int total = 0;
        for (map<Key, vector<pair<ll, ll> > >::iterator it = lines.begin(); it != lines.end(); ++it) {
            vector<pair<ll, ll> >& v = it->second;
            sort(v.begin(), v.end());
            ll curEnd = 0;
            bool have = false;
            for (size_t i = 0; i < v.size(); ++i) {
                if (!have || v[i].first > curEnd) {
                    total++;
                    curEnd = v[i].second;
                    have = true;
                } else {
                    if (v[i].second > curEnd) curEnd = v[i].second;
                }
            }
        }
        printf("%d\n", total);
    }
    return 0;
}
