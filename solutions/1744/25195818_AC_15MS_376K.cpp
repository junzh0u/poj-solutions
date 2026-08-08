// POJ 1744 - Elevator Stopping Plan
// Model: claude-opus-5
//
// The elevator makes one upward trip from floor 1: reaching floor s costs
// 4*(s-1) seconds plus 10 for every stop made strictly before it, and a rider
// then walks 20 seconds per floor to their office.  Nobody is forced onto the
// elevator, so a request at floor f is also served by the stairs in 20*(f-1)
// seconds -- that is a virtual stop at floor 1 with arrival time 0, and it
// costs no 10-second penalty to anyone else.  Stops need not be at requested
// floors (confirmed on the discuss board, msg 5819); the separating case
// "4 2 3 5 6" answers 32 with a stop at the unrequested floor 4 and 40 without.
//
// Binary search the answer L.  For a fixed set of stops, the stop minimizing a
// rider's time is monotone in their floor (reaching one stop further up costs
// 4/floor + 10/stop <= 14/floor, always cheaper than walking 20/floor), so the
// requests split into consecutive groups, each served by one stop, with the
// stairs group at the bottom.  So L is achievable iff this greedy succeeds:
// walk everybody with 20*(f-1) <= L, then repeatedly take the lowest unserved
// floor a, place the stop as high as the budget allows,
//     j = max{ j : 4*(j-1) + 20*(j-a) <= L - 10*num },
// which also maximizes how far up that stop reaches,
//     b = max{ b : 4*(j-1) + 20*(b-j) <= L - 10*num },
// and serve every request <= b.  Extending each group maximally is optimal
// because later groups carry a strictly larger stop penalty, and shrinking a
// group never raises the cost of the one above it.
//
// O(n log(max answer)) per test case.

#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

static const int MAXN = 30005;
static int f[MAXN];
static int n;

// Can every requested floor be reached within L seconds?
static bool feasible(int L) {
    int i = 0;
    while (i < n && 20 * (f[i] - 1) <= L) i++;   // the stairs group
    int num = 0;                                 // elevator stops made so far
    while (i < n) {
        int a = f[i];
        int budget = L - 10 * num;
        if (budget < 4 * (a - 1)) return false;  // lowest unserved floor is out of reach
        int j = (budget + 20 * a + 4) / 24;      // highest stop still serving a
        int b = (budget + 16 * j + 4) / 20;      // highest floor that stop reaches
        while (i < n && f[i] <= b) i++;
        num++;
    }
    return true;
}

static char buf[1 << 16];
static int bufLen = 0, bufPos = 0;

static int nextChar() {
    if (bufPos == bufLen) {
        bufLen = (int)fread(buf, 1, sizeof(buf), stdin);
        bufPos = 0;
        if (bufLen <= 0) return -1;
    }
    return (unsigned char)buf[bufPos++];
}

static bool readInt(int &out) {
    int c = nextChar();
    while (c != -1 && (c < '0' || c > '9') && c != '-') c = nextChar();
    if (c == -1) return false;
    int sign = 1;
    if (c == '-') { sign = -1; c = nextChar(); }
    int v = 0;
    while (c >= '0' && c <= '9') { v = v * 10 + (c - '0'); c = nextChar(); }
    out = sign * v;
    return true;
}

int main() {
    while (readInt(n) && n > 0) {
        for (int i = 0; i < n; i++) {
            if (!readInt(f[i])) return 0;
        }
        sort(f, f + n);
        n = (int)(unique(f, f + n) - f);
        int lo = 0;
        int hi = 4 * (f[n - 1] - 1) + 10 * (n - 1);   // stop at every request
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (feasible(mid)) hi = mid; else lo = mid + 1;
        }
        printf("%d\n", lo);
    }
    return 0;
}
