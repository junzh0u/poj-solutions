// POJ 1795 - DNA Laboratory
// Model: claude-sonnet-5
//
// Shortest common superstring over {A,C,G,T} for up to 15 strings (each
// length 1..100), ties broken by lexicographically smallest result.
//
// After removing duplicate strings and any string that is a substring of
// another (both are redundant — the containing string already covers
// them), we have m <= 15 distinct, pairwise non-containing strings.
// overlap[i][j] = largest k such that the length-k suffix of s[i] equals
// the length-k prefix of s[j] (k < min(len_i,len_j), since neither string
// contains the other).
//
// For any fixed permutation of the m strings, the shortest string that
// results from concatenating them in that order is obtained by using the
// *maximum* possible overlap between every adjacent pair (using less only
// makes the result longer). So minimizing total length is exactly a
// maximum-weight Hamiltonian path problem in the overlap graph:
//   f[mask][i] = best total overlap achievable by optimally ordering the
//                strings NOT in mask, appended after i (i is the last
//                placed string, i is a member of mask).
//   f[full][i] = 0
//   f[mask][i] = max over j not in mask of overlap[i][j] + f[mask|1<<j][j]
// bestTotal = max over i of f[(1<<i)][i]  (choice of first string).
// This determines the minimal total length; it does NOT yet fix which of
// the (possibly many) optimal permutations gives the lexicographically
// smallest string.
//
// To get the smallest string among all optimal completions we simulate
// all optimal continuations *simultaneously*, character by character:
// a "candidate" is (cur, remMask, ov, off) meaning we are currently
// emitting the fragment of s[cur] starting at offset ov (its overlap with
// the previous string), off characters into that fragment already
// emitted, with remMask strings still left to place after cur. At each
// step we look at the next character of every live candidate, keep only
// those with the globally smallest next character, and advance. When a
// candidate finishes its fragment we expand it into all children j
// (j in remMask) that keep it on an optimal path, i.e.
// overlap[cur][j] + f[usedMask|1<<j][j] == f[usedMask][cur]. Because
// every optimal completion of a fixed state has the same total remaining
// length, this greedy per-character selection produces exactly the
// lexicographically smallest optimal superstring. Candidate count stays
// tiny in practice (m <= 15, total length <= 1500), well within the
// 30000K / 5000MS limits.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
using namespace std;

static int m;
static string s[15];
static int len_[15];
static int overlap_[15][15];
static int f[1 << 15][15];

struct Cand {
    int cur, rem, ov, off;
    bool operator<(const Cand& o) const {
        if (cur != o.cur) return cur < o.cur;
        if (rem != o.rem) return rem < o.rem;
        if (ov != o.ov) return ov < o.ov;
        return off < o.off;
    }
};

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    for (int tc = 1; tc <= T; tc++) {
        int n;
        scanf("%d", &n);
        vector<string> raw(n);
        char buf[128];
        for (int i = 0; i < n; i++) {
            scanf("%s", buf);
            raw[i] = buf;
        }
        // dedup exact duplicates, preserve first occurrence order
        vector<string> uniq;
        for (int i = 0; i < n; i++) {
            bool dup = false;
            for (size_t k = 0; k < uniq.size(); k++)
                if (uniq[k] == raw[i]) { dup = true; break; }
            if (!dup) uniq.push_back(raw[i]);
        }
        // remove strings that are substrings of another distinct string
        vector<string> filtered;
        int un = (int)uniq.size();
        for (int i = 0; i < un; i++) {
            bool contained = false;
            for (int j = 0; j < un; j++) {
                if (j == i) continue;
                if (uniq[j].find(uniq[i]) != string::npos) { contained = true; break; }
            }
            if (!contained) filtered.push_back(uniq[i]);
        }
        m = (int)filtered.size();
        for (int i = 0; i < m; i++) {
            s[i] = filtered[i];
            len_[i] = (int)s[i].size();
        }
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < m; j++) {
                if (i == j) { overlap_[i][j] = 0; continue; }
                int mx = min(len_[i], len_[j]);
                int best = 0;
                for (int k = mx; k >= 0; k--) {
                    if (k == 0) { best = 0; break; }
                    if (s[i].compare(len_[i] - k, k, s[j], 0, k) == 0) { best = k; break; }
                }
                overlap_[i][j] = best;
            }
        }
        int full = (1 << m) - 1;
        // f[mask][i], process mask from full down to 0 (mask|bit > mask always)
        for (int mask = full; mask >= 0; mask--) {
            for (int i = 0; i < m; i++) {
                if (!(mask & (1 << i))) continue;
                if (mask == full) { f[mask][i] = 0; continue; }
                int best = -1;
                for (int j = 0; j < m; j++) {
                    if (mask & (1 << j)) continue;
                    int val = overlap_[i][j] + f[mask | (1 << j)][j];
                    if (val > best) best = val;
                }
                f[mask][i] = best;
            }
        }
        int bestTotal = -1;
        for (int i = 0; i < m; i++) {
            int v = f[1 << i][i];
            if (v > bestTotal) bestTotal = v;
        }
        int totalLen = 0;
        for (int i = 0; i < m; i++) totalLen += len_[i];
        totalLen -= bestTotal;

        // initial candidates
        vector<Cand> cands;
        for (int i = 0; i < m; i++) {
            if (f[1 << i][i] == bestTotal) {
                Cand c; c.cur = i; c.rem = full ^ (1 << i); c.ov = 0; c.off = 0;
                cands.push_back(c);
            }
        }

        string ans;
        ans.reserve(totalLen);
        int produced = 0;
        while (produced < totalLen) {
            // expand any candidate that has finished its current fragment
            vector<Cand> expanded;
            set<long long> seen;
            for (size_t idx = 0; idx < cands.size(); idx++) {
                Cand c = cands[idx];
                int fragLen = len_[c.cur] - c.ov;
                if (c.off == fragLen) {
                    if (c.rem == 0) {
                        // fully finished; contributes no more chars
                        continue;
                    }
                    int usedMask = full ^ c.rem;
                    int targetF = f[usedMask][c.cur];
                    for (int j = 0; j < m; j++) {
                        if (!(c.rem & (1 << j))) continue;
                        int val = overlap_[c.cur][j] + f[(usedMask | (1 << j))][j];
                        if (val == targetF) {
                            Cand nc; nc.cur = j; nc.rem = c.rem ^ (1 << j);
                            nc.ov = overlap_[c.cur][j]; nc.off = 0;
                            long long key = (((long long)nc.cur << 15 | nc.rem) << 7 | nc.ov) << 7 | nc.off;
                            if (seen.insert(key).second) expanded.push_back(nc);
                        }
                    }
                } else {
                    long long key = (((long long)c.cur << 15 | c.rem) << 7 | c.ov) << 7 | c.off;
                    if (seen.insert(key).second) expanded.push_back(c);
                }
            }
            cands.swap(expanded);

            // find min next char
            char mn = 'Z';
            for (size_t idx = 0; idx < cands.size(); idx++) {
                const Cand& c = cands[idx];
                char ch = s[c.cur][c.ov + c.off];
                if (ch < mn) mn = ch;
            }
            vector<Cand> survivors;
            survivors.reserve(cands.size());
            for (size_t idx = 0; idx < cands.size(); idx++) {
                Cand c = cands[idx];
                char ch = s[c.cur][c.ov + c.off];
                if (ch == mn) {
                    c.off++;
                    survivors.push_back(c);
                }
            }
            ans.push_back(mn);
            produced++;
            cands.swap(survivors);
        }

        printf("Scenario #%d:\n%s\n\n", tc, ans.c_str());
    }
    return 0;
}
