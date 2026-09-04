// POJ 3708 - Recurrent Function
// Model: claude-sonnet-5
//
// f is defined digit-wise in base d: writing m in base d as a digit
// string D_1 D_2 ... D_L (D_1 the nonzero leading digit), f(m) is the
// number whose leading digit is a[D_1] and whose other digits are
// b[D_i]. Since a permutes {1..d-1} and b permutes {0..d-1}, f is a
// bijection that preserves the base-d digit length. So f_x(m)=k is
// possible only if m and k have equal digit length in base d; then,
// independently for the leading digit (via permutation a) and every
// other digit (via permutation b), x must equal the (unique, taken
// modulo the containing permutation cycle's length) number of steps
// that carries that digit of m to the corresponding digit of k. This
// is a system of linear congruences x = r_i (mod p_i) solved with the
// extended-Euclid CRT merge (moduli need not be pairwise coprime).
// Since a,b act on at most 100 elements, any cycle-length LCM arising
// from a single such permutation is bounded by Landau's g(100) ~=2.3e8,
// so the combined modulus safely fits in 64 bits (needs long long ->
// language G++, since POJ's plain C++ has no 64-bit integer type).
//
// Ambiguity settled from the discuss board: the second input line's
// 2d-1 integers are read as a whitespace/newline-insensitive token
// stream (sample splits them across two printed lines); m,k are read
// as plain decimal strings with no leading zero.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

typedef long long ll;
typedef unsigned long long ull;

ll extgcd(ll a, ll b, ll &x, ll &y) {
    if (b == 0) { x = 1; y = 0; return a; }
    ll x1, y1;
    ll g = extgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

// merge x = r1 (mod m1) with x = r2 (mod m2) into x = r1 (mod m1)
bool mergeCRT(ll &r1, ll &m1, ll r2, ll m2) {
    ll x, y;
    ll g = extgcd(m1, m2, x, y); // m1*x + m2*y = g
    ll diff = r2 - r1;
    if (diff % g != 0) return false;
    ll lcm = m1 / g * m2;
    ll m2g = m2 / g;
    // t = (diff/g) * x  (mod m2g)
    ll d0 = (diff / g) % m2g;
    if (d0 < 0) d0 += m2g;
    ll xm = x % m2g;
    if (xm < 0) xm += m2g;
    __int128 t = (__int128)d0 * xm % m2g;
    __int128 newr = (__int128)r1 + (__int128)m1 * t;
    newr %= (__int128)lcm;
    if (newr < 0) newr += lcm;
    r1 = (ll)newr;
    m1 = lcm;
    return true;
}

// convert a decimal digit string to base-d digits, most significant first
vector<int> toBaseD(string dec, int d) {
    vector<int> digitsRev; // base-d digits, least significant first
    string cur = dec;
    while (!(cur.size() == 1 && cur[0] == '0')) {
        // divide cur by d, get remainder
        string q;
        int rem = 0;
        for (size_t i = 0; i < cur.size(); i++) {
            int val = rem * 10 + (cur[i] - '0');
            int qd = val / d;
            rem = val % d;
            if (!(q.empty() && qd == 0)) q.push_back((char)('0' + qd));
        }
        if (q.empty()) q = "0";
        digitsRev.push_back(rem);
        cur = q;
    }
    if (digitsRev.empty()) digitsRev.push_back(0);
    reverse(digitsRev.begin(), digitsRev.end());
    return digitsRev;
}

// build cycle position/length/id arrays for perm defined on [lo,hi]
void buildCycles(vector<int>& perm, int lo, int hi, vector<int>& pos, vector<int>& clen, vector<int>& cid) {
    vector<int> visited(hi + 1, 0);
    int cidCounter = 0;
    for (int v = lo; v <= hi; v++) {
        if (visited[v]) continue;
        vector<int> cyc;
        int cur = v;
        while (!visited[cur]) {
            visited[cur] = 1;
            cyc.push_back(cur);
            cur = perm[cur];
        }
        int L = (int)cyc.size();
        for (int i = 0; i < L; i++) {
            pos[cyc[i]] = i;
            clen[cyc[i]] = L;
            cid[cyc[i]] = cidCounter;
        }
        cidCounter++;
    }
}

int main() {
    int d;
    while (scanf("%d", &d) == 1) {
        if (d == -1) break;
        vector<int> a(d, 0); // a[1..d-1]
        vector<int> b(d, 0); // b[0..d-1]
        for (int i = 1; i <= d - 1; i++) scanf("%d", &a[i]);
        for (int i = 0; i <= d - 1; i++) scanf("%d", &b[i]);
        char mbuf[210], kbuf[210];
        scanf("%s", mbuf);
        scanf("%s", kbuf);
        string mstr(mbuf), kstr(kbuf);

        vector<int> mdig = toBaseD(mstr, d);
        vector<int> kdig = toBaseD(kstr, d);

        if (mdig.size() != kdig.size()) {
            printf("NO\n");
            continue;
        }
        int L = (int)mdig.size();

        vector<int> posA(d, 0), clenA(d, 0), cidA(d, 0);
        vector<int> posB(d, 0), clenB(d, 0), cidB(d, 0);
        buildCycles(a, 1, d - 1, posA, clenA, cidA);
        buildCycles(b, 0, d - 1, posB, clenB, cidB);

        bool ok = true;
        ll r = 0, m = 1;

        for (int idx = 0; idx < L && ok; idx++) {
            int mv = mdig[idx], kv = kdig[idx];
            int ri, pi;
            if (idx == 0) {
                if (cidA[mv] != cidA[kv]) { ok = false; break; }
                pi = clenA[mv];
                ri = (posA[kv] - posA[mv]) % pi;
                if (ri < 0) ri += pi;
            } else {
                if (cidB[mv] != cidB[kv]) { ok = false; break; }
                pi = clenB[mv];
                ri = (posB[kv] - posB[mv]) % pi;
                if (ri < 0) ri += pi;
            }
            if (!mergeCRT(r, m, (ll)ri, (ll)pi)) { ok = false; break; }
        }

        if (!ok) {
            printf("NO\n");
        } else {
            printf("%llu\n", (unsigned long long)r);
        }
    }
    return 0;
}
