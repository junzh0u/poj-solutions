// POJ 3926 - Parade
// Model: claude-sonnet-5
//
// Grid of (n+1) west-east roads (rows, given north to south) and m+1
// north-south roads (columns). The parade starts anywhere on the
// southernmost row and ends anywhere on the northernmost row, moving only
// northward between rows (vertical moves are free/instant and can happen
// at any column). Within one row the path segment between the column it
// enters at and the column it leaves at must be monotonic (a straight
// line road cannot be revisited), and the total zone *length* used while
// inside that row must not exceed k.
//
// DP by row, south to north. dp_in[j] = best welcome sum upon entering the
// current row at column j (== dp_out of the row just below, columns line
// up because vertical roads exist at every column). dp_out[j] = best sum
// after optionally sliding along the current row to end at column j:
//   east[j]  = max over c<=j with L[j]-L[c]<=k of dp_in[c] + (W[j]-W[c])
//   west[j]  = max over c>=j with L[c]-L[j]<=k of dp_in[c] + (W[c]-W[j])
//   dp_out[j] = max(east[j], west[j])          (c=j is included in both,
//                                                giving dp_out[j]>=dp_in[j])
// west[] is obtained by mirroring the row (columns and zones reversed) and
// reusing the east computation, which is a monotonic-deque sliding-window
// maximum over prefix sums (O(m) per row per direction).
//
// The discuss board (message 163547/163549/157558) repeatedly reports
// plain C++ (POJ language 4, old MSVC) timing out on this problem even
// after heavy constant-factor optimization, while G++ passes easily
// (rank1 was taken with G++); this is a real compiler-speed issue for
// this problem's huge aggregate input, not a long-long/scanf myth, so
// this is submitted under G++.
//
// Answer is guaranteed to fit a 32-bit int; all DP/prefix-sum arithmetic
// is done in long long to stay safe against overflow of intermediate
// prefix sums (row zone lengths/values are not otherwise bounded in the
// statement).
#include <cstdio>
#include <algorithm>
using namespace std;

namespace fastio {
    const int BUFSZ = 1 << 20;
    char buf[BUFSZ];
    int bufLen = 0, bufPos = 0;
    inline int gc() {
        if (bufPos == bufLen) {
            bufLen = (int)fread(buf, 1, BUFSZ, stdin);
            bufPos = 0;
            if (bufLen == 0) return -1;
        }
        return buf[bufPos++];
    }
    inline bool readInt(long long &out) {
        int c = gc();
        while (c != -1 && c != '-' && (c < '0' || c > '9')) c = gc();
        if (c == -1) return false;
        bool neg = false;
        if (c == '-') { neg = true; c = gc(); }
        long long val = 0;
        while (c >= '0' && c <= '9') { val = val * 10 + (c - '0'); c = gc(); }
        out = neg ? -val : val;
        return true;
    }
}

const int MAXN = 105;
const int MAXM = 10005;

static long long welcome[MAXN][MAXM];
static long long length_[MAXN][MAXM];

static long long W[MAXM], L[MAXM];
static int dq[MAXM];

// Computes, for a row of m zones (1..m) with values w[] and lengths len[],
// east[j] = max over c<=j with L[j]-L[c]<=k of dpIn[c] + (W[j]-W[c]).
static void computeEast(const long long *dpIn, const long long *w, const long long *len,
                         int m, long long k, long long *eastOut) {
    W[0] = 0; L[0] = 0;
    for (int t = 1; t <= m; t++) {
        W[t] = W[t - 1] + w[t];
        L[t] = L[t - 1] + len[t];
    }
    int head = 0, tail = 0;
    for (int j = 0; j <= m; j++) {
        long long keyJ = dpIn[j] - W[j];
        while (tail > head && (dpIn[dq[tail - 1]] - W[dq[tail - 1]]) <= keyJ) tail--;
        dq[tail++] = j;
        while (L[j] - L[dq[head]] > k) head++;
        eastOut[j] = dpIn[dq[head]] - W[dq[head]] + W[j];
    }
}

static long long dpIn[MAXM], dpOut[MAXM];
static long long dpInRev[MAXM], wRev[MAXM], lenRev[MAXM];
static long long eastArr[MAXM], eastRevArr[MAXM];

int main() {
    long long n, m, k;
    while (fastio::readInt(n) && fastio::readInt(m) && fastio::readInt(k)) {
        if (n == 0 && m == 0 && k == 0) break;
        int N = (int)n, M = (int)m;
        for (int i = 0; i <= N; i++)
            for (int t = 1; t <= M; t++) {
                long long v; fastio::readInt(v); welcome[i][t] = v;
            }
        for (int i = 0; i <= N; i++)
            for (int t = 1; t <= M; t++) {
                long long v; fastio::readInt(v); length_[i][t] = v;
            }

        for (int j = 0; j <= M; j++) dpIn[j] = 0;

        long long ans = 0;
        for (int i = N; i >= 0; i--) {
            computeEast(dpIn, welcome[i], length_[i], M, k, eastArr);
            for (int j = 0; j <= M; j++) dpInRev[j] = dpIn[M - j];
            for (int t = 1; t <= M; t++) {
                wRev[t] = welcome[i][M + 1 - t];
                lenRev[t] = length_[i][M + 1 - t];
            }
            computeEast(dpInRev, wRev, lenRev, M, k, eastRevArr);
            for (int j = 0; j <= M; j++) {
                long long westVal = eastRevArr[M - j];
                dpOut[j] = eastArr[j] > westVal ? eastArr[j] : westVal;
            }
            if (i == 0) {
                ans = dpOut[0];
                for (int j = 1; j <= M; j++) if (dpOut[j] > ans) ans = dpOut[j];
            } else {
                for (int j = 0; j <= M; j++) dpIn[j] = dpOut[j];
            }
        }
        printf("%lld\n", ans);
    }
    return 0;
}
