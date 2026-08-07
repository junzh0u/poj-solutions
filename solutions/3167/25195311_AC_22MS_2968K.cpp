// POJ 3167 - Cow Patterns
// Model: claude-opus-5
//
// Order-isomorphic (order-preserving) pattern matching by KMP.
//
// A window T[i..i+K-1] matches the pattern P iff the two are order-isomorphic:
// for every pair of positions the spot counts compare the same way as the
// pattern ranks do.  Equivalently, and incrementally, an order-isomorphic match
// of length j extends by one iff the new element's rank inside the window
// agrees with P's: writing lt[j] = #{r < j : P[r] < P[j]} and
// eq[j] = #{r < j : P[r] == P[j]}, the window must contain exactly lt[j]
// elements below T[i] and eq[j] equal to it.  That pair (lt, eq) pins down the
// new element's relation to every earlier one, because the counts of earlier
// elements below/equal form a strictly increasing sequence of prefix sums over
// the distinct values, so only one split is consistent with a given lt.
//
// Order-isomorphism is an equivalence closed under taking prefixes, so the KMP
// failure function is well defined over it and the usual shift argument holds.
// The window's contents live in a Fenwick tree, so each test is two prefix-sum
// queries; every text element is inserted once and deleted at most once as the
// automaton falls back, giving O((N + K) log) overall.
//
// Note that any two single elements are order-isomorphic, so every pattern
// prefix of length >= 2 has a border of length >= 1 -- fail[j] is never 0 for
// j >= 2.  That falls out of the code (the k == 0 test always succeeds); it is
// not a special case.
//
// Robustness notes, both from the problem's discuss board:
//   * one poster's Accepted run needed pattern arrays larger than the stated
//     K <= 25,000, so nothing here is sized from the stated bounds -- the
//     vectors are sized from the N and K actually read, and values are
//     order-preservingly compressed instead of assuming the range 1..S;
//   * cin/cout was reported at 3.5s against a 2s limit, hence the fread
//     tokenizer and the buffered output.
// Input is consumed to EOF in case a file holds more than one data set.

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;

/* ---------- fast input ---------- */
static vector<char> g_in;
static size_t g_pos = 0;

static void readAll() {
    char buf[1 << 16];
    size_t got;
    while ((got = fread(buf, 1, sizeof(buf), stdin)) > 0)
        g_in.insert(g_in.end(), buf, buf + got);
    g_in.push_back('\0');
}

static bool readInt(int &out) {
    size_t n = g_in.size();
    while (g_pos < n && (g_in[g_pos] < '0' || g_in[g_pos] > '9') && g_in[g_pos] != '-')
        ++g_pos;
    if (g_pos >= n || g_in[g_pos] == '\0') return false;
    int sign = 1;
    if (g_in[g_pos] == '-') { sign = -1; ++g_pos; }
    int v = 0;
    bool any = false;
    while (g_pos < n && g_in[g_pos] >= '0' && g_in[g_pos] <= '9') {
        v = v * 10 + (g_in[g_pos] - '0');
        ++g_pos;
        any = true;
    }
    if (!any) return false;
    out = v * sign;
    return true;
}

/* ---------- output buffer ---------- */
static vector<char> g_out;
static void putInt(int v) {
    char tmp[12];
    int len = 0;
    if (v == 0) tmp[len++] = '0';
    while (v > 0) { tmp[len++] = (char)('0' + v % 10); v /= 10; }
    while (len > 0) g_out.push_back(tmp[--len]);
    g_out.push_back('\n');
}

/* ---------- Fenwick over compressed values 1..M ---------- */
static int M;
static vector<int> bitT, bitP;

static void bitAdd(vector<int> &b, int i, int d) {
    for (; i <= M; i += i & (-i)) b[i] += d;
}
static int bitSum(const vector<int> &b, int i) {
    int s = 0;
    if (i > M) i = M;
    for (; i > 0; i -= i & (-i)) s += b[i];
    return s;
}

int main() {
    readAll();
    int n, k, s;
    while (readInt(n) && readInt(k) && readInt(s)) {
        (void)s;
        if (n < 0) n = 0;
        if (k < 0) k = 0;
        vector<int> t(n), p(k);
        for (int i = 0; i < n; ++i) readInt(t[i]);
        for (int i = 0; i < k; ++i) readInt(p[i]);

        /* order-preserving coordinate compression of the union */
        vector<int> all;
        all.reserve(n + k);
        for (int i = 0; i < n; ++i) all.push_back(t[i]);
        for (int i = 0; i < k; ++i) all.push_back(p[i]);
        sort(all.begin(), all.end());
        all.erase(unique(all.begin(), all.end()), all.end());
        M = (int)all.size();
        if (M < 1) M = 1;
        for (int i = 0; i < n; ++i)
            t[i] = (int)(lower_bound(all.begin(), all.end(), t[i]) - all.begin()) + 1;
        for (int i = 0; i < k; ++i)
            p[i] = (int)(lower_bound(all.begin(), all.end(), p[i]) - all.begin()) + 1;

        bitT.assign(M + 1, 0);
        bitP.assign(M + 1, 0);

        /* prefix signature of the pattern: for each j, how many of p[0..j-1]
           are strictly less than p[j], and how many equal it */
        vector<int> plt(k, 0), peq(k, 0);
        for (int j = 0; j < k; ++j) {
            int lo = bitSum(bitP, p[j] - 1);
            int eq = bitSum(bitP, p[j]) - lo;
            plt[j] = lo;
            peq[j] = eq;
            bitAdd(bitP, p[j], 1);
        }
        bitP.assign(M + 1, 0);

        /* failure function under order-isomorphism */
        vector<int> fail(k + 1, 0);
        {
            int kk = 0;
            for (int j = 1; j < k; ++j) {
                while (kk > 0) {
                    int lo = bitSum(bitP, p[j] - 1);
                    int eq = bitSum(bitP, p[j]) - lo;
                    if (lo == plt[kk] && eq == peq[kk]) break;
                    int nk = fail[kk];
                    for (int r = j - kk; r < j - nk; ++r) bitAdd(bitP, p[r], -1);
                    kk = nk;
                }
                bitAdd(bitP, p[j], 1);
                ++kk;
                fail[j + 1] = kk;
            }
        }

        /* KMP scan of the text */
        vector<int> res;
        if (k > 0 && k <= n) {
            int j = 0;
            for (int i = 0; i < n; ++i) {
                while (j > 0) {
                    int lo = bitSum(bitT, t[i] - 1);
                    int eq = bitSum(bitT, t[i]) - lo;
                    if (lo == plt[j] && eq == peq[j]) break;
                    int nj = fail[j];
                    for (int r = i - j; r < i - nj; ++r) bitAdd(bitT, t[r], -1);
                    j = nj;
                }
                bitAdd(bitT, t[i], 1);
                ++j;
                if (j == k) {
                    res.push_back(i - k + 2);
                    int nj = fail[k];
                    for (int r = i - k + 1; r <= i - nj; ++r) bitAdd(bitT, t[r], -1);
                    j = nj;
                }
            }
        }

        putInt((int)res.size());
        for (size_t i = 0; i < res.size(); ++i) putInt(res[i]);
    }
    if (!g_out.empty()) fwrite(&g_out[0], 1, g_out.size(), stdout);
    return 0;
}
