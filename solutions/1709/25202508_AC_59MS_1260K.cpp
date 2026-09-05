// POJ 1709 - Crossword
// Model: claude-sonnet-5
//
// Approach: The output string S (length L) must admit two decompositions D1,
// D2 into dictionary words such that (a) every word used in D1 or D2 is used
// at most once overall (this single global "used" constraint captures both
// "no repeats within a decomposition" and "disjoint between D1 and D2",
// since dictionary words are pairwise distinct strings), and (b) the sets of
// internal cut positions (word boundaries strictly between 0 and L) of D1
// and D2 are disjoint.
//
// We find the lexicographically smallest S by building it one character at
// a time, always keeping a confirmed feasible prefix and testing, for each
// next character c ('a'..'z' in order), whether SOME completion to length L
// exists (a boolean oracle). The first c that keeps the prefix feasible is
// committed; by induction this greedy process never needs to backtrack
// across positions, because the oracle answers "does *any* completion
// exist", so once a prefix is confirmed feasible, some next character must
// keep it feasible.
//
// The oracle itself is a backtracking search over pairs of decompositions,
// modeled as two pointers (posA, posB) marking how far each decomposition
// has been built. We always extend whichever pointer is behind (arbitrary
// but consistent tie-break when equal), using a trie of the dictionary to
// enumerate candidate next words: words that are consistent with the
// already-fixed characters between the lagging pointer and the current
// frontier (max(posA,posB,confirmed-prefix-length)), optionally extending
// past the frontier (which then commits new characters of S). Word reuse is
// tracked with one global "used" boolean array; cut positions of each
// decomposition are tracked separately to enforce the disjoint-cut rule.
// A cheap necessary-condition prune (can the remaining length be tiled at
// all by *some* combination of available word lengths, ignoring reuse)
// short-circuits large swaths of infeasible search.
//
// Ambiguity check: the discuss board (msg 5218 et al.) asked whether a word
// may be used once total across both decompositions or once in each; the
// final answer given (msg 56566, "is the former") matches the plain English
// statement already used here (word usable at most once, period). Verified
// by hand against the statement's illustrative example and confirmed against
// the official sample (andatareadofortor decomposes as
// and|at|area|do|for|tor and an|data|read|of|ort|or, with disjoint word sets
// and disjoint internal cut positions {3,5,9,11,14} vs {2,6,10,12,15}).

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

const int MAXN = 1005;
const int MAXL = 55;
const int MAXNODES = MAXN * 21 + 10;

int N;
string words[MAXN];
int wordLen[MAXN];

int trieChildren[MAXNODES][26];
int trieIsWord[MAXNODES];
vector<int> subtreeWords[MAXNODES];
int nodeCount;

int newNode() {
    int id = nodeCount++;
    for (int c = 0; c < 26; c++) trieChildren[id][c] = -1;
    trieIsWord[id] = -1;
    return id;
}

void insertWord(const string &w, int idx) {
    int cur = 0;
    subtreeWords[0].push_back(idx);
    for (size_t k = 0; k < w.size(); k++) {
        int c = w[k] - 'a';
        if (trieChildren[cur][c] == -1) trieChildren[cur][c] = newNode();
        cur = trieChildren[cur][c];
        subtreeWords[cur].push_back(idx);
    }
    trieIsWord[cur] = idx;
}

int L;
int prefixLen;
char S[MAXL];
bool usedWord[MAXN];
bool cutA[MAXL + 2], cutB[MAXL + 2];
bool reachableLen[MAXL + 2];

bool dfs(int posA, int posB) {
    if (posA == L && posB == L) return true;
    if (!reachableLen[L - posA] || !reachableLen[L - posB]) return false;

    bool aLag = (posA <= posB);
    int m = aLag ? posA : posB;
    int M = aLag ? posB : posA;
    int Frontier = (M > prefixLen) ? M : prefixLen;
    int limit = Frontier - m;

    vector<pair<int, bool> > cand;
    int node = 0, depth = 0;
    bool broke = false;
    while (depth < limit) {
        int c = S[m + depth] - 'a';
        int nxt = trieChildren[node][c];
        if (nxt == -1) { broke = true; break; }
        node = nxt;
        depth++;
        if (trieIsWord[node] != -1) cand.push_back(make_pair(trieIsWord[node], false));
    }
    if (!broke) {
        if (trieIsWord[node] != -1) cand.push_back(make_pair(trieIsWord[node], false));
        vector<int> &sw = subtreeWords[node];
        for (size_t k = 0; k < sw.size(); k++) {
            int idx = sw[k];
            if (wordLen[idx] > limit && m + wordLen[idx] <= L)
                cand.push_back(make_pair(idx, true));
        }
    }

    bool *otherCut = aLag ? cutB : cutA;
    bool *lagCut = aLag ? cutA : cutB;

    for (size_t k = 0; k < cand.size(); k++) {
        int wid = cand[k].first;
        bool isExt = cand[k].second;
        if (usedWord[wid]) continue;
        int len = wordLen[wid];
        int e = m + len;
        if (e > L) continue;
        if (e < L && otherCut[e]) continue;

        usedWord[wid] = true;
        lagCut[e] = true;
        if (isExt) {
            const string &w = words[wid];
            for (int p = Frontier; p < e; p++) S[p] = w[p - m];
        }
        int newPosA = aLag ? e : posA;
        int newPosB = aLag ? posB : e;
        bool ok = dfs(newPosA, newPosB);
        usedWord[wid] = false;
        lagCut[e] = false;
        if (ok) return true;
    }
    return false;
}

bool feasible(int i, const string &prefix) {
    prefixLen = i;
    for (int k = 0; k < i; k++) S[k] = prefix[k];
    memset(usedWord, 0, sizeof(bool) * (N + 1));
    memset(cutA, 0, sizeof(bool) * (L + 2));
    memset(cutB, 0, sizeof(bool) * (L + 2));
    return dfs(0, 0);
}

int main() {
    scanf("%d", &L);
    scanf("%d", &N);
    nodeCount = 0;
    newNode(); // root
    for (int i = 0; i < N; i++) {
        char buf[25];
        scanf("%s", buf);
        words[i] = buf;
        wordLen[i] = (int)words[i].size();
        insertWord(words[i], i);
    }

    bool hasLen[25];
    memset(hasLen, 0, sizeof(hasLen));
    for (int i = 0; i < N; i++) if (wordLen[i] <= 24) hasLen[wordLen[i]] = true;
    reachableLen[0] = true;
    for (int len = 1; len <= L; len++) {
        reachableLen[len] = false;
        for (int wl = 2; wl <= 20 && wl <= len; wl++) {
            if (hasLen[wl] && reachableLen[len - wl]) { reachableLen[len] = true; break; }
        }
    }

    if (!feasible(0, "")) {
        printf("NO SOLUTION\n");
        return 0;
    }

    string prefix = "";
    for (int pos = 0; pos < L; pos++) {
        for (char c = 'a'; c <= 'z'; c++) {
            string trial = prefix + c;
            if (feasible(pos + 1, trial)) {
                prefix = trial;
                break;
            }
        }
    }
    printf("%s\n", prefix.c_str());
    return 0;
}
