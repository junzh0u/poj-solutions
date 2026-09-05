// POJ 1570 - Exchange Rates
// Model: claude-sonnet-5
// Weighted union-find over item "worth" ratios, using exact reduced fractions
// (long long numerator/denominator, gcd-reduced at every combine) so query
// answers come out exactly in lowest terms without floating point.
// For each non-root node x, ratioNum[x]/ratioDen[x] = w(x)/w(root(x)).
// An assertion "m A = n B" means m*w(A) = n*w(B), i.e. w(A)/w(B) = n/m.
// Union by size keeps trees shallow so the fractions combined during path
// compression stay small (items <= 60, exchange values < 100).
#include <cstdio>
#include <map>
#include <string>
using namespace std;

static int parent_[70], sz[70];
static long long rNum[70], rDen[70];
static int numItems = 0;
static map<string,int> idOf;

static long long gcdll(long long a, long long b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b) { long long t = a % b; a = b; b = t; }
    return a;
}

// Multiply two reduced fractions n1/d1 * n2/d2, cross-cancelling first (so
// the factors themselves stay small) and doing the final multiply in
// __int128 as extra headroom before reducing back down to long long. Chained
// exchange-rate ratios can transiently grow well past the <=10000 bound that
// only the final printed answer is guaranteed to respect.
static void mulReduce(long long n1, long long d1, long long n2, long long d2,
                       long long &outN, long long &outD) {
    long long g = gcdll(n1, d2);
    if (g != 0) { n1 /= g; d2 /= g; }
    g = gcdll(n2, d1);
    if (g != 0) { n2 /= g; d1 /= g; }
    __int128 N = (__int128)n1 * n2;
    __int128 D = (__int128)d1 * d2;
    __int128 a = N < 0 ? -N : N;
    __int128 b = D < 0 ? -D : D;
    while (b) { __int128 t = a % b; a = b; b = t; }
    if (a != 0) { N /= a; D /= a; }
    outN = (long long)N;
    outD = (long long)D;
}

static int getId(const string &s) {
    map<string,int>::iterator it = idOf.find(s);
    if (it != idOf.end()) return it->second;
    int id = numItems++;
    idOf[s] = id;
    parent_[id] = id;
    sz[id] = 1;
    rNum[id] = rDen[id] = 1;
    return id;
}

// Path-compressing find; after the call rNum[x]/rDen[x] == w(x)/w(root).
static int find(int x) {
    if (parent_[x] == x) return x;
    int path[70];
    int cnt = 0;
    int cur = x;
    while (parent_[cur] != cur) {
        path[cnt++] = cur;
        cur = parent_[cur];
    }
    int root = cur;
    for (int i = cnt - 1; i >= 0; i--) {
        int node = path[i];
        int p = parent_[node];
        if (p != root) {
            long long n, d;
            mulReduce(rNum[node], rDen[node], rNum[p], rDen[p], n, d);
            rNum[node] = n; rDen[node] = d;
        }
        parent_[node] = root;
    }
    return root;
}

// Assert w(a)/w(b) = num/den.
static void unite(int a, int b, long long num, long long den) {
    int ra = find(a), rb = find(b);
    if (ra == rb) return; // guaranteed consistent; nothing new to add
    // w(ra)/w(rb) = (num/den) * (w(b)/w(rb)) / (w(a)/w(ra))
    long long tN, tD, RNum, RDen;
    mulReduce(num, den, rNum[b], rDen[b], tN, tD);
    mulReduce(tN, tD, rDen[a], rNum[a], RNum, RDen);
    if (sz[ra] >= sz[rb]) {
        parent_[rb] = ra;
        rNum[rb] = RDen; rDen[rb] = RNum; // w(rb)/w(ra) is the inverse
        sz[ra] += sz[rb];
    } else {
        parent_[ra] = rb;
        rNum[ra] = RNum; rDen[ra] = RDen;
        sz[rb] += sz[ra];
    }
}

int main() {
    char cmd;
    while (scanf(" %c", &cmd) == 1) {
        if (cmd == '.') break;
        if (cmd == '!') {
            long long m, n;
            char bufA[25], bufB[25], eq[4];
            scanf("%lld %s %s %lld %s", &m, bufA, eq, &n, bufB);
            int a = getId(bufA), b = getId(bufB);
            unite(a, b, n, m); // w(A)/w(B) = n/m
        } else if (cmd == '?') {
            char bufA[25], eq[4], bufB[25];
            scanf("%s %s %s", bufA, eq, bufB);
            int a = getId(bufA), b = getId(bufB);
            int ra = find(a), rb = find(b);
            if (ra != rb) {
                printf("? %s = ? %s\n", bufA, bufB);
            } else {
                long long x, y;
                mulReduce(rNum[b], rDen[b], rDen[a], rNum[a], x, y);
                printf("%lld %s = %lld %s\n", x, bufA, y, bufB);
            }
        }
    }
    return 0;
}
