// POJ 1382 - The Proper Key
// Model: claude-sonnet-5
//
// The key (R x C) can only move straight down and slide left/right (no
// rotation) inside the lock (D x W). Let v be the vertical offset of the
// key's top row relative to the lock's top row (v negative while the key is
// still above the lock, v >= D once it has fully passed below). Depth d
// (distance from the key's bottom to the lock's top) equals v + R. Key cell
// (r,c) collides with lock cell (v+r, x+c) iff both are '#' and
// 0 <= v+r < D.
//
// We simulate d = 1, 2, ... one step at a time, keeping the set of
// horizontal offsets x in [0, W-C] that are reachable. Going from d-1 to d:
// an offset survives as a "seed" if it was reachable before and is not
// blocked at the new depth; then every maximal free run of x's (not
// blocked) that contains a seed becomes fully reachable, since sliding
// within a free run is free. If the reachable set empties, the previous
// depth is the answer; if it survives through d = D+R-1 (the last depth
// where any lock row still participates) the key falls all the way
// through.
//
// "Blocked at depth d" is a bitwise correlation between one key row and one
// lock row, computed for every x at once: blocked |= OR_{c in key row's set
// bits} (lock row >> c). Doing this with 64-bit words makes the D*R
// row-pair correlations (the dominant cost) cheap. For rows that are mostly
// a few contiguous runs of '#' (very common: the statement guarantees the
// key stays connected even after cutting rows off its top, which tends to
// keep runs long and few) we instead mark blocked ranges directly with a
// difference array: a run [lo,hi] on one side and a single '#' at position p
// on the other blocks exactly x in [p-hi, p-lo], and two runs overlap
// (shifted by x) exactly on a contiguous range of x too. For each row pair
// we pick whichever of these representations (plain bitset shift, run x
// point, or run x run) is cheapest by its actual set-bit/run counts.
//
// Lock rows are only needed while they sit inside the R-row sliding window
// that participates in the current depth's check, so their run/point lists
// are computed lazily into an R-slot cache instead of being kept for all D
// rows (D can be 10000, so keeping every row's list would risk the 32MB
// limit).
//
// Verification: matches all 4 official samples, matches ~1300 random
// differential trials against a brute-force reference (same physical model,
// no bitset trickery) across small and medium sizes, and matches the
// board's own separating case from message 54063 (a "hook" key ## / #. /
// #. / #. dropped into lock #.# / ###, which the thread confirms falls to
// depth 1, not depth 3 as a naive "insert from the left" reading would
// suggest) -- kept as test_data/1382-hook.in/.out.
#include <cstdio>
#include <cstring>
#include <vector>
#include <utility>
using namespace std;

typedef unsigned long long u64;
typedef pair<int,int> pii;

static int R, C, D, W;
static char keyBuf[105][105];
static vector<char> lockBuf;

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T--) {
        scanf("%d %d", &R, &C);
        for (int r = 0; r < R; ++r) scanf("%s", keyBuf[r]);
        scanf("%d %d", &D, &W);
        lockBuf.assign((size_t)D * W, '.');
        for (int i = 0; i < D; ++i) {
            static char tmp[1005];
            scanf("%s", tmp);
            memcpy(&lockBuf[(size_t)i * W], tmp, W);
        }

        int numX = W - C + 1;
        if (numX <= 0) { printf("The key falls to depth 0.\n"); continue; }

        int wlen = (W + 63) / 64;

        // ---- key row structures (only R rows, precompute fully) ----
        vector<vector<int> > keyPoints(R);
        vector<vector<pii> > keyRuns(R);
        for (int r = 0; r < R; ++r) {
            for (int c = 0; c < C; ++c) if (keyBuf[r][c] == '#') keyPoints[r].push_back(c);
            int c = 0;
            while (c < C) {
                if (keyBuf[r][c] != '#') { ++c; continue; }
                int start = c;
                while (c < C && keyBuf[r][c] == '#') ++c;
                keyRuns[r].push_back(pii(start, c - 1));
            }
        }

        // ---- lock rows: bitset words for all D rows, flattened into one
        // contiguous array for cache-friendly access, plus lazily-cached
        // points/runs for a sliding window of R rows ----
        vector<u64> lockWordsFlat((size_t)D * wlen, 0ULL);
        vector<int> lockPopcountAll(D, 0);
        for (int i = 0; i < D; ++i) {
            const char *row = &lockBuf[(size_t)i * W];
            u64 *dst = &lockWordsFlat[(size_t)i * wlen];
            int pc = 0;
            for (int j = 0; j < W; ++j) {
                if (row[j] == '#') { dst[j >> 6] |= (1ULL << (j & 63)); ++pc; }
            }
            lockPopcountAll[i] = pc;
        }

        int cacheSize = R > 0 ? R : 1;
        vector<int> cachedL(cacheSize, -1);
        vector<vector<int> > lockPointsCache(cacheSize);
        vector<vector<pii> > lockRunsCache(cacheSize);

        vector<u64> blockedWords(wlen);
        vector<int> diffArr(numX + 2);
        vector<char> reachable(numX, 1);
        vector<char> newReachable(numX, 0);
        vector<char> freeMask(numX, 0);
        vector<int> countArr(numX);

        int lastGood = 0;
        bool fellThrough = false;
        int dMax = D + R - 1;

        for (int d = 1; d <= dMax; ++d) {
            for (int w = 0; w < wlen; ++w) blockedWords[w] = 0ULL;
            for (int x = 0; x <= numX; ++x) diffArr[x] = 0;

            for (int r = 0; r < R; ++r) {
                int L = d - R + r;
                if (L < 0 || L >= D) continue;
                if (keyPoints[r].empty()) continue;
                if (lockPopcountAll[L] == 0) continue;

                int slot = L % cacheSize;
                if (cachedL[slot] != L) {
                    vector<int> &pts = lockPointsCache[slot];
                    vector<pii> &runs = lockRunsCache[slot];
                    pts.clear(); runs.clear();
                    const char *row = &lockBuf[(size_t)L * W];
                    for (int j = 0; j < W; ++j) if (row[j] == '#') pts.push_back(j);
                    int j = 0;
                    while (j < W) {
                        if (row[j] != '#') { ++j; continue; }
                        int start = j;
                        while (j < W && row[j] == '#') ++j;
                        runs.push_back(pii(start, j - 1));
                    }
                    cachedL[slot] = L;
                }
                const vector<int> &lockPts = lockPointsCache[slot];
                const vector<pii> &lockRuns = lockRunsCache[slot];
                const vector<pii> &kRuns = keyRuns[r];
                const vector<int> &kPts = keyPoints[r];

                long costBitset = (long)kPts.size() * wlen;
                long costSeg = (long)kRuns.size() * lockRuns.size();
                long costA = (long)kRuns.size() * lockPts.size();
                long costB = (long)lockRuns.size() * kPts.size();

                long best = costBitset;
                int method = 0;
                if (costSeg < best) { best = costSeg; method = 3; }
                if (costA < best) { best = costA; method = 1; }
                if (costB < best) { best = costB; method = 2; }

                if (method == 0) {
                    const u64 *src = &lockWordsFlat[(size_t)L * wlen];
                    for (size_t bi = 0; bi < kPts.size(); ++bi) {
                        int c = kPts[bi];
                        int wordShift = c >> 6;
                        int bitShift = c & 63;
                        if (bitShift == 0) {
                            for (int w = 0; w + wordShift < wlen; ++w) {
                                blockedWords[w] |= src[w + wordShift];
                            }
                        } else {
                            for (int w = 0; w < wlen; ++w) {
                                int idx = w + wordShift;
                                u64 val = 0ULL;
                                if (idx < wlen) {
                                    val = src[idx] >> bitShift;
                                    if (idx + 1 < wlen) val |= src[idx + 1] << (64 - bitShift);
                                }
                                blockedWords[w] |= val;
                            }
                        }
                    }
                } else if (method == 3) {
                    // key run [lo,hi] overlaps lock run [a,b] (shifted by x)
                    // iff x in [a-hi, b-lo]
                    for (size_t ri = 0; ri < kRuns.size(); ++ri) {
                        int lo = kRuns[ri].first, hi = kRuns[ri].second;
                        for (size_t si = 0; si < lockRuns.size(); ++si) {
                            int a = lockRuns[si].first, b = lockRuns[si].second;
                            int lx = a - hi, rx = b - lo;
                            if (lx < 0) lx = 0;
                            if (rx > numX - 1) rx = numX - 1;
                            if (lx <= rx) { diffArr[lx] += 1; diffArr[rx + 1] -= 1; }
                        }
                    }
                } else if (method == 1) {
                    for (size_t ri = 0; ri < kRuns.size(); ++ri) {
                        int lo = kRuns[ri].first, hi = kRuns[ri].second;
                        for (size_t pi = 0; pi < lockPts.size(); ++pi) {
                            int p = lockPts[pi];
                            int a = p - hi, b = p - lo;
                            if (a < 0) a = 0;
                            if (b > numX - 1) b = numX - 1;
                            if (a <= b) { diffArr[a] += 1; diffArr[b + 1] -= 1; }
                        }
                    }
                } else {
                    for (size_t ri = 0; ri < lockRuns.size(); ++ri) {
                        int a0 = lockRuns[ri].first, b0 = lockRuns[ri].second;
                        for (size_t pi = 0; pi < kPts.size(); ++pi) {
                            int c = kPts[pi];
                            int a = a0 - c, b = b0 - c;
                            if (a < 0) a = 0;
                            if (b > numX - 1) b = numX - 1;
                            if (a <= b) { diffArr[a] += 1; diffArr[b + 1] -= 1; }
                        }
                    }
                }
            }

            int run = 0;
            for (int x = 0; x < numX; ++x) {
                run += diffArr[x];
                countArr[x] = run;
            }
            for (int x = 0; x < numX; ++x) {
                u64 bit = (blockedWords[x >> 6] >> (x & 63)) & 1U;
                bool blocked = (bit != 0) || (countArr[x] > 0);
                freeMask[x] = blocked ? 0 : 1;
            }

            for (int x = 0; x < numX; ++x) newReachable[x] = 0;
            int x = 0; bool any = false;
            while (x < numX) {
                if (!freeMask[x]) { ++x; continue; }
                int start = x; bool hasSeed = false;
                while (x < numX && freeMask[x]) { if (reachable[x]) hasSeed = true; ++x; }
                if (hasSeed) { for (int k = start; k < x; ++k) newReachable[k] = 1; any = true; }
            }

            if (!any) { lastGood = d - 1; fellThrough = false; break; }
            for (int k = 0; k < numX; ++k) reachable[k] = newReachable[k];
            lastGood = d;
            if (d == dMax) fellThrough = true;
        }

        if (fellThrough) printf("The key can fall through.\n");
        else printf("The key falls to depth %d.\n", lastGood);
    }
    return 0;
}
