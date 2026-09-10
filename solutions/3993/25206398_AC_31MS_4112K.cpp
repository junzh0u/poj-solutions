// POJ 3993 - Not So Flat After All
// Model: claude-sonnet-5
// Approach: For A and B (0 < A,B < 1,000,000), factorize both into
// prime -> exponent maps. The minimum-ordinal space that plots both
// numbers is one axis per distinct prime appearing in either
// factorization (N = |union of primes|). The Manhattan distance between
// the two points is the sum over that union of |exponent_A - exponent_B|
// (treating an absent prime as exponent 0). A smallest-prime-factor
// sieve up to 999999 makes each factorization O(log v) instead of
// O(sqrt(v)) trial division, per the discuss board's hint to optimize
// the factorization since the number of test cases is unbounded/unknown.
//
// Ambiguity: the input spec says A*B > 1, which rules out A=B=1, but the
// discuss board (message 340890, title translates to "seems there's a
// perverse case where A and B are both 1") warns that "1 1" appears as
// real (non-terminator) input despite that. The natural formulas already
// handle it without special-casing: both factorizations are empty, so
// N=0 and D=0 fall out directly with no division/array issues.
#include <cstdio>
#include <map>
#include <set>
using namespace std;

const int MAXV = 1000000;
int spf[MAXV]; // smallest prime factor, spf[1] = 1 (no prime factors)

void sieve() {
    for (int i = 0; i < MAXV; ++i) spf[i] = 0;
    for (int i = 2; i < MAXV; ++i) {
        if (spf[i] == 0) {
            for (int j = i; j < MAXV; j += i) {
                if (spf[j] == 0) spf[j] = i;
            }
        }
    }
}

void factorize(int v, map<int, int>& m) {
    while (v > 1) {
        int p = spf[v];
        int cnt = 0;
        while (v % p == 0) { v /= p; ++cnt; }
        m[p] = cnt;
    }
}

int main() {
    sieve();
    int a, b;
    int caseNo = 1;
    while (scanf("%d %d", &a, &b) == 2) {
        if (a == 0 && b == 0) break;
        map<int, int> fa, fb;
        factorize(a, fa);
        factorize(b, fb);
        set<int> keys;
        for (map<int, int>::iterator it = fa.begin(); it != fa.end(); ++it) keys.insert(it->first);
        for (map<int, int>::iterator it = fb.begin(); it != fb.end(); ++it) keys.insert(it->first);
        int N = (int)keys.size();
        int D = 0;
        for (set<int>::iterator it = keys.begin(); it != keys.end(); ++it) {
            int ea = fa.count(*it) ? fa[*it] : 0;
            int eb = fb.count(*it) ? fb[*it] : 0;
            D += (ea > eb) ? (ea - eb) : (eb - ea);
        }
        printf("%d. %d:%d\n", caseNo++, N, D);
    }
    return 0;
}
