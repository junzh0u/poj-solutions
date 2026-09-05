// POJ 1943 - Hall of Fountains
// Model: claude-sonnet-5
//
// Positions 0..n: position 0 is "in front of room 1" (unconstrained, no
// fountain), position i (1<=i<=n) means standing inside room i.  Each second
// you may move to position-1, position, or position+1 (clamped at the ends);
// a move into position j (1<=j<=n) is legal only if room j's fountain is off
// during the second you arrive.  We track, for every integer time t, the set
// of positions you could safely occupy at that instant (bitset over 0..n),
// updating it second by second: reachable(t) = (reachable(t-1) shifted by
// -1,0,+1) masked by which rooms are off at time t.  Position 0 is always in
// the mask.  The answer is (first t with position n reachable) + 1, since
// from inside the last room the exit itself is unconstrained.
//
// Each fountain i has period 2*p_i (on for p_i seconds starting at offset
// q_i, then off for p_i seconds), or is permanently off when p_i=0.  All
// periods 2,4,...,20 divide L=5040=2^4*3^2*5*7, so the whole system's
// evolution is driven by t mod 5040.  Since reachable(t) is monotone in its
// input (a bigger reachable set can only produce a bigger next reachable
// set) and position 0 is reachable at every time, reachable(k*5040) is a
// non-decreasing sequence of subsets of {0..n} as k grows, hence it reaches
// a fixed point within n+2 periods; if position n hasn't appeared by then it
// never will.  We precompute the off-mask for every residue mod 5040 once
// per test case, then simulate with O(1) work per second using a 128-bit
// bitset, checking for a fixed point every 5040 steps to stop early on
// impossible cases and capping at (n+2)*5040 steps as a rigorous safety net.
//
// Statement ambiguity: none found; the discuss board's main pitfall (per
// its titles) is forgetting that you may step back to position 0 at any
// time, which the model above already allows for free.

#include <cstdio>
#include <bitset>
#include <cstring>

using namespace std;

static const int L = 5040; // LCM(2,4,6,8,10,12,14,16,18,20)

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        static int p[105], q[105];
        for (int i = 1; i <= n; ++i) scanf("%d", &p[i]);
        for (int i = 1; i <= n; ++i) scanf("%d", &q[i]);

        static bitset<128> maskTable[L];
        for (int tau = 0; tau < L; ++tau) {
            bitset<128> m;
            m.set(0);
            for (int i = 1; i <= n; ++i) {
                if (p[i] == 0) {
                    m.set(i);
                } else {
                    int period = 2 * p[i];
                    int r = ((tau - q[i]) % period + period) % period;
                    if (r >= p[i]) m.set(i);
                }
            }
            maskTable[tau] = m;
        }

        bitset<128> reachable;
        reachable.set(0);
        bitset<128> checkpoint = reachable;
        int bound = (n + 2) * L;
        int answer = 0;
        for (int t = 1; t <= bound; ++t) {
            int tau = t % L;
            bitset<128> nxt = (reachable | (reachable << 1) | (reachable >> 1)) & maskTable[tau];
            reachable = nxt;
            if (reachable.test(n)) {
                answer = t + 1;
                break;
            }
            if (t % L == 0) {
                if (reachable == checkpoint) break;
                checkpoint = reachable;
            }
        }
        printf("%d\n", answer);
    }
    return 0;
}
