// POJ 3160 - Father Christmas flymouse
// Model: claude-opus-5
//
// A journey is a walk in a directed graph; on arriving at a room flymouse may
// enter it (collecting its comfort index) or bypass it in silence, and a room
// is never entered twice.  So the value of a walk is the sum of the POSITIVE
// comfort indices among the distinct rooms it touches.
//
// Tarjan SCC + longest path on the condensation.  Inside one SCC every room is
// reachable from every other and he can always come back, so an SCC he enters
// contributes exactly the sum of its positive comfort indices; the condensation
// is a DAG and Tarjan numbers its components in reverse topological order, so a
// single increasing sweep gives dp[c] = pos_sum[c] + max(0, max dp[successor]).
// All weights are >= 0, so the answer is >= 0 and stopping early is free, which
// makes the "until he could reach no more unvisited rooms" clause immaterial.
//
// Ambiguity: whether he may skip a negative room *inside* an SCC.  The statement
// grants the bypass unconditionally, and the official sample cannot separate the
// readings (its two rooms are both positive) - the separating case is an SCC of
// two rooms worth 5 and -3, which is 5 here and 2 under "take the whole SCC".
// Input is 0-indexed (the sample uses rooms 0 and 1 for N = 2) and holds several
// data sets, read to end of file.

#include <cstdio>
#include <cstring>

static const int MAXN = 30005;
static const int MAXM = 150005;

static int head[MAXN], eto[MAXM], enx[MAXM];
static int chead[MAXN], cto[MAXM], cnx[MAXM];
static int w[MAXN];
static int eu[MAXM], ev[MAXM];

static int dfn[MAXN], low[MAXN], comp[MAXN], stk[MAXN];
static char instk[MAXN];
static int frU[MAXN], frE[MAXN];
static int cw[MAXN], dp[MAXN];

static int n, m, ecnt, ccnt, ncomp;

/* ---- fast input ---- */
static char buf[1 << 16];
static int bl = 0, bp = 0;
static int gc() {
    if (bp == bl) {
        bl = (int)fread(buf, 1, sizeof(buf), stdin);
        bp = 0;
        if (bl <= 0) return -1;
    }
    return (unsigned char)buf[bp++];
}
static bool readInt(int &out) {
    int c = gc();
    while (c != -1 && (c < '0' || c > '9') && c != '-') c = gc();
    if (c == -1) return false;
    int sgn = 1;
    if (c == '-') { sgn = -1; c = gc(); }
    int x = 0;
    while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = gc(); }
    out = x * sgn;
    return true;
}

static void tarjan() {
    int idx = 0, top = 0;
    ncomp = 0;
    for (int i = 0; i < n; i++) { dfn[i] = 0; low[i] = 0; comp[i] = -1; instk[i] = 0; }
    for (int s = 0; s < n; s++) {
        if (dfn[s]) continue;
        int sp = 0;
        dfn[s] = low[s] = ++idx;
        stk[top++] = s; instk[s] = 1;
        frU[0] = s; frE[0] = head[s];
        while (sp >= 0) {
            int u = frU[sp];
            int e = frE[sp];
            if (e != -1) {
                frE[sp] = enx[e];
                int v = eto[e];
                if (!dfn[v]) {
                    dfn[v] = low[v] = ++idx;
                    stk[top++] = v; instk[v] = 1;
                    ++sp; frU[sp] = v; frE[sp] = head[v];
                } else if (instk[v]) {
                    if (dfn[v] < low[u]) low[u] = dfn[v];
                }
            } else {
                if (low[u] == dfn[u]) {
                    int v;
                    do {
                        v = stk[--top];
                        instk[v] = 0;
                        comp[v] = ncomp;
                    } while (v != u);
                    ncomp++;
                }
                --sp;
                if (sp >= 0) {
                    int p = frU[sp];
                    if (low[u] < low[p]) low[p] = low[u];
                }
            }
        }
    }
}

int main() {
    while (readInt(n)) {
        if (!readInt(m)) break;
        if (n < 0) n = 0;
        if (m < 0) m = 0;
        for (int i = 0; i < n; i++) { head[i] = -1; w[i] = 0; }
        for (int i = 0; i < n; i++) readInt(w[i]);

        int mx = -1, mn = 0x7fffffff;
        for (int i = 0; i < m; i++) {
            readInt(eu[i]); readInt(ev[i]);
            if (eu[i] > mx) mx = eu[i];
            if (ev[i] > mx) mx = ev[i];
            if (eu[i] < mn) mn = eu[i];
            if (ev[i] < mn) mn = ev[i];
        }
        /* Rooms are 0-indexed (the sample uses 0 and 1 for N = 2). Guard against
           a 1-indexed data set anyway: it costs nothing and cannot misfire on
           valid 0-indexed input, where no index may equal N. */
        if (m > 0 && mx >= n && mn >= 1) {
            for (int i = 0; i < m; i++) { eu[i]--; ev[i]--; }
        }
        ecnt = 0;
        for (int i = 0; i < m; i++) {
            int a = eu[i], b = ev[i];
            if (a < 0 || a >= n || b < 0 || b >= n) continue;
            eto[ecnt] = b; enx[ecnt] = head[a]; head[a] = ecnt; ecnt++;
        }

        tarjan();

        for (int c = 0; c < ncomp; c++) { cw[c] = 0; chead[c] = -1; }
        /* Inside one SCC every room can be reached and left again, so he enters
           exactly the rooms with a positive comfort index and bypasses the rest. */
        for (int i = 0; i < n; i++)
            if (w[i] > 0) cw[comp[i]] += w[i];

        ccnt = 0;
        for (int u = 0; u < n; u++) {
            for (int e = head[u]; e != -1; e = enx[e]) {
                int v = eto[e];
                if (comp[u] == comp[v]) continue;
                cto[ccnt] = comp[v]; cnx[ccnt] = chead[comp[u]]; chead[comp[u]] = ccnt; ccnt++;
            }
        }

        /* Tarjan numbers components in reverse topological order: every successor
           of component c has an id strictly smaller than c. */
        int ans = 0;
        for (int c = 0; c < ncomp; c++) {
            int best = 0;
            for (int e = chead[c]; e != -1; e = cnx[e]) {
                int d = cto[e];
                if (dp[d] > best) best = dp[d];
            }
            dp[c] = cw[c] + best;
            if (dp[c] > ans) ans = dp[c];
        }
        printf("%d\n", ans);
    }
    return 0;
}
