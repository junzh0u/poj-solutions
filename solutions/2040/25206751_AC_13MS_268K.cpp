// POJ 2040 - Translations
// Model: claude-sonnet-5
//
// Each language's phrases give a directed graph on its distinct words:
// an edge word1 -> word2 for every phrase (word1, word2). A valid
// translation is a bijection between language-1 words and language-2
// words that maps the multiset of language-1 edges exactly onto the
// multiset of language-2 edges (a directed multigraph isomorphism);
// the problem guarantees this isomorphism is unique. We find it with
// backtracking search: assign language-1 nodes to language-2 nodes one
// at a time, always picking next the node with the fewest remaining
// feasible candidates (MRV heuristic), pruning candidates by matching
// out-degree/in-degree/self-loop count and by edge-count consistency
// with every already-fixed pair. With at most 25 distinct words per
// language and out/in-degree bounded by 10, this converges essentially
// immediately.
//
// The statement's alphabetical ordering of each language's phrase list
// is not needed by the algorithm: each input line already gives a
// correctly paired (word1, word2) phrase, so all n pairs per language
// are read directly as graph edges regardless of line order.
//
// Output words are sorted case-insensitively (A,a,B,b,C,c,... order),
// per the problem's discuss board (message 9628), with original
// spelling as a tie-break for words equal under case folding.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cctype>
using namespace std;

const int MAXW = 30;
int m;
int cnt1[MAXW][MAXW], cnt2[MAXW][MAXW];
int outdeg1[MAXW], indeg1[MAXW], outdeg2[MAXW], indeg2[MAXW];
int p[MAXW];
bool used2[MAXW];

bool feasible(int i, int j) {
    if (outdeg1[i] != outdeg2[j]) return false;
    if (indeg1[i] != indeg2[j]) return false;
    if (cnt1[i][i] != cnt2[j][j]) return false;
    for (int k = 0; k < m; ++k) {
        if (p[k] == -1) continue;
        if (cnt1[i][k] != cnt2[j][p[k]]) return false;
        if (cnt1[k][i] != cnt2[p[k]][j]) return false;
    }
    return true;
}

bool solve(int assignedCount) {
    if (assignedCount == m) return true;
    int bestI = -1;
    vector<int> bestCands;
    for (int i = 0; i < m; ++i) {
        if (p[i] != -1) continue;
        vector<int> cands;
        for (int j = 0; j < m; ++j) {
            if (used2[j]) continue;
            if (feasible(i, j)) cands.push_back(j);
        }
        if (bestI == -1 || cands.size() < bestCands.size()) {
            bestI = i;
            bestCands = cands;
            if (bestCands.empty()) break;
        }
    }
    if (bestI == -1 || bestCands.empty()) return false;
    for (size_t t = 0; t < bestCands.size(); ++t) {
        int j = bestCands[t];
        p[bestI] = j;
        used2[j] = true;
        if (solve(assignedCount + 1)) return true;
        p[bestI] = -1;
        used2[j] = false;
    }
    return false;
}

string lowered(const string &s) {
    string r = s;
    for (size_t i = 0; i < r.size(); ++i) r[i] = (char)tolower((unsigned char)r[i]);
    return r;
}

bool wordLess(const string &a, const string &b) {
    string la = lowered(a), lb = lowered(b);
    if (la != lb) return la < lb;
    return a < b;
}

int main() {
    int n;
    bool firstSet = true;
    while (scanf("%d", &n) == 1 && n != 0) {
        vector<string> w1a(n), w1b(n), w2a(n), w2b(n);
        char buf[64];
        for (int i = 0; i < n; ++i) {
            scanf("%s", buf); w1a[i] = buf;
            scanf("%s", buf); w1b[i] = buf;
        }
        for (int i = 0; i < n; ++i) {
            scanf("%s", buf); w2a[i] = buf;
            scanf("%s", buf); w2b[i] = buf;
        }

        map<string,int> idx1, idx2;
        vector<string> words1, words2;
        for (int i = 0; i < n; ++i) {
            if (idx1.find(w1a[i]) == idx1.end()) { idx1[w1a[i]] = (int)words1.size(); words1.push_back(w1a[i]); }
            if (idx1.find(w1b[i]) == idx1.end()) { idx1[w1b[i]] = (int)words1.size(); words1.push_back(w1b[i]); }
            if (idx2.find(w2a[i]) == idx2.end()) { idx2[w2a[i]] = (int)words2.size(); words2.push_back(w2a[i]); }
            if (idx2.find(w2b[i]) == idx2.end()) { idx2[w2b[i]] = (int)words2.size(); words2.push_back(w2b[i]); }
        }
        m = (int)words1.size();

        memset(cnt1, 0, sizeof(cnt1));
        memset(cnt2, 0, sizeof(cnt2));
        for (int i = 0; i < n; ++i) cnt1[idx1[w1a[i]]][idx1[w1b[i]]]++;
        for (int i = 0; i < n; ++i) cnt2[idx2[w2a[i]]][idx2[w2b[i]]]++;

        for (int i = 0; i < m; ++i) {
            int od = 0, id = 0;
            for (int j = 0; j < m; ++j) { od += cnt1[i][j]; id += cnt1[j][i]; }
            outdeg1[i] = od; indeg1[i] = id;
        }
        for (int j = 0; j < m; ++j) {
            int od = 0, id = 0;
            for (int k = 0; k < m; ++k) { od += cnt2[j][k]; id += cnt2[k][j]; }
            outdeg2[j] = od; indeg2[j] = id;
        }

        for (int i = 0; i < m; ++i) p[i] = -1;
        for (int i = 0; i < m; ++i) used2[i] = false;
        solve(0);

        vector<pair<string,string> > result(m);
        for (int i = 0; i < m; ++i) result[i] = make_pair(words1[i], words2[p[i]]);
        // insertion sort using wordLess on .first (m <= 25, so this is plenty fast)
        for (int i = 1; i < m; ++i) {
            pair<string,string> key = result[i];
            int j = i - 1;
            while (j >= 0 && wordLess(key.first, result[j].first)) {
                result[j+1] = result[j];
                --j;
            }
            result[j+1] = key;
        }

        if (!firstSet) printf("\n");
        firstSet = false;
        for (int i = 0; i < m; ++i) {
            printf("%s/%s\n", result[i].first.c_str(), result[i].second.c_str());
        }
    }
    return 0;
}
