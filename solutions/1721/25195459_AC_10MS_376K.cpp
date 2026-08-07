// POJ 1721 - CARDS
// Model: claude-sonnet-5
//
// Alice builds x (a single N-cycle on {1..N}, N odd) from an arbitrary
// permutation a: x maps position a_i to card a_{i+1} (cyclically). Since a
// ranges over all permutations, x ranges over all N-cycles.
// A "double shuffle" replaces permutation q by q∘q (q squared as a
// function: new position i holds card q(q(i))). After S double shuffles
// starting from x we get p = x^(2^S) (as function composition powers).
// Since x is a single N-cycle it has order exactly N, so exponents are
// taken mod N: p = x^(2^S mod N). Because N is odd, 2 is invertible mod N,
// so let t = 2^S mod N and inv = t^{-1} mod N (extended gcd); then
// x = p^inv. Powers of the same permutation commute, so binary
// exponentiation over function composition needs no order care.
// Verified against the sample (N=7,S=4) by hand and by a differential test
// that builds a random N-cycle x, forward-doubles it S times to produce p,
// and checks the recovered x' reproduces x exactly (the map x -> x^t is a
// bijection on N-cycles since gcd(t,N)=1, so recovery is unique).
#include <cstdio>

static int modpow(int base, int e, int mod) {
    base %= mod;
    int r = 1 % mod;
    while (e > 0) {
        if (e & 1) r = r * base % mod;
        base = base * base % mod;
        e >>= 1;
    }
    return r;
}

static int extgcd(int a, int b, int &x, int &y) {
    if (b == 0) { x = 1; y = 0; return a; }
    int x1, y1;
    int g = extgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

int main() {
    int N, S;
    if (scanf("%d %d", &N, &S) != 2) return 0;
    static int p[1001];
    for (int i = 1; i <= N; i++) scanf("%d", &p[i]);

    if (N == 1) {
        printf("1\n");
        return 0;
    }

    int t = modpow(2, S, N);
    int inv, dummy;
    extgcd(t, (int)N, inv, dummy);
    inv = ((inv % N) + N) % N;

    static int cur[1001], base_[1001], tmp[1001];
    for (int i = 1; i <= N; i++) cur[i] = i;
    for (int i = 1; i <= N; i++) base_[i] = p[i];

    int e = inv;
    while (e > 0) {
        if (e & 1) {
            for (int i = 1; i <= N; i++) tmp[i] = base_[cur[i]];
            for (int i = 1; i <= N; i++) cur[i] = tmp[i];
        }
        for (int i = 1; i <= N; i++) tmp[i] = base_[base_[i]];
        for (int i = 1; i <= N; i++) base_[i] = tmp[i];
        e >>= 1;
    }

    for (int i = 1; i <= N; i++) printf("%d\n", cur[i]);
    return 0;
}
