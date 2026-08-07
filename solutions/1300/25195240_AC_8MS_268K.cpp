// POJ 1300 - Door Man
// Model: claude-opus-5
//
// The butler walks through open doors and shuts each one behind him, never
// reopening a closed one, and must finish in room 0 with every door shut.
// That is exactly an Euler trail from the start room M to room 0 in the
// multigraph whose edges are the open doors.  So the answer is YES iff
//   * every room that still has an open door is reachable from M, and
//   * the degree parity admits a trail M -> 0:
//       M == 0 : every degree even (Euler circuit),
//       M != 0 : deg(M) and deg(0) odd, all other degrees even.
// When it is YES the number of doors closed is simply the number of doors.
//
// Notes on the input format: room lines may be empty (the last one always
// is), so the whole file is read with getline rather than >>; doors may be
// repeated, which is why degrees are counted with multiplicity; each data
// set is terminated by "END" and the file by "ENDOFINPUT".
//
// Degenerate cases: with no doors at all the answer is "YES 0" when M == 0
// (he is already home with everything shut) and "NO" otherwise (he cannot
// move, so he can never reach room 0).  Both fall out of the rules above.
//
// The discuss board reports that the judge data does not actually contain a
// disconnected-but-even-parity case, so the connectivity test changes no
// verdict on it; it is kept because it is what the statement asks for, and
// it is restricted to rooms that have at least one open door (isolated
// rooms are irrelevant and testing them would wrongly reject valid cases).

#include <cstdio>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

static const int MAXN = 64;

vector<int> adj[MAXN];
int deg[MAXN];
bool seen[MAXN];

void dfs(int u) {
    seen[u] = true;
    for (size_t i = 0; i < adj[u].size(); ++i) {
        int v = adj[u][i];
        if (!seen[v]) dfs(v);
    }
}

static void chomp(string &s) {
    while (!s.empty() && (s[s.size() - 1] == '\r' || s[s.size() - 1] == '\n'))
        s.erase(s.size() - 1);
}

int main() {
    string line;
    while (getline(cin, line)) {
        chomp(line);
        if (line.empty()) continue;
        istringstream hs(line.c_str());
        string tag;
        hs >> tag;
        if (tag == "ENDOFINPUT") break;
        if (tag != "START") continue;
        int M, N;
        hs >> M >> N;
        if (N < 0) N = 0;
        if (N > MAXN - 1) N = MAXN - 1;
        for (int i = 0; i < MAXN; ++i) { adj[i].clear(); deg[i] = 0; seen[i] = false; }

        int doors = 0;
        for (int i = 0; i < N; ++i) {
            if (!getline(cin, line)) line = "";
            chomp(line);
            istringstream rs(line.c_str());
            int v;
            while (rs >> v) {
                if (v < 0 || v >= MAXN) continue;
                adj[i].push_back(v);
                adj[v].push_back(i);
                ++deg[i];
                ++deg[v];
                ++doors;
            }
        }
        getline(cin, line);   /* the "END" line */

        bool ok = true;
        if (M < 0 || M >= MAXN) {
            ok = false;
        } else {
            dfs(M);
            for (int i = 0; i < MAXN && ok; ++i)
                if (deg[i] > 0 && !seen[i]) ok = false;
            if (ok) {
                if (M == 0) {
                    for (int i = 0; i < MAXN && ok; ++i)
                        if (deg[i] % 2 != 0) ok = false;
                } else {
                    if (deg[0] % 2 == 0 || deg[M] % 2 == 0) ok = false;
                    for (int i = 0; i < MAXN && ok; ++i) {
                        if (i == 0 || i == M) continue;
                        if (deg[i] % 2 != 0) ok = false;
                    }
                }
            }
        }
        if (ok) printf("YES %d\n", doors);
        else printf("NO\n");
    }
    return 0;
}
