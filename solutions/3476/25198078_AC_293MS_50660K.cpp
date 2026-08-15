// POJ 3476 - A Game with Colored Balls
// Model: claude-sonnet-5
//
// Simulate: repeatedly find the leftmost run of maximal consecutive same-
// colored balls; if its length is 1, the whole game stops (no output for
// it); otherwise remove it, report its original ball numbers in order, and
// if its former left/right neighbor runs are now adjacent and share a
// color, merge them into one run.
//
// A run's "position" key for the leftmost tie-break is the smallest
// original ball number in it: because the chain always keeps balls in
// their original left-to-right order, a run earlier in the chain has all
// of its original numbers smaller than every number in a later run, even
// after repeated merges. So merged runs are also always range-sorted, and
// their ball-number lists can be represented as a linked concatenation of
// the original contiguous [lo,hi] blocks (no copying needed on merge -
// just splice one singly linked interval list onto another), giving O(1)
// merges and O(total balls printed) = O(N) total output work.
//
// Priority (max length, tie leftmost/min original position) is maintained
// with a binary max-heap over a plain (len, minpos, id) triple ordered by
// a custom comparator, plus lazy deletion via an alive[] flag - every run
// id is pushed exactly once when created, so a popped entry is valid iff
// its run is still alive, and it is then guaranteed to be the true
// current max. (No 64-bit arithmetic anywhere - not needed here, so it is
// simplest to just not depend on it.)
//
// Total run objects created is bounded by (initial runs) + (merges) <=
// 2*N, and interval nodes are only created for the initial runs (<= N),
// since later merges just relink existing nodes; array sizes below reflect
// those bounds.
//
// Board notes checked: only one dataset (frkstyc's own reply, "one"); N=1
// (or any chain that ends with only singleton runs left) legitimately
// produces no output for the leftover balls, confirmed by the sample
// itself (ball 10 is never printed). No statement ambiguity found beyond
// the above, and it was pinned down by hand-tracing the sample step by
// step before coding (matches exactly).

#include <cstdio>
#include <cstdlib>
#include <queue>
#include <vector>
using namespace std;

static const int MAXN = 1000005;
static char ball[MAXN];
static int N = 0;

struct HKey {
    int len, minpos, id;
};
struct HKeyCmp {
    bool operator()(const HKey &a, const HKey &b) const {
        if (a.len != b.len) return a.len < b.len;      // bigger len first
        return a.minpos > b.minpos;                    // tie: smaller minpos first
    }
};
static HKey makeKey(int len_, int minpos_, int id_) {
    HKey k; k.len = len_; k.minpos = minpos_; k.id = id_; return k;
}

int main() {
    // Read the whole input, keep only R/G/B characters (robust to any
    // line wrapping / stray whitespace / CR).
    static char buf[1 << 16];
    size_t got;
    while ((got = fread(buf, 1, sizeof(buf), stdin)) > 0) {
        for (size_t i = 0; i < got; i++) {
            char c = buf[i];
            if (c == 'R' || c == 'G' || c == 'B') {
                if (N < MAXN - 1) ball[++N] = c; // 1-indexed
            }
        }
    }

    if (N == 0) return 0;

    int MAXR = 2 * N + 10;
    int MAXI = N + 10;

    char *rColor = (char*)malloc(MAXR);
    int *rLen = (int*)malloc(sizeof(int) * MAXR);
    int *rMin = (int*)malloc(sizeof(int) * MAXR);
    char *rAlive = (char*)malloc(MAXR);
    int *rLeft = (int*)malloc(sizeof(int) * MAXR);
    int *rRight = (int*)malloc(sizeof(int) * MAXR);
    int *rHead = (int*)malloc(sizeof(int) * MAXR);
    int *rTail = (int*)malloc(sizeof(int) * MAXR);

    int *iLo = (int*)malloc(sizeof(int) * MAXI);
    int *iHi = (int*)malloc(sizeof(int) * MAXI);
    int *iNext = (int*)malloc(sizeof(int) * MAXI);

    if (!rColor || !rLen || !rMin || !rAlive || !rLeft || !rRight || !rHead ||
        !rTail || !iLo || !iHi || !iNext) {
        fprintf(stderr, "alloc failed\n");
        return 1;
    }

    vector<HKey> heapStore;
    heapStore.reserve(MAXR);
    priority_queue<HKey, vector<HKey>, HKeyCmp> pq(HKeyCmp(), heapStore);

    int runCnt = 0;   // next run id
    int intCnt = 0;   // next interval node id

    #define MAKE_KEY(len_, minpos_, id_) makeKey(len_, minpos_, id_)

    // Build initial runs.
    int i = 1;
    int prevRun = -1;
    while (i <= N) {
        int j = i;
        while (j + 1 <= N && ball[j + 1] == ball[i]) j++;
        int id = runCnt++;
        int node = intCnt++;
        iLo[node] = i; iHi[node] = j; iNext[node] = -1;
        rColor[id] = ball[i];
        rLen[id] = j - i + 1;
        rMin[id] = i;
        rAlive[id] = 1;
        rHead[id] = node; rTail[id] = node;
        rLeft[id] = prevRun;
        rRight[id] = -1;
        if (prevRun != -1) rRight[prevRun] = id;
        prevRun = id;
        pq.push(MAKE_KEY(rLen[id], rMin[id], id));
        i = j + 1;
    }

    // Output buffer.
    size_t outCap = 20 * 1024 * 1024;
    char *out = (char*)malloc(outCap);
    size_t outLen = 0;
    #define ENSURE(extra) do { \
        if (outLen + (extra) > outCap) { \
            outCap *= 2; \
            out = (char*)realloc(out, outCap); \
        } \
    } while (0)

    char numbuf[12];

    while (!pq.empty()) {
        HKey key = pq.top();
        pq.pop();
        int id = key.id;
        if (!rAlive[id]) continue;
        if (rLen[id] == 1) break; // largest run has length 1 -> game over

        // Output this run: color then all ball numbers ascending.
        ENSURE(4);
        out[outLen++] = rColor[id];
        for (int node = rHead[id]; node != -1; node = iNext[node]) {
            for (int v = iLo[node]; v <= iHi[node]; v++) {
                ENSURE(9);
                out[outLen++] = ' ';
                int len = 0;
                int vv = v;
                do { numbuf[len++] = (char)('0' + vv % 10); vv /= 10; } while (vv);
                while (len) out[outLen++] = numbuf[--len];
            }
        }
        ENSURE(1);
        out[outLen++] = '\n';

        int L = rLeft[id], R = rRight[id];
        rAlive[id] = 0;
        if (L != -1) rRight[L] = R;
        if (R != -1) rLeft[R] = L;

        if (L != -1 && R != -1 && rColor[L] == rColor[R]) {
            int nid = runCnt++;
            rColor[nid] = rColor[L];
            rLen[nid] = rLen[L] + rLen[R];
            rMin[nid] = rMin[L];
            rHead[nid] = rHead[L];
            rTail[nid] = rTail[R];
            iNext[rTail[L]] = rHead[R];
            int LL = rLeft[L], RR = rRight[R];
            rLeft[nid] = LL; rRight[nid] = RR;
            if (LL != -1) rRight[LL] = nid;
            if (RR != -1) rLeft[RR] = nid;
            rAlive[L] = 0; rAlive[R] = 0; rAlive[nid] = 1;
            pq.push(MAKE_KEY(rLen[nid], rMin[nid], nid));
        }
    }

    fwrite(out, 1, outLen, stdout);
    return 0;
}
