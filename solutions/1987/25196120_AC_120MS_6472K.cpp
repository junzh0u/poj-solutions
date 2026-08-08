// POJ 1987 - Distance Statistics
// Model: claude-sonnet-5
//
// Same input as "Navigation Nightmare": N farms, M = N-1 roads forming a
// tree (weighted, undirected -- the N/E/S/W direction letter is irrelevant
// here and only read/discarded). Given K, count unordered pairs of distinct
// farms whose tree distance is <= K.
//
// Approach: centroid decomposition. For the component currently being
// processed, find its centroid, BFS distances from the centroid to every
// node in the component, and count pairs (i,j) with dist_i + dist_j <= K
// via sort + two-pointer. That count also includes pairs of nodes that
// share the same immediate branch off the centroid, whose real tree path
// does not pass through the centroid, so it is an overcount; subtract the
// same computation restricted to each branch's own distance list. Then
// remove the centroid and recurse into each remaining branch.
//
// All of centroid finding, BFS distance collection, and the decomposition
// itself are done iteratively (explicit queues / worklist stack) rather
// than by recursion: the discuss board records stack overflows/TLE from
// naive recursive DFS at N up to ~40000, so recursion depth is avoided
// entirely regardless of judge language/compiler stack size.
//
// Edge lengths and path sums can reach ~4*10^10 (up to ~40000 edges of up
// to 1,000,000 each), well past 32-bit range, so all distances/sums use
// long long throughout -- this rules out POJ's language 4 (old MSVC, no
// long long), submitted under G++ instead.

#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;

typedef long long ll;

const int MAXN = 100005;

vector< pair<int,int> > adj[MAXN]; // (neighbor, weight)
bool removedNode[MAXN];
int stampArr[MAXN];
int curStamp = 0;
int parentArr[MAXN];
int sz[MAXN];
ll distArr[MAXN];
int branchArr[MAXN];
int compOrder[MAXN];
int centQueue[MAXN];

int N, M;
ll K;
ll ans = 0;

ll calcPairs(vector<ll>& v) {
    sort(v.begin(), v.end());
    ll cnt = 0;
    int lo = 0, hi = (int)v.size() - 1;
    while (lo < hi) {
        if (v[lo] + v[hi] <= K) {
            cnt += (hi - lo);
            lo++;
        } else {
            hi--;
        }
    }
    return cnt;
}

int main() {
    if (scanf("%d %d", &N, &M) != 2) return 0;
    for (int i = 0; i < M; i++) {
        int a, b, l;
        char d[8];
        scanf("%d %d %d %s", &a, &b, &l, d);
        adj[a].push_back(make_pair(b, l));
        adj[b].push_back(make_pair(a, l));
    }
    long long Kin;
    scanf("%lld", &Kin);
    K = Kin;

    vector<int> worklist;
    worklist.push_back(1);

    while (!worklist.empty()) {
        int start = worklist.back();
        worklist.pop_back();
        if (removedNode[start]) continue;

        // Phase 1: BFS the component containing 'start' (not crossing
        // removed nodes), recording BFS order and parent pointers.
        int qh = 0, qt = 0;
        compOrder[qt++] = start;
        curStamp++;
        stampArr[start] = curStamp;
        parentArr[start] = 0; // sentinel: farms are indexed from 1
        while (qh < qt) {
            int u = compOrder[qh++];
            for (size_t i = 0; i < adj[u].size(); i++) {
                int v = adj[u][i].first;
                if (removedNode[v]) continue;
                if (stampArr[v] == curStamp) continue;
                stampArr[v] = curStamp;
                parentArr[v] = u;
                compOrder[qt++] = v;
            }
        }
        int total = qt;

        // Subtree sizes, processing BFS order in reverse (children first).
        for (int i = 0; i < total; i++) sz[compOrder[i]] = 1;
        for (int i = total - 1; i >= 1; i--) {
            int u = compOrder[i];
            sz[parentArr[u]] += sz[u];
        }

        // Find centroid: minimize the largest branch size.
        int centroid = -1;
        int bestMax = total + 1;
        for (int i = 0; i < total; i++) {
            int u = compOrder[i];
            int maxBranch = total - sz[u]; // upward (parent-side) branch
            for (size_t k = 0; k < adj[u].size(); k++) {
                int v = adj[u][k].first;
                if (removedNode[v]) continue;
                if (stampArr[v] != curStamp) continue;
                if (parentArr[v] != u) continue; // only true BFS-tree children
                if (sz[v] > maxBranch) maxBranch = sz[v];
            }
            if (maxBranch < bestMax) {
                bestMax = maxBranch;
                centroid = u;
            }
        }

        // Phase 2: BFS distances from the centroid over the same component,
        // tagging each node with the id of the centroid's immediate
        // neighbor through which it was reached (its "branch").
        curStamp++;
        int qh2 = 0, qt2 = 0;
        centQueue[qt2++] = centroid;
        stampArr[centroid] = curStamp;
        distArr[centroid] = 0;
        branchArr[centroid] = 0;

        vector<ll> allDist;
        allDist.reserve(total);
        allDist.push_back(0);

        vector< pair<int, ll> > branchDist;
        branchDist.reserve(total);

        while (qh2 < qt2) {
            int u = centQueue[qh2++];
            for (size_t i = 0; i < adj[u].size(); i++) {
                int v = adj[u][i].first;
                int w = adj[u][i].second;
                if (removedNode[v]) continue;
                if (stampArr[v] == curStamp) continue;
                stampArr[v] = curStamp;
                distArr[v] = distArr[u] + w;
                branchArr[v] = (u == centroid) ? v : branchArr[u];
                centQueue[qt2++] = v;
                allDist.push_back(distArr[v]);
                branchDist.push_back(make_pair(branchArr[v], distArr[v]));
            }
        }

        ans += calcPairs(allDist);

        sort(branchDist.begin(), branchDist.end());
        size_t idx = 0;
        while (idx < branchDist.size()) {
            size_t j = idx;
            int bid = branchDist[idx].first;
            vector<ll> group;
            while (j < branchDist.size() && branchDist[j].first == bid) {
                group.push_back(branchDist[j].second);
                j++;
            }
            ans -= calcPairs(group);
            idx = j;
        }

        // Remove centroid; each still-present neighbor starts a new
        // component to process (pushed onto the worklist rather than
        // recursed into).
        removedNode[centroid] = true;
        for (size_t i = 0; i < adj[centroid].size(); i++) {
            int v = adj[centroid][i].first;
            if (!removedNode[v]) worklist.push_back(v);
        }
    }

    printf("%lld\n", ans);
    return 0;
}
