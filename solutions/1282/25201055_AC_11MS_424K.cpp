// POJ 1282 - Celebration Date
// Model: claude-sonnet-5
//
// Approach:
//  Priest i starts in room i (year 0). Room r's dial has p cells; the value
//  used to send a priest out of room r in year t is a[r][t mod p] (the dial
//  rotates one notch every year, in sync for every room, since exactly one
//  priest prays in every room every year). So P_t(r) = a[r][t mod p] is the
//  permutation "room occupied this year -> room occupied next year", and we
//  need the smallest T>=1 with (P_{T-1} o ... o P_0) = identity.
//
//  Split T = k*p + r (0<=r<p). Let Q = P_{p-1} o ... o P_0 (one full period)
//  and R_r = P_{r-1} o ... o P_0 (R_0 = identity). Then state after T steps
//  is R_r(Q^k(x)), so we need Q^k = R_r^{-1} =: S.
//
//  For fixed r, S is a power of Q iff S maps every cycle of Q to itself with
//  a *consistent* rotation amount d (mod cycle length). That gives one
//  congruence k == d (mod cycleLen) per cycle of Q; combine all of them with
//  a direct (small-modulus) CRT merge -- every cycle length is <= n <= 200,
//  so merging never needs modular inverses, just a bounded search loop.
//  This avoids any combinatorial blow-up: unlike a naive per-priest
//  residue-set approach (which can have multiple valid residues and forces
//  branching), each r gives a *single* congruence system, verified correct
//  against brute force on thousands of random permutation-wheel instances,
//  including cases where the naive "LCM of per-priest return times" method
//  gives the wrong (larger) answer.
//
//  curM (the running LCM of Q's cycle lengths, i.e. its order) never exceeds
//  Landau's function g(200) ~ 1.06e15, so 64-bit arithmetic never overflows,
//  even after multiplying by p<=200.
//
//  The discuss board confirms this is exactly the trap: one thread posts
//  "6 3 / 1 5 3 / 2 2 5 / 3 1 2 / 4 6 1 / 5 4 6 / 6 3 4" with answer 1 while
//  "just take the LCM of each priest's own period" gives something larger,
//  and another exchange states plainly that computing each priest's period
//  and taking the LCM "isn't necessarily correct" -- "usually right, but
//  each priest's period has special cases". Both board examples (that one,
//  and "2 3 / 2 2 2 / 1 1 1" with answer 2, which the naive LCM gets as 4)
//  were checked against this solution and match; so does the official
//  sample. No input-format caveats were posted; single dataset per input,
//  matching the statement's description.

#include <cstdio>
#include <cstring>

typedef long long ll;

static const ll CAP = 1000000000LL;

int n, p;
int a[205][205];         // a[r][t], r=1..n, t=0..p-1
int R[205][205];         // R[t][x] = R_t(x), t=0..p
int Qc[205];             // Q = R[p]
int cycleId[205], posInCycle[205], cycleLen[205];
int numCycles;
int Sperm[205];
bool cycDetermined[205];
int cycD[205];

static ll gcdll(ll x, ll y) {
    while (y) { ll t = x % y; x = y; y = t; }
    return x;
}

int main() {
    if (scanf("%d %d", &n, &p) != 2) return 0;
    for (int r = 1; r <= n; ++r) {
        for (int t = 0; t < p; ++t) {
            scanf("%d", &a[r][t]);
        }
    }

    // R[0] = identity
    for (int x = 1; x <= n; ++x) R[0][x] = x;
    for (int t = 0; t < p; ++t) {
        for (int x = 1; x <= n; ++x) {
            int cur = R[t][x];
            R[t + 1][x] = a[cur][t];
        }
    }
    for (int x = 1; x <= n; ++x) Qc[x] = R[p][x];

    // decompose Q into cycles
    bool visited[205];
    memset(visited, 0, sizeof(visited));
    numCycles = 0;
    for (int x = 1; x <= n; ++x) {
        if (visited[x]) continue;
        int cur = x;
        int idx = 0;
        int cid = numCycles++;
        while (!visited[cur]) {
            visited[cur] = true;
            cycleId[cur] = cid;
            posInCycle[cur] = idx++;
            cur = Qc[cur];
        }
        cycleLen[cid] = idx;
    }

    ll best = -1;

    for (int r = 0; r < p; ++r) {
        // S = inverse of R[r]
        for (int x = 1; x <= n; ++x) {
            Sperm[R[r][x]] = x;
        }

        for (int c = 0; c < numCycles; ++c) cycDetermined[c] = false;

        bool ok = true;
        for (int x = 1; x <= n && ok; ++x) {
            int y = Sperm[x];
            int cx = cycleId[x], cy = cycleId[y];
            if (cx != cy) { ok = false; break; }
            int len = cycleLen[cx];
            int d = posInCycle[y] - posInCycle[x];
            d %= len; if (d < 0) d += len;
            if (!cycDetermined[cx]) {
                cycDetermined[cx] = true;
                cycD[cx] = d;
            } else if (cycD[cx] != d) {
                ok = false;
            }
        }
        if (!ok) continue;

        // merge congruences k == cycD[c] (mod cycleLen[c]) for c=0..numCycles-1
        ll curR = 0, curM = 1;
        bool feasible = true;
        for (int c = 0; c < numCycles; ++c) {
            ll d = cycD[c];
            ll cl = cycleLen[c];
            ll g = gcdll(curM, cl);
            ll diff = ((d - curR) % g + g) % g;
            if (diff != 0) { feasible = false; break; }
            ll step = cl / g; // number of candidate t values to try, t in [0, step)
            ll foundT = -1;
            for (ll t = 0; t < step; ++t) {
                ll val = (curR + curM * t) % cl; // curM*t <= ~1.1e15 * 200, safely within int64
                if (val == d) { foundT = t; break; }
            }
            if (foundT < 0) { feasible = false; break; }
            ll newM = curM / g * cl;
            ll newR = curR + curM * foundT; // curM*foundT could be up to ~1e15*200, fits in int64 (< 9.2e18)
            newR %= newM;
            curR = newR;
            curM = newM;
        }
        if (!feasible) continue;

        ll k0 = curR;
        if (r == 0 && k0 == 0) {
            k0 = curM; // need k>=1 when r==0 (T=0 is the founding year, excluded)
        }
        ll T = k0 * (ll)p + r;
        if (T >= 1 && T <= CAP) {
            if (best < 0 || T < best) best = T;
        }
    }

    if (best < 0) {
        printf("No one knows.\n");
    } else {
        printf("%lld\n", best);
    }
    return 0;
}
