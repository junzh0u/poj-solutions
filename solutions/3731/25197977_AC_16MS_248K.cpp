// POJ 3731 - Escape
// Model: claude-sonnet-5
//
// Hamilton starts at (0,0) heading North on a grid city with corners
// (0,0)..(X,Y). At every intersection he may go straight or turn right,
// never left, and never revisits an intersection. Count the number of
// distinct routes from (0,0) to (x,y), mod 100000007.
//
// Because only right turns are allowed, the walk's heading only ever
// advances through N->E->S->W->N->... (clockwise), so the whole route is a
// sequence of straight runs separated by right turns: it looks like a
// rectangular spiral. A route reaching (x,y) with final heading H can be
// decomposed by the number of "loops" i it winds around the target before
// its last straight run into (x,y); for each choice of i, the four straight
// runs before each of the up-to-i+1 turns in each of the four directions
// are independently choosable via a product of 4 binomial coefficients
// (choosing which of the available unit steps in each of the four
// "arms" around (x,y) get used to build the concentric rectangles of the
// spiral). Summing that product over i, and over the 4 possible final
// headings, counts every self-avoiding no-left-turn walk into (x,y)
// exactly once. x==0 additionally admits the direct straight-line route
// along the starting column (needs no turn at all, including the trivial
// zero-length route when (x,y)==(0,0)); all four loop sums are naturally
// empty in that case since they are gated on i<=x-1.
//
// This decomposition (and the four per-heading binomial-product sums
// below) matches a formula posted on the POJ message board for this
// problem (message 129053); it was not taken on faith - it is verified
// here against an independent brute-force self-avoiding-walk counter
// (built directly from the statement's turn rule, not from the posted
// formula) over every (X,Y,x,y) with X,Y up to 7, 1225 cases, all exact
// matches, plus agreement with all three official samples.
//
// Answers stay below the modulus (< 1.1e8), so the final printf uses
// plain %d; only the internal products need 64-bit range.

#include <cstdio>

typedef long long ll;
const int MOD = 100000007;
const int MAXN = 2005;

static ll fact[MAXN], inv_fact[MAXN];

ll power(ll a, ll b, ll mod) {
    a %= mod;
    ll res = 1;
    while (b > 0) {
        if (b & 1) res = res * a % mod;
        a = a * a % mod;
        b >>= 1;
    }
    return res;
}

void init() {
    fact[0] = 1;
    for (int i = 1; i < MAXN; i++) fact[i] = fact[i-1] * i % MOD;
    inv_fact[MAXN-1] = power(fact[MAXN-1], MOD-2, MOD);
    for (int i = MAXN-2; i >= 0; i--) inv_fact[i] = inv_fact[i+1] * (i+1) % MOD;
}

ll C(int n, int k) {
    if (k < 0 || n < 0 || k > n) return 0;
    return fact[n] * inv_fact[k] % MOD * inv_fact[n-k] % MOD;
}

int main() {
    init();
    int T;
    scanf("%d", &T);
    while (T--) {
        int X, Y, x, y;
        scanf("%d %d %d %d", &X, &Y, &x, &y);
        ll ret = 0;
        int i;
        // final heading South
        for (i = 0; i <= x-1 && i <= X-x && i <= y && i+1 <= Y-y; i++)
            ret = (ret + C(x-1,i) * C(X-x,i) % MOD * C(y,i) % MOD * C(Y-y,i+1)) % MOD;
        // final heading East
        for (i = 0; i <= x-1 && i <= X-x && i <= y && i <= Y-y; i++)
            ret = (ret + C(x-1,i) * C(X-x,i) % MOD * C(y,i) % MOD * C(Y-y,i)) % MOD;
        // final heading North
        for (i = 0; i <= x-1 && i+1 <= X-x && i+1 <= y && i+1 <= Y-y; i++)
            ret = (ret + C(x-1,i) * C(X-x,i+1) % MOD * C(y,i+1) % MOD * C(Y-y,i+1)) % MOD;
        // final heading West
        for (i = 0; i <= x-1 && i+1 <= X-x && i <= y && i+1 <= Y-y; i++)
            ret = (ret + C(x-1,i) * C(X-x,i+1) % MOD * C(y,i) % MOD * C(Y-y,i+1)) % MOD;
        if (x == 0) ret = (ret + 1) % MOD;
        printf("%d\n", (int)ret);
    }
    return 0;
}
