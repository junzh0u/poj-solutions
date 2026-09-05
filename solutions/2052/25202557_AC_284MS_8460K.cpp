// POJ 2052 - Fun Game
// Model: claude-sonnet-5
//
// Approach: The kids sit in an unknown circular sequence of B/G. Each paper's
// string is a contiguous run of the (bi-infinite, period-N) circular sequence,
// read in one of the two directions (clockwise/counter-clockwise), which for a
// fixed reading direction on the circle means the paper's string equals either
// the string itself or its reverse read forward on the circle.
//
// A paper's string can be LONGER than the circle (the paper can wrap around
// the same kids more than once), so we must consider two regimes:
//
//   (a) "Wrap" regime: circle size N <= max(paper length). Any paper with
//       length >= N is forced to be periodic with period N (fold-check), and
//       its first N characters (up to rotation) fully pin down the circle
//       (since it wraps at least once). All such "forcing" strings (in some
//       chosen orientation) must agree on the same necklace up to rotation;
//       we intersect, for every N from 2..maxLen, the set of canonical
//       (lexicographically-minimal-rotation) necklaces each forcing string can
//       produce, and if a common one exists, verify every shorter paper
//       occurs (forward or reversed) inside the doubled necklace.
//
//   (b) "No-wrap" regime: circle size N >= max(paper length), i.e. every
//       paper occupies a literal contiguous non-overlapping-with-itself arc.
//       This is the classical "shortest cyclic superstring with reversal
//       freedom" problem: after removing strings that are (in either
//       orientation) a plain substring of another (they add no information),
//       solve a bitmask DP over a graph whose nodes are (string, orientation)
//       pairs, maximizing total suffix/prefix overlap along a Hamiltonian
//       cycle that visits each remaining string exactly once. Answer here is
//       total length minus the best overlap sum (always >= max length).
//
// The true answer is the minimum over regime (a) (scanned ascending, so the
// first hit is already the global minimum, since every candidate there is
// <= maxLen <= regime (b)'s value) and regime (b).
//
// The discuss board's separating test case (2052 forum, thread "这组数据您认为
// 应该输出什么？"): strings "BGGBBBBGB" and "BGBBBBGGB" -> multiple accepted
// solutions agree the answer is 8 (one poster guessed 7, but the accepted
// programs output 8); used as a local regression test alongside the sample.

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

static int n;
static string s[16], rs[16];
static int slen[16];

static string canonicalRotation(const string &t) {
    int N = (int)t.size();
    string best = t;
    for (int r = 1; r < N; r++) {
        string cand = t.substr(r) + t.substr(0, r);
        if (cand < best) best = cand;
    }
    return best;
}

static bool foldCheck(const string &t, int N, string &necklace) {
    int L = (int)t.size();
    for (int i = 0; i + N < L; i++) {
        if (t[i] != t[i + N]) return false;
    }
    necklace = t.substr(0, N);
    return true;
}

static int solveWrapRegime(int maxL) {
    for (int N = 2; N <= maxL; N++) {
        bool ok = true;
        map<string, int> cnt;
        int numForced = 0;
        vector<int> shortIdx;
        for (int i = 0; i < n && ok; i++) {
            if (slen[i] >= N) {
                numForced++;
                string nk1, nk2;
                bool ok1 = foldCheck(s[i], N, nk1);
                bool ok2 = foldCheck(rs[i], N, nk2);
                if (!ok1 && !ok2) { ok = false; break; }
                map<string, bool> seen;
                if (ok1) {
                    string c = canonicalRotation(nk1);
                    if (!seen[c]) { seen[c] = true; cnt[c]++; }
                }
                if (ok2) {
                    string c = canonicalRotation(nk2);
                    if (!seen[c]) { seen[c] = true; cnt[c]++; }
                }
            } else {
                shortIdx.push_back(i);
            }
        }
        if (!ok) continue;
        // find candidate necklaces with full intersection
        vector<string> candidates;
        for (map<string, int>::iterator it = cnt.begin(); it != cnt.end(); ++it) {
            if (it->second == numForced) candidates.push_back(it->first);
        }
        for (size_t c = 0; c < candidates.size(); c++) {
            string necklace2 = candidates[c] + candidates[c];
            bool allOk = true;
            for (size_t k = 0; k < shortIdx.size(); k++) {
                int i = shortIdx[k];
                if (necklace2.find(s[i]) == string::npos &&
                    necklace2.find(rs[i]) == string::npos) {
                    allOk = false;
                    break;
                }
            }
            if (allOk) return N;
        }
    }
    return -1; // none feasible in this range
}

static int overlapText(const string &a, const string &b) {
    int la = (int)a.size(), lb = (int)b.size();
    int maxk = min(la, lb);
    for (int k = maxk; k >= 1; k--) {
        if (a.compare(la - k, k, b, 0, k) == 0) return k;
    }
    return 0;
}

static int dp[1 << 16][32];

static int solveNoWrapRegime() {
    bool removed[16];
    memset(removed, 0, sizeof(removed));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) continue;
            if (slen[j] < slen[i]) continue;
            if (slen[j] == slen[i]) {
                bool eq = (s[j] == s[i] || s[j] == rs[i]);
                if (!eq) continue;
                if (j >= i) continue; // tie-break: dominated only by earlier index
            } else {
                if (s[j].find(s[i]) == string::npos &&
                    s[j].find(rs[i]) == string::npos) continue;
            }
            removed[i] = true;
            break;
        }
    }
    vector<int> keep;
    for (int i = 0; i < n; i++) if (!removed[i]) keep.push_back(i);
    int m = (int)keep.size();

    vector<string> txt(2 * m);
    vector<int> L(m);
    int totalLen = 0;
    for (int k = 0; k < m; k++) {
        txt[2 * k] = s[keep[k]];
        txt[2 * k + 1] = rs[keep[k]];
        L[k] = slen[keep[k]];
        totalLen += L[k];
    }
    if (m == 1) return L[0];

    int numNodes = 2 * m;
    static int ov[32][32];
    for (int a = 0; a < numNodes; a++) {
        for (int b = 0; b < numNodes; b++) {
            if (a / 2 == b / 2) { ov[a][b] = 0; continue; }
            ov[a][b] = overlapText(txt[a], txt[b]);
        }
    }

    int fullMask = (1 << m) - 1;
    int bestTotalOverlap = -1;

    for (int startOr = 0; startOr < 2; startOr++) {
        int startNode = startOr; // string index 0, orientation startOr
        for (int mask = 0; mask <= fullMask; mask++)
            for (int node = 0; node < numNodes; node++) dp[mask][node] = -1;
        dp[1][startNode] = 0;
        for (int mask = 1; mask <= fullMask; mask++) {
            if (!(mask & 1)) continue;
            for (int last = 0; last < numNodes; last++) {
                if (dp[mask][last] < 0) continue;
                int lastString = last / 2;
                if (!((mask >> lastString) & 1)) continue;
                for (int k = 0; k < m; k++) {
                    if ((mask >> k) & 1) continue;
                    for (int ori = 0; ori < 2; ori++) {
                        int nxt = 2 * k + ori;
                        int val = dp[mask][last] + ov[last][nxt];
                        int nmask = mask | (1 << k);
                        if (val > dp[nmask][nxt]) dp[nmask][nxt] = val;
                    }
                }
            }
        }
        for (int last = 0; last < numNodes; last++) {
            if (dp[fullMask][last] < 0) continue;
            int lastString = last / 2;
            if (lastString == 0) continue;
            int total = dp[fullMask][last] + ov[last][startNode];
            if (total > bestTotalOverlap) bestTotalOverlap = total;
        }
    }
    return totalLen - bestTotalOverlap;
}

int main() {
    while (scanf("%d", &n) == 1 && n != 0) {
        int maxL = 0;
        for (int i = 0; i < n; i++) {
            char buf[110];
            scanf("%s", buf);
            s[i] = buf;
            rs[i] = s[i];
            reverse(rs[i].begin(), rs[i].end());
            slen[i] = (int)s[i].size();
            if (slen[i] > maxL) maxL = slen[i];
        }
        int a = solveWrapRegime(maxL);
        int ans;
        if (a != -1) {
            ans = a;
        } else {
            ans = solveNoWrapRegime();
        }
        // The problem guarantees at least two kids; an unconstrained minimum
        // of 1 (all strings consist of a single repeated character) is always
        // achievable at N=2 too (double the single character), so clamp.
        if (ans < 2) ans = 2;
        printf("%d\n", ans);
    }
    return 0;
}
