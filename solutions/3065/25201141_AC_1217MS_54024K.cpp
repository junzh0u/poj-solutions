// POJ 3065 - Stargates
// Model: claude-sonnet-5
//
// Approach: weighted/union-by-rank union-find with path compression over a
// fixed universe of planet ids 0..6000000. Each 'D n' line starts a brand
// new data set of n planets and must forget every earlier connection; rather
// than memset-ing the whole array (which would be too slow if many large
// data sets appear), each node carries a "version" stamp and is treated as
// a fresh singleton the first time it is touched after the version counter
// is bumped by a 'D' line. This makes both "start a new data set" and
// "look up a planet that has never been touched" O(1).
//
// Statement ambiguities settled from the discuss board (poj.org/bbs,
// problem_id=3065):
//  - The statement says planets are numbered 1..N, but the judge data
//    actually contains planet id 0 as well (board thread "注意 有编号为0的",
//    with a follow-up "我就错这了" confirming it cost a submission). So the
//    valid id range for a data set of size N is treated as [0, N] inclusive,
//    not [1, N].
//  - The step values (dststep, srcstep) can be 0 (board: "step 可能为0")
//    and can be negative, and the generated id src + k*srcstep /
//    dst + k*dststep can fall outside [0, N] (board:
//    "步长可能会是负数吧？？计算中也有可能超出N吧（那算不相连吧）"). Any
//    generated id outside [0, N] is treated as referring to a nonexistent
//    planet: a 'C' pair touching such an id is simply not linked, and a 'Q'
//    pair touching such an id is counted as disconnected (it cannot be
//    connected to anything since it doesn't exist).
//  - A 'D' line can appear in the middle of the input (the sample itself
//    does this) and always discards the previous data set completely, even
//    if no query used it yet.
//
// All arithmetic for the generated ids is done in 64-bit to avoid overflow
// from k * step before the range check clamps it back to a 32-bit index.

#include <cstdio>
#include <cctype>

typedef long long ll;

const int MAXN = 6000001; // valid planet ids for a data set of size N are 0..N, N <= 6000000

static int parent_[MAXN];
static unsigned char rnk_[MAXN];
static unsigned int ver_[MAXN];
static unsigned int curVer = 1; // never 0, so the zero-initialized ver_[] never matches on the very first use
static ll curN = -1;            // -1 means "no data set defined yet"

inline int find(int x) {
    if (ver_[x] != curVer) {
        ver_[x] = curVer;
        parent_[x] = x;
        rnk_[x] = 0;
        return x;
    }
    int root = x;
    while (parent_[root] != root) root = parent_[root];
    while (parent_[x] != root) {
        int nxt = parent_[x];
        parent_[x] = root;
        x = nxt;
    }
    return root;
}

inline void unite(int a, int b) {
    int ra = find(a), rb = find(b);
    if (ra == rb) return;
    if (rnk_[ra] < rnk_[rb]) { int t = ra; ra = rb; rb = t; }
    parent_[rb] = ra;
    if (rnk_[ra] == rnk_[rb]) rnk_[ra]++;
}

// ---- fast input ----
static char inbuf[1 << 16];
static int inbuf_len = 0, inbuf_pos = 0;

inline int getChar() {
    if (inbuf_pos == inbuf_len) {
        inbuf_len = (int)fread(inbuf, 1, sizeof(inbuf), stdin);
        inbuf_pos = 0;
        if (inbuf_len <= 0) return -1;
    }
    return (unsigned char)inbuf[inbuf_pos++];
}

// Reads the next non-blank logical line: a command letter followed by up to
// maxVals integers on the same line. Returns the number of integers parsed,
// or -1 if there is no more input.
int readCommand(char &cmd, ll *vals, int maxVals) {
    int c = getChar();
    while (c == '\n' || c == '\r' || c == ' ' || c == '\t') c = getChar();
    if (c == -1) return -1;
    cmd = (char)toupper(c);
    int cnt = 0;
    c = getChar();
    while (c != -1 && c != '\n') {
        if (c == '\r' || c == ' ' || c == '\t') { c = getChar(); continue; }
        bool neg = false;
        if (c == '-') { neg = true; c = getChar(); }
        else if (c == '+') { c = getChar(); }
        ll val = 0;
        bool any = false;
        while (c != -1 && c >= '0' && c <= '9') {
            val = val * 10 + (c - '0');
            any = true;
            c = getChar();
        }
        if (any) {
            if (cnt < maxVals) vals[cnt] = neg ? -val : val;
            cnt++;
        } else if (c != -1 && c != '\n') {
            c = getChar(); // skip an unexpected character defensively
        }
    }
    return cnt;
}

int main() {
    char cmd;
    ll vals[5];
    char outbuf[1 << 20];
    setvbuf(stdout, outbuf, _IOFBF, sizeof(outbuf));

    for (;;) {
        int cnt = readCommand(cmd, vals, 5);
        if (cnt < 0) break;
        if (cmd == 'D') {
            if (cnt >= 1) curN = vals[0];
            curVer++;
        } else if (cmd == 'C' || cmd == 'Q') {
            if (cnt < 2) continue;
            ll src = vals[0], dst = vals[1];
            ll nnn = (cnt >= 3) ? vals[2] : 1;
            ll dststep = (cnt >= 4) ? vals[3] : 1;
            ll srcstep = (cnt >= 5) ? vals[4] : 0;

            ll connectedCnt = 0, disconnectedCnt = 0;
            for (ll k = 0; k < nnn; k++) {
                ll s = src + k * srcstep;
                ll d = dst + k * dststep;
                bool sValid = (s >= 0 && s <= curN);
                bool dValid = (d >= 0 && d <= curN);
                if (cmd == 'C') {
                    if (sValid && dValid) unite((int)s, (int)d);
                } else {
                    if (sValid && dValid && find((int)s) == find((int)d)) connectedCnt++;
                    else disconnectedCnt++;
                }
            }
            if (cmd == 'Q') printf("%lld - %lld\n", connectedCnt, disconnectedCnt);
        }
    }
    return 0;
}
