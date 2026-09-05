// POJ 2148 - Color the Map
// Model: claude-sonnet-5
// Approach: build the country adjacency graph by testing, for every pair of
// territory edges belonging to different countries, whether the two segments
// are collinear and their overlap (projected onto the shared line) has
// positive length -- that is the definition of "share a border of non-zero
// length" from the statement; touching at a single point does not count.
// With at most 10 countries, the exact chromatic number is then found with
// a standard subset-DP: precompute which subsets of countries are
// independent sets, then dp[mask] = 1 + min over independent submask s of
// dp[mask ^ s], enumerating submasks in O(3^K).
// Ambiguity: none found in the statement itself; the discuss board's own
// stress case (a 6-territory map with a country split into two disjoint
// unit squares) was used to check that collinear-but-non-overlapping edges
// (touching at a single shared endpoint) are correctly NOT treated as an
// adjacency -- some rejected submissions on record apparently got this
// wrong.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

struct Territory {
    string name;
    vector<int> xs, ys; // vertices in order
};

// cross product of (p1-p0) x (p2-p0)
int cross(int x0,int y0,int x1,int y1,int x2,int y2){
    return (x1-x0)*(y2-y0) - (y1-y0)*(x2-x0);
}

// Check if segments (ax1,ay1)-(ax2,ay2) and (bx1,by1)-(bx2,by2) overlap
// (collinear) with positive length.
bool segsOverlapPositive(int ax1,int ay1,int ax2,int ay2,
                          int bx1,int by1,int bx2,int by2){
    // must be collinear: all four b-points lie on line through a
    if (cross(ax1,ay1,ax2,ay2,bx1,by1) != 0) return false;
    if (cross(ax1,ay1,ax2,ay2,bx2,by2) != 0) return false;
    // direction vector of segment a
    int dx = ax2-ax1, dy = ay2-ay1;
    // project points onto direction using dot product (monotonic along the line)
    int tA1 = 0;
    int tA2 = dx*dx + dy*dy;
    int tB1 = dx*(bx1-ax1) + dy*(by1-ay1);
    int tB2 = dx*(bx2-ax1) + dy*(by2-ay1);
    int aLo = min(tA1,tA2), aHi = max(tA1,tA2);
    int bLo = min(tB1,tB2), bHi = max(tB1,tB2);
    int lo = max(aLo,bLo);
    int hi = min(aHi,bHi);
    return hi > lo; // positive length overlap in parameter space => positive actual length
}

int main(){
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        vector<Territory> terr(n);
        for (int i = 0; i < n; ++i) {
            char buf[64];
            scanf("%s", buf);
            terr[i].name = buf;
            int x;
            while (scanf("%d", &x) == 1) {
                if (x == -1) break;
                int y;
                scanf("%d", &y);
                terr[i].xs.push_back(x);
                terr[i].ys.push_back(y);
            }
        }
        // map country name -> id
        map<string,int> id;
        for (int i = 0; i < n; ++i) {
            if (id.find(terr[i].name) == id.end()) {
                int k = (int)id.size();
                id[terr[i].name] = k;
            }
        }
        int K = (int)id.size();
        vector<vector<bool> > adj(K, vector<bool>(K,false));

        for (int i = 0; i < n; ++i) {
            int ci = id[terr[i].name];
            int mi = (int)terr[i].xs.size();
            for (int j = i+1; j < n; ++j) {
                int cj = id[terr[j].name];
                if (ci == cj) continue;
                if (adj[ci][cj]) continue;
                int mj = (int)terr[j].xs.size();
                bool found = false;
                for (int a = 0; a < mi && !found; ++a) {
                    int a2 = (a+1) % mi;
                    int ax1 = terr[i].xs[a], ay1 = terr[i].ys[a];
                    int ax2 = terr[i].xs[a2], ay2 = terr[i].ys[a2];
                    for (int b = 0; b < mj; ++b) {
                        int b2 = (b+1) % mj;
                        int bx1 = terr[j].xs[b], by1 = terr[j].ys[b];
                        int bx2 = terr[j].xs[b2], by2 = terr[j].ys[b2];
                        if (segsOverlapPositive(ax1,ay1,ax2,ay2,bx1,by1,bx2,by2)) {
                            found = true;
                            break;
                        }
                    }
                }
                if (found) {
                    adj[ci][cj] = adj[cj][ci] = true;
                }
            }
        }

        // exact chromatic number via subset DP
        int full = 1 << K;
        vector<bool> indep(full, true);
        for (int mask = 0; mask < full; ++mask) {
            bool ok = true;
            for (int u = 0; u < K && ok; ++u) {
                if (!(mask & (1<<u))) continue;
                for (int v = u+1; v < K; ++v) {
                    if (!(mask & (1<<v))) continue;
                    if (adj[u][v]) { ok = false; break; }
                }
            }
            indep[mask] = ok;
        }
        vector<int> dp(full, 1000000);
        dp[0] = 0;
        for (int mask = 1; mask < full; ++mask) {
            for (int s = mask; s; s = (s-1) & mask) {
                if (indep[s]) {
                    int cand = dp[mask ^ s] + 1;
                    if (cand < dp[mask]) dp[mask] = cand;
                }
            }
        }
        printf("%d\n", dp[full-1]);
    }
    return 0;
}
