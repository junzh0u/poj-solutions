// POJ 1634 - Who's the boss?
// Model: claude-opus-5
//
// Sort employees by salary ascending. Your immediate boss is the least-paid
// employee who out-earns you and is at least as tall as you, which in that
// ordering is exactly the nearest index to the right whose height is >= yours
// -- the classic "next greater or equal element" scan, done with a monotonic
// stack in O(m). Parent indices therefore always exceed child indices, so
// subtree sizes fall out of one left-to-right pass (sz[par[i]] += sz[i]) with
// no recursion and no depth limit; the answer for a query is sz[k] - 1.
// Employees with no such successor print boss 0.
//
// Ambiguity: "subordinates" is transitive, not the direct-report count -- the
// statement spells this out ("all his subordinates are your subordinates as
// well") and the discuss board records people losing submissions to the
// direct-only reading. Height ties go to the boss: "at least as tall", so the
// stack pops on strictly-smaller heights only. The official sample does not
// distinguish that tie-break, so it was settled from the statement's wording
// and pinned with a constructed separating case.

#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

const int MAXM = 30005;
const int IDSPAN = 1000000;

struct Emp {
    int id, sal, h;
};

static Emp e[MAXM];
static int par[MAXM];
static int sz[MAXM];
static int stk[MAXM];
static int idIndex[IDSPAN];

static bool bySalary(const Emp &a, const Emp &b) { return a.sal < b.sal; }

static char buf[1 << 16];
static int bufLen = 0, bufPos = 0;

static int gc() {
    if (bufPos == bufLen) {
        bufLen = (int)fread(buf, 1, sizeof(buf), stdin);
        bufPos = 0;
        if (bufLen <= 0) return -1;
    }
    return (unsigned char)buf[bufPos++];
}

static int readInt() {
    int c = gc();
    while (c != -1 && (c < '0' || c > '9') && c != '-') c = gc();
    int sign = 1;
    if (c == '-') { sign = -1; c = gc(); }
    int v = 0;
    while (c >= '0' && c <= '9') { v = v * 10 + (c - '0'); c = gc(); }
    return v * sign;
}

int main() {
    memset(idIndex, -1, sizeof(idIndex));
    int cases = readInt();
    while (cases-- > 0) {
        int m = readInt();
        int q = readInt();
        for (int i = 0; i < m; ++i) {
            e[i].id = readInt();
            e[i].sal = readInt();
            e[i].h = readInt();
        }
        sort(e, e + m, bySalary);
        for (int i = 0; i < m; ++i) idIndex[e[i].id] = i;

        int top = 0;
        for (int i = m - 1; i >= 0; --i) {
            while (top > 0 && e[stk[top - 1]].h < e[i].h) --top;
            par[i] = (top > 0) ? stk[top - 1] : -1;
            stk[top++] = i;
        }

        for (int i = 0; i < m; ++i) sz[i] = 1;
        for (int i = 0; i < m; ++i)
            if (par[i] >= 0) sz[par[i]] += sz[i];

        for (int Q = 0; Q < q; ++Q) {
            int id = readInt();
            int k = idIndex[id];
            int p = par[k];
            printf("%d %d\n", p >= 0 ? e[p].id : 0, sz[k] - 1);
        }

        for (int i = 0; i < m; ++i) idIndex[e[i].id] = -1;
    }
    return 0;
}
