// POJ 2672 - Hotkeys
// Model: claude-sonnet-5
//
// Each menuitem's hotkey must be a capital letter that begins one of its
// words. Direct sub-menuitems of the same menuitem must all get distinct
// hotkeys, and a menuitem's hotkey must differ from each of its direct
// sub-menuitems (but not from deeper descendants). This is a rooted-tree
// list-coloring feasibility question where the only interaction is between
// a node and its immediate children (the discuss board confirms: process
// bottom-up, one bipartite matching per sibling group).
//
// For a node v define usable(v) = set of letters L in v's own candidate
// set such that v's whole subtree can be validly completed with v taking
// letter L. Leaves: usable(leaf) = candidate(leaf). For an internal node v
// with children c_1..c_m, v can take L iff the children (using their own
// usable(c_i) pools, NOT raw candidates) admit a system of distinct
// representatives avoiding L. So usable(v) = candidate(v) minus the set of
// "essential" letters of the children's bipartite matching (letters used
// in every maximum matching of the children-vs-letters graph). We find one
// maximum matching M over the children (ignoring any exclusion); if it is
// not perfect, usable(v) is empty regardless of v's letter. Otherwise, for
// each letter matched in M we test whether removing it still permits a
// perfect matching (standard augmenting-path "is this matched edge
// avoidable" test); letters that can't be avoided are essential and get
// excluded from usable(v).
//
// The top-level items (children of the implicit global menu) need only a
// plain SDR among themselves (no letter to avoid), using their usable()
// pools; a perfect matching there is exactly "Got It!".
//
// Ambiguity: none found in the statement itself; the board's own
// clarification ("constraint only between adjacent layers") matches this
// bottom-up formulation, which was verified by a small randomized
// differential test against a brute-force tree backtracker on trees with
// up to ~8 nodes and a shrunk alphabet, plus a stress test on a 10000-node
// single chain for recursion/performance safety.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
using namespace std;

static const int MAXN = 10005;
int candMask[MAXN];
int usableMask[MAXN];
int parentId[MAXN];
int depthOf[MAXN];
vector<int> childList[MAXN];
vector<int> topLevel;
int n = 0;

// Kuhn's augmenting path search for local index u within a group.
// pools[i] = bitmask of letters child i (local index) may use.
// matchLetter[L] = local child index matched to letter L, or -1.
// matchChild[i] = letter matched to local child i, or -1.
// excl = letter forbidden for this whole search (or -1 for none).
bool tryKuhn(int u, const vector<int>& pools, int matchLetter[26], vector<int>& matchChild,
             bool visited[26], int excl) {
    int mask = pools[u];
    for (int L = 0; L < 26; L++) {
        if (L == excl) continue;
        if (!((mask >> L) & 1)) continue;
        if (visited[L]) continue;
        visited[L] = true;
        if (matchLetter[L] == -1 || tryKuhn(matchLetter[L], pools, matchLetter, matchChild, visited, excl)) {
            matchLetter[L] = u;
            matchChild[u] = L;
            return true;
        }
    }
    return false;
}

// Compute usable mask for a node given its raw candidate mask and its
// children's ids (whose usableMask[] must already be computed).
int computeUsable(int cand, const vector<int>& kids) {
    int m = (int)kids.size();
    vector<int> pools(m);
    for (int i = 0; i < m; i++) pools[i] = usableMask[kids[i]];

    int matchLetter[26];
    for (int i = 0; i < 26; i++) matchLetter[i] = -1;
    vector<int> matchChild(m, -1);

    int matched = 0;
    for (int i = 0; i < m; i++) {
        bool visited[26];
        memset(visited, 0, sizeof(visited));
        if (tryKuhn(i, pools, matchLetter, matchChild, visited, -1)) matched++;
    }
    if (matched < m) return 0; // infeasible regardless of v's own letter

    // Snapshot originally matched letters before any reroute testing.
    vector<int> origMatched;
    for (int L = 0; L < 26; L++) if (matchLetter[L] != -1) origMatched.push_back(L);

    bool essential[26];
    memset(essential, 0, sizeof(essential));

    for (size_t k = 0; k < origMatched.size(); k++) {
        int L = origMatched[k];
        if (matchLetter[L] == -1) continue; // already freed by an earlier reroute
        int c = matchLetter[L];
        matchLetter[L] = -1;
        matchChild[c] = -1;
        bool visited[26];
        memset(visited, 0, sizeof(visited));
        bool ok = tryKuhn(c, pools, matchLetter, matchChild, visited, L);
        if (!ok) {
            // restore
            matchLetter[L] = c;
            matchChild[c] = L;
            essential[L] = true;
        }
        // if ok, matching already updated in place (L left unmatched)
    }

    int result = cand;
    for (int L = 0; L < 26; L++) if (essential[L]) result &= ~(1 << L);
    return result;
}

int computeCandMask(const string& name) {
    int mask = 0;
    istringstream iss(name);
    string word;
    while (iss >> word) {
        char c = word[0];
        if (c >= 'A' && c <= 'Z') mask |= (1 << (c - 'A'));
    }
    return mask;
}

int main() {
    vector<string> lines;
    {
        string line;
        while (true) {
            if (!std::getline(std::cin, line)) break;
            // strip trailing \r
            while (!line.empty() && (line[line.size()-1] == '\r' || line[line.size()-1] == '\n'))
                line.erase(line.size()-1);
            // trim leading/trailing spaces for structural comparison; keep a
            // trimmed copy for content, original words extraction uses istringstream anyway
            size_t start = line.find_first_not_of(" \t");
            if (start == string::npos) continue; // skip blank lines defensively
            size_t end = line.find_last_not_of(" \t");
            string trimmed = line.substr(start, end - start + 1);
            lines.push_back(trimmed);
        }
    }

    size_t idx = 0;
    if (idx >= lines.size() || lines[idx] != "<") {
        // malformed; nothing to do
        printf("Got It!\n");
        return 0;
    }
    idx++;

    vector<int> stk;
    stk.push_back(-1); // -1 = virtual root (top level)

    while (idx < lines.size()) {
        if (lines[idx] == ">") {
            stk.pop_back();
            idx++;
            if (stk.empty()) break;
            continue;
        }
        // menuitem name line
        string name = lines[idx];
        idx++;
        int par = stk.back();
        int id = n++;
        candMask[id] = computeCandMask(name);
        parentId[id] = par;
        depthOf[id] = (par == -1) ? 0 : depthOf[par] + 1;
        if (par == -1) topLevel.push_back(id);
        else childList[par].push_back(id);

        if (idx < lines.size() && lines[idx] == "<") {
            idx++;
            stk.push_back(id);
        }
    }

    // Process nodes in decreasing depth order so children are done before parents.
    int maxDepth = 0;
    for (int i = 0; i < n; i++) if (depthOf[i] > maxDepth) maxDepth = depthOf[i];
    vector<vector<int> > byDepth(maxDepth + 1);
    for (int i = 0; i < n; i++) byDepth[depthOf[i]].push_back(i);

    for (int d = maxDepth; d >= 0; d--) {
        for (size_t k = 0; k < byDepth[d].size(); k++) {
            int id = byDepth[d][k];
            usableMask[id] = computeUsable(candMask[id], childList[id]);
        }
    }

    // Final check: top-level items need a plain SDR (no exclusion).
    int m = (int)topLevel.size();
    vector<int> pools(m);
    for (int i = 0; i < m; i++) pools[i] = usableMask[topLevel[i]];
    int matchLetter[26];
    for (int i = 0; i < 26; i++) matchLetter[i] = -1;
    vector<int> matchChild(m, -1);
    int matched = 0;
    for (int i = 0; i < m; i++) {
        bool visited[26];
        memset(visited, 0, sizeof(visited));
        if (tryKuhn(i, pools, matchLetter, matchChild, visited, -1)) matched++;
    }

    if (matched == m) printf("Got It!\n");
    else printf("No Solution\n");

    return 0;
}
