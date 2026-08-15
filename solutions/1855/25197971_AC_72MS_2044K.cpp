// POJ 1855 - Mint
// Model: claude-sonnet-5
// A table leg of length L can be built from coin type i iff thickness[i] divides L
// (using L/thickness[i] coins of that type, at least one). Four legs of equal
// length L are buildable (with four distinct coin types) iff at least 4 of the
// n coin thicknesses divide L. So for a candidate L, define count(L) = number
// of coin thicknesses dividing L; we need count(L) >= 4.
//
// Key fact: count(L) >= 4 implies L is a common multiple of some 4-subset of
// the coin thicknesses, i.e. a multiple of lcm(a,b,c,d) for that subset. So the
// set of feasible L is exactly the union, over all C(n,4) 4-subsets, of the
// multiples of that subset's LCM. n <= 50 so C(50,4) = 230300, small enough to
// enumerate all subset LCMs once per test case (with overflow-safe LCM, since
// the board reports leg lengths can reach ~2e9, well within 64-bit headroom).
// For each query height h, the greatest length not exceeding h is
// max over subset-LCMs L of floor(h/L)*L, and the smallest not less than h is
// the min over subset-LCMs L of ceil(h/L)*L. Scanning all stored LCMs per
// query (<=230300 * 10 per test case) is fast.
//
// Ambiguity check: sample coins 50,100,200,400 (n=4, only 1 combination),
// lcm=400. height 1000 -> 800/1200; height 2000 -> 2000/2000. Matches sample
// output exactly, confirming the "at least 4 divide L" reading (not "exactly
// this leg's own coin count" or similar).
//
// Overflow: LCM of 4 arbitrary 64-bit-ish thicknesses could in theory overflow
// even long long if thicknesses are adversarially chosen large coprime values;
// guarded with an overflow check that clamps to a large sentinel (never wins
// a min/max comparison against any real candidate, since board discussion
// indicates true answers stay under ~2e9).

#include <cstdio>
#include <vector>
using namespace std;

typedef long long ll;
const ll INF = (ll)4e18;

ll gcd(ll a, ll b) {
    while (b) { ll t = a % b; a = b; b = t; }
    return a;
}

// safe lcm, clamps to INF on overflow
ll safe_lcm(ll a, ll b) {
    if (a == INF || b == INF) return INF;
    ll g = gcd(a, b);
    ll q = a / g;
    // check q * b overflow
    if (b != 0 && q > INF / b) return INF;
    return q * b;
}

int main() {
    int n, t;
    while (scanf("%d %d", &n, &t) == 2) {
        if (n == 0 && t == 0) break;
        vector<ll> coin(n);
        for (int i = 0; i < n; i++) scanf("%lld", &coin[i]);

        vector<ll> lcms;
        lcms.reserve(230300);
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                ll l2 = safe_lcm(coin[i], coin[j]);
                if (l2 >= INF) continue; // still could matter but treat as irrelevant huge
                for (int k = j + 1; k < n; k++) {
                    ll l3 = safe_lcm(l2, coin[k]);
                    for (int m = k + 1; m < n; m++) {
                        ll l4 = safe_lcm(l3, coin[m]);
                        lcms.push_back(l4);
                    }
                }
            }
        }

        for (int q = 0; q < t; q++) {
            ll h;
            scanf("%lld", &h);
            ll best_low = 0; // any combo whose true LCM overflows our cap is
                              // certainly > h in realistic data, so its floor
                              // multiple is 0; 0 is a safe, correct fallback
                              // (never wrongly beats a real achievable combo).
            ll best_high = INF;
            for (size_t idx = 0; idx < lcms.size(); idx++) {
                ll L = lcms[idx];
                if (L <= 0 || L >= INF) continue;
                ll low = (h / L) * L;
                ll high = low;
                if (low != h) high = low + L;
                if (low > best_low) best_low = low;
                if (high < best_high) best_high = high;
            }
            printf("%lld %lld\n", best_low, best_high);
        }
    }
    return 0;
}
