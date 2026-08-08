// POJ 3175 - Finding Bovine Roots
// Model: claude-opus-5
//
// Smallest n >= 1 whose sqrt has the given L digits right after the point.
// With M = 10^L and D the digit string's value, that is
//     floor(M*sqrt(n)) mod M == D,
// i.e. writing A = k*M + D (k = floor(sqrt n)), we need the smallest k for
// which [A^2, (A+1)^2) contains a multiple of M^2; then n = ceil(A^2/M^2).
// All of it is exact integer work -- doubles cannot separate the candidates
// once n passes 2^53, and the answer reaches 2.5e17.
//
// A^2 = (k^2+f)*M^2 + (c*M + D^2) with c = (k*e) mod M, e = (2D) mod M,
// f = 2kD/M.  Feeding that into "(-A^2) mod M^2 <= 2A" and splitting on
// whether c*M + D^2 crosses M^2, both branches collapse to one condition:
//     z_k <= 2k + delta,   z_k = (beta - k*e) mod M,
//     beta = M - floor(D^2/M) - 1,  delta = floor((D^2+2D)/M) - floor(D^2/M) - 1.
// D = 0 is the one case outside this (A^2 is then already a multiple of M^2,
// so n = 1); it is handled separately.
//
// Scanning k one at a time is O(k) and k reaches M/2 = 5e8 (e.g. L=9,
// "999999999" -> 250000001000000000), far too slow.  Instead walk k in strides
// of Q, a continued-fraction denominator of e/M chosen to minimise the work:
// within one residue class k = r + i*Q the walk is z -= (Q*e) mod M, which
// drifts by only rho = min(d, M-d) per stride, and between two wraps z is
// monotone while the threshold 2k+delta is monotone the other way -- so the
// first satisfying index of a whole run is one division.  Cost is
// Q + (M/2)*rho/M with rho < M/Q, i.e. O(sqrt(M)) ~ 3e4 steps worst case.
// The stride is a pure speed knob: every k is still accounted for, so any Q
// gives the same answer (verified against Q = 1, 5, 97, 300, 1000, 30000).
//
// Ambiguity: an all-zero digit string. n = 0 also has bovine root 0.000...,
// but "smallest integer" is read as the cows' own domain n >= 1, which is what
// every accepted program on the discuss board enumerates; so "000000000" -> 1.
//
// The discuss board's claim that L=6 "000001" -> 62500000001 is correct (a
// reply calling it miscomputed is wrong), and L=9 "000000001" is
// 62500000000000001, not the 281474976710656 = 2^48 posted there.

#include <iostream>
#include <string>
using namespace std;
typedef long long ll;

static ll QMAX;

// work needed if the walk is taken in strides of q
static ll costOf(ll q, ll e, ll m) {
    ll d = (q % m) * (e % m) % m;
    ll rho = d < m - d ? d : m - d;
    return q + rho / 2;
}

// a continued-fraction denominator of e/m, at most QMAX, of least cost
static ll chooseQ(ll e, ll m) {
    if (e == 0) return 1;
    ll bestQ = 1, bestCost = costOf(1, e, m);
    ll km1 = 0, k0 = 1;
    ll r0 = m, r1 = e;
    while (r1 != 0) {
        ll a = r0 / r1;
        ll k1 = a * k0 + km1;
        if (k1 > QMAX) {
            ll ap = (QMAX - km1) / k0;             // best semiconvergent that fits
            if (ap > 0) {
                ll cand = ap * k0 + km1;
                ll cc = costOf(cand, e, m);
                if (cc < bestCost) { bestCost = cc; bestQ = cand; }
            }
            break;
        }
        ll cc = costOf(k1, e, m);
        if (cc < bestCost) { bestCost = cc; bestQ = k1; }
        km1 = k0; k0 = k1;
        ll r2 = r0 % r1; r0 = r1; r1 = r2;
    }
    return bestQ < 1 ? 1 : bestQ;
}

// smallest k >= 1 with (beta - k*e) mod m <= 2*k + delta
static ll solveK(ll m, ll e, ll beta, ll delta) {
    ll Q = chooseQ(e, m);
    ll d = (Q % m) * (e % m) % m;      // drift of one stride
    ll u = m - d;                      // same walk seen as an increase
    ll A = 2 * Q;                      // threshold gain per stride
    ll best = -1;
    for (ll r = 0; r < Q; r++) {
        ll br = ((beta - (r % m) * (e % m)) % m + m) % m;
        ll i = (r == 0) ? 1 : 0;       // k = r + i*Q must be >= 1
        ll B = 2 * r + delta;
        ll z = ((br - (i % m) * d) % m + m) % m;
        for (;;) {
            if (best >= 0 && r + i * Q > best) break;
            ll need = z - A * i - B;
            if (d == 0) {                            // z never moves
                if (need > 0) i += (need + A - 1) / A;
                if (best < 0 || r + i * Q < best) best = r + i * Q;
                break;
            }
            if (d <= u) {                            // run of decreases by d
                ll t = z / d;
                ll jj = (need <= 0) ? 0 : (need + (A + d) - 1) / (A + d);
                if (jj <= t) {
                    ll k = r + (i + jj) * Q;
                    if (best < 0 || k < best) best = k;
                    break;
                }
                i += t + 1;
                z = z - (t + 1) * d + m;
            } else {                                 // run of increases by u
                ll t = (m - 1 - z) / u;
                if (need <= 0) {
                    ll k = r + i * Q;
                    if (best < 0 || k < best) best = k;
                    break;
                }
                if (A > u) {
                    ll jj = (need + (A - u) - 1) / (A - u);
                    if (jj <= t) {
                        ll k = r + (i + jj) * Q;
                        if (best < 0 || k < best) best = k;
                        break;
                    }
                }
                i += t + 1;
                z = z + (t + 1) * u - m;
            }
        }
    }
    return best;
}

int main() {
    int L;
    string s;
    while (cin >> L >> s) {                          // several data sets, read to EOF
        ll M = 1;
        for (int i = 0; i < L; i++) M *= 10;
        ll D = 0;
        for (size_t i = 0; i < s.size(); i++) D = D * 10 + (s[i] - '0');
        while (D >= M) M *= 10;                      // guard: more digits than L says
        if (D == 0) { cout << 1 << "\n"; continue; }
        QMAX = 1;
        while (2 * QMAX * QMAX < M) QMAX *= 2;
        ll e = (2 * D) % M;
        ll P = (D * D) / M;
        ll P2 = (D * D + 2 * D) / M;
        ll k = solveK(M, e, M - P - 1, P2 - P - 1);
        ll c = (k % M) * (e % M) % M;
        ll f = (2 * k * D) / M;
        ll G = c * M + D * D;
        cout << k * k + f + 1 + (G > M * M ? 1 : 0) << "\n";
    }
    return 0;
}
