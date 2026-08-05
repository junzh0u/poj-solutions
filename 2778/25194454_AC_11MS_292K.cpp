// POJ 2778 - DNA Sequence
//
// Count length-n strings over {A,C,G,T} (n <= 2*10^9) that contain none of the
// m <= 10 forbidden segments (each <= 10 chars), modulo 100000.
//
// Approach: build an Aho-Corasick automaton over the forbidden segments (at most
// 10*10+1 = 101 states). A state is "dead" if it ends a forbidden segment, and
// deadness must be propagated along suffix links during the BFS that builds
// them: a state whose fail state is dead also contains that segment as a
// suffix. Missing this propagation is the classic wrong answer here.
//
// The surviving states form a directed graph whose adjacency matrix M has
// M[i][j] = number of letters c with goto(i,c) = j. Walks of length n from the
// root that never touch a dead state are exactly the safe strings, so the
// answer is the sum of row 0 of M^n, computed by binary exponentiation:
// ~31 squarings of an at-most-95x95 matrix, trivially inside the 1s limit.
//
// Notes on the statement:
//  - "mod 100000" is 10^5, which is *not* prime; nothing here needs it to be,
//    since only additions and multiplications are involved (no division).
//  - m = 0 is legal and yields 4^n mod 100000 (the automaton is the root alone).
//  - n <= 2000000000 < 2^31-1, so a plain int suffices; that also sidesteps
//    POJ's unreliable %lld.
//  - Duplicate segments and segments that are prefixes of one another are fine
//    and were covered by a randomized differential test against a brute force
//    enumerating all 4^n strings for n <= 9.

#include <cstdio>
#include <cstring>

static const int MOD = 100000;
static const int MAXN = 105;

int go[MAXN][4];
int fail_[MAXN];
bool bad[MAXN];
int sz;

int codeOf(char c) {
    switch (c) {
        case 'A': return 0;
        case 'C': return 1;
        case 'G': return 2;
        default:  return 3; /* T */
    }
}

void insert(const char *s) {
    int cur = 0;
    for (const char *p = s; *p; ++p) {
        int c = codeOf(*p);
        if (go[cur][c] == 0) {
            go[cur][c] = sz;
            ++sz;
        }
        cur = go[cur][c];
    }
    bad[cur] = true;
}

int queue_[MAXN];

void build() {
    int head = 0, tail = 0;
    for (int c = 0; c < 4; ++c) {
        int v = go[0][c];
        if (v != 0) {
            fail_[v] = 0;
            queue_[tail++] = v;
        }
    }
    while (head < tail) {
        int u = queue_[head++];
        /* a node whose fail node contains a forbidden segment is bad too */
        if (bad[fail_[u]]) bad[u] = true;
        for (int c = 0; c < 4; ++c) {
            int v = go[u][c];
            if (v == 0) {
                go[u][c] = go[fail_[u]][c];
            } else {
                fail_[v] = go[fail_[u]][c];
                queue_[tail++] = v;
            }
        }
    }
}

int dim;

struct Mat {
    int a[MAXN][MAXN];
};

Mat tmpM;

void mul(Mat &r, const Mat &x, const Mat &y) {
    static long long buf[MAXN];
    for (int i = 0; i < dim; ++i) {
        for (int j = 0; j < dim; ++j) buf[j] = 0;
        for (int k = 0; k < dim; ++k) {
            int v = x.a[i][k];
            if (v == 0) continue;
            const int *yr = y.a[k];
            for (int j = 0; j < dim; ++j) buf[j] += (long long)v * yr[j];
        }
        for (int j = 0; j < dim; ++j) r.a[i][j] = (int)(buf[j] % MOD);
    }
}

char buf[32];

int main() {
    int m, n;
    /* n <= 2000000000 < 2^31-1, so a plain int is enough and %lld is avoided
       (POJ's ancient runtime is unreliable with %lld). */
    while (scanf("%d %d", &m, &n) == 2) {
        memset(go, 0, sizeof(go));
        memset(fail_, 0, sizeof(fail_));
        memset(bad, 0, sizeof(bad));
        sz = 1;
        for (int i = 0; i < m; ++i) {
            if (scanf("%s", buf) != 1) buf[0] = '\0';
            insert(buf);
        }
        build();

        /* compress alive states */
        int idx[MAXN];
        int cnt = 0;
        for (int i = 0; i < sz; ++i) idx[i] = bad[i] ? -1 : cnt++;
        dim = cnt;

        Mat M, R;
        memset(&M, 0, sizeof(M));
        memset(&R, 0, sizeof(R));
        for (int i = 0; i < sz; ++i) {
            if (bad[i]) continue;
            for (int c = 0; c < 4; ++c) {
                int v = go[i][c];
                if (bad[v]) continue;
                M.a[idx[i]][idx[v]]++;
            }
        }
        for (int i = 0; i < dim; ++i) R.a[i][i] = 1;

        int e = n;
        while (e > 0) {
            if (e & 1) { mul(tmpM, R, M); R = tmpM; }
            e >>= 1;
            if (e > 0) { mul(tmpM, M, M); M = tmpM; }
        }

        int ans = 0;
        int start = idx[0]; /* root is never bad unless a pattern is empty, which cannot happen */
        if (start < 0) {
            ans = 0;
        } else {
            for (int j = 0; j < dim; ++j) ans += R.a[start][j];
            ans %= MOD;
        }
        printf("%d\n", ans);
    }
    return 0;
}
