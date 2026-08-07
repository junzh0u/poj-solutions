// POJ 1198 - Solitaire
// Model: claude-sonnet-5
// Approach: bidirectional BFS meeting in the middle (max 8 moves -> 4 each
// side). State = the 4 identical pieces' cells canonicalized by sorting,
// packed into one base-64 integer so a std::map<int,int> can dedupe/hold
// shortest depth. Forward BFS from board 1 and backward BFS from board 2
// use the same move generator, since every move (slide or jump) is its own
// reverse. Any state reached by both sides with dist1+dist2<=8 means YES.
// Statement note: the "board" reused here is one dataset per statement
// text (no explicit multi-case count), read in a while(cin>>...) loop so
// it degrades safely to EOF either way.
#include <cstdio>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

int dr[4] = {-1, 1, 0, 0};
int dc[4] = {0, 0, -1, 1};

bool occupied(const int p[4], int cell) {
    for (int i = 0; i < 4; ++i) if (p[i] == cell) return true;
    return false;
}

int encode(int p[4]) {
    sort(p, p + 4);
    return p[0] + p[1] * 64 + p[2] * 4096 + p[3] * 262144;
}

void decode(int code, int p[4]) {
    p[0] = code % 64; code /= 64;
    p[1] = code % 64; code /= 64;
    p[2] = code % 64; code /= 64;
    p[3] = code;
}

map<int, int> bfs4(int startCode) {
    map<int, int> dist;
    dist[startCode] = 0;
    vector<int> layer;
    layer.push_back(startCode);
    for (int depth = 1; depth <= 4; ++depth) {
        vector<int> next;
        for (size_t li = 0; li < layer.size(); ++li) {
            int pieces[4];
            decode(layer[li], pieces);
            for (int k = 0; k < 4; ++k) {
                int cell = pieces[k];
                int r = cell / 8 + 1, c = cell % 8 + 1;
                for (int d = 0; d < 4; ++d) {
                    int r1 = r + dr[d], c1 = c + dc[d];
                    if (r1 < 1 || r1 > 8 || c1 < 1 || c1 > 8) continue;
                    int cell1 = (r1 - 1) * 8 + (c1 - 1);
                    if (!occupied(pieces, cell1)) {
                        int np[4];
                        for (int t = 0; t < 4; ++t) np[t] = pieces[t];
                        np[k] = cell1;
                        int code = encode(np);
                        if (dist.find(code) == dist.end()) {
                            dist[code] = depth;
                            next.push_back(code);
                        }
                    } else {
                        int r2 = r + 2 * dr[d], c2 = c + 2 * dc[d];
                        if (r2 < 1 || r2 > 8 || c2 < 1 || c2 > 8) continue;
                        int cell2 = (r2 - 1) * 8 + (c2 - 1);
                        if (!occupied(pieces, cell2)) {
                            int np[4];
                            for (int t = 0; t < 4; ++t) np[t] = pieces[t];
                            np[k] = cell2;
                            int code = encode(np);
                            if (dist.find(code) == dist.end()) {
                                dist[code] = depth;
                                next.push_back(code);
                            }
                        }
                    }
                }
            }
        }
        layer = next;
    }
    return dist;
}

bool solve(int a[4], int b[4]) {
    int sa[4], sb[4];
    for (int i = 0; i < 4; ++i) sa[i] = a[i];
    for (int i = 0; i < 4; ++i) sb[i] = b[i];
    int codeA = encode(sa);
    int codeB = encode(sb);
    map<int, int> d1 = bfs4(codeA);
    map<int, int> d2 = bfs4(codeB);
    for (map<int, int>::iterator it = d1.begin(); it != d1.end(); ++it) {
        map<int, int>::iterator jt = d2.find(it->first);
        if (jt != d2.end() && it->second + jt->second <= 8) return true;
    }
    return false;
}

int main() {
    int vals[16];
    while (scanf("%d", &vals[0]) == 1) {
        for (int i = 1; i < 16; ++i) {
            if (scanf("%d", &vals[i]) != 1) return 0;
        }
        int a[4], b[4];
        for (int i = 0; i < 4; ++i) {
            int r = vals[2 * i], c = vals[2 * i + 1];
            a[i] = (r - 1) * 8 + (c - 1);
        }
        for (int i = 0; i < 4; ++i) {
            int r = vals[8 + 2 * i], c = vals[8 + 2 * i + 1];
            b[i] = (r - 1) * 8 + (c - 1);
        }
        printf(solve(a, b) ? "YES\n" : "NO\n");
    }
    return 0;
}
