// POJ 1122 - FDNY to the Rescue!
// Model: claude-sonnet-5
// Approach: Floyd-Warshall with next-hop path reconstruction on the
// directed NxN time matrix (row i, col j = time from intersection i to
// j; -1 means no direct edge). Compute the shortest directed time and
// path from every firehouse intersection to the fire intersection,
// stable-sort the firehouses by that time, and print
// Org/Dest/Time/Path with every field TAB-separated, including the
// header line -- the discuss board reports a Presentation Error comes
// from column-padding with spaces instead of using '\t'
// (message 351497/351498). The firehouse list is read to EOF since it
// may wrap onto more than one physical line (message 94169). If a
// firehouse intersection equals the fire intersection the diagonal is
// forced to distance 0 / path-of-one-node regardless of the input
// matrix's own diagonal value, matching the statement's note 4.
// Statement note 3 explicitly allows printing any one of several tied
// shortest paths, so ties are not a special-judge concern; a disputed
// board thread (message 164284) claiming ties must all be printed was
// rejected by the account this repo submits under (150014) in the same
// thread, and contradicts note 3's own wording.

#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

static const int INF = 1000000000;
static int N;
static int dist_[25][25];
static int nxt[25][25];

int main() {
    if (scanf("%d", &N) != 1) return 0;
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            int t;
            scanf("%d", &t);
            if (i == j) {
                dist_[i][j] = 0;
                nxt[i][j] = i;
            } else if (t < 0) {
                dist_[i][j] = INF;
                nxt[i][j] = -1;
            } else {
                dist_[i][j] = t;
                nxt[i][j] = j;
            }
        }
    }

    int fire;
    scanf("%d", &fire);
    vector<int> stations;
    int x;
    while (scanf("%d", &x) == 1) stations.push_back(x);

    for (int k = 1; k <= N; k++) {
        for (int i = 1; i <= N; i++) {
            if (dist_[i][k] >= INF) continue;
            for (int j = 1; j <= N; j++) {
                if (dist_[k][j] >= INF) continue;
                if (dist_[i][k] + dist_[k][j] < dist_[i][j]) {
                    dist_[i][j] = dist_[i][k] + dist_[k][j];
                    nxt[i][j] = nxt[i][k];
                }
            }
        }
    }

    int m = (int)stations.size();
    vector<pair<int, int> > order(m);
    for (int i = 0; i < m; i++) {
        order[i] = make_pair(dist_[stations[i]][fire], i);
    }
    sort(order.begin(), order.end());

    printf("Org\tDest\tTime\tPath\n");
    for (int t = 0; t < m; t++) {
        int idx = order[t].second;
        int s = stations[idx];
        printf("%d\t%d\t%d", s, fire, dist_[s][fire]);
        int cur = s;
        while (cur != fire) {
            printf("\t%d", cur);
            cur = nxt[cur][fire];
        }
        printf("\t%d\n", fire);
    }
    return 0;
}
