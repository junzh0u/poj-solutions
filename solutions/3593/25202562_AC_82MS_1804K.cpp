// POJ 3593 - Sea Base Exploration
// Model: claude-sonnet-5
//
// State-compression shortest path. State = (cell, mask) where mask is the
// set of resource kinds already collected (K <= 10, so mask fits in an int).
// Moving to an adjacent free cell costs 1 + sum(B_i for i in mask) (base
// unit cost plus the extra weight of every resource currently carried,
// confirmed against the sample: to-A distance 10 + dig cost 1 + back
// distance 10 * (1+0) = 21). Digging at a cell whose letter i is not yet in
// mask costs A_i and does not consume a move. The ship cell '*' may only be
// entered once mask == full set: the board (message 93283/93284/136840)
// confirms that touching '*' early ends the run automatically, so any path
// that would do so before finishing is simply invalid and is excluded by
// blocking early entry outright.
//
// Since mask only ever grows (dig sets a bit, move keeps it fixed) and any
// submask has a strictly smaller numeric value, masks can be processed in
// increasing numeric order: every predecessor state is already finalized
// once its mask is reached. For a fixed mask, every move edge has the same
// weight wsum[mask], so the min-cost distances to every cell for that mask
// are found with one Dijkstra over just the M*N grid cells (seeded by
// whatever dig transitions from smaller masks already deposited there);
// afterwards, dig edges push improved seeds forward into mask|bit layers.
// This is equivalent to one Dijkstra over the full (cell,mask) product
// graph, but keeps each heap to <= M*N elements instead of one holding
// ~M*N*2^K entries, which matters at the 20x20x2^10 bound this problem's
// own discussion board names as the intended state count.
//
// Input framing: an explicit leading T is honored exactly with a counted
// loop (board message 159469 reports EOF-looping after T gives WA, while a
// counted while(t--) loop is AC).
#include <cstdio>
#include <queue>
#include <vector>
using namespace std;

static char grid[22][22];
static int A[10], B[10];
static int dist[22 * 22 * 1024];
static int wsum[1024];

int main(){
    int T;
    if(scanf("%d", &T) != 1) return 0;
    while(T--){
        int M, N, K, P;
        scanf("%d %d %d %d", &M, &N, &K, &P);
        for(int i = 0; i < M; i++) scanf("%s", grid[i]);
        for(int i = 0; i < K; i++) scanf("%d %d", &A[i], &B[i]);

        int shipR = -1, shipC = -1;
        for(int i = 0; i < M; i++)
            for(int j = 0; j < N; j++)
                if(grid[i][j] == '*') { shipR = i; shipC = j; }

        int fullMask = (1 << K) - 1;
        int cells = M * N;
        int nStates = cells * (fullMask + 1);

        for(int mask = 0; mask <= fullMask; mask++){
            int s = 0;
            for(int b = 0; b < K; b++) if(mask & (1 << b)) s += B[b];
            wsum[mask] = 1 + s;
        }

        const int INF = 0x3f3f3f3f;
        for(int i = 0; i < nStates; i++) dist[i] = INF;

        int startCell = shipR * N + shipC;
        dist[startCell] = 0; // mask 0 layer

        static const int dr[4] = {-1, 1, 0, 0};
        static const int dc[4] = {0, 0, -1, 1};

        typedef pair<int,int> pii;
        priority_queue<pii, vector<pii>, greater<pii> > pq;

        for(int mask = 0; mask <= fullMask; mask++){
            int base = mask * cells;
            int w = wsum[mask];

            while(!pq.empty()) pq.pop();
            for(int cell = 0; cell < cells; cell++)
                if(dist[base + cell] < INF) pq.push(make_pair(dist[base + cell], cell));

            while(!pq.empty()){
                pii top = pq.top(); pq.pop();
                int d = top.first;
                int cell = top.second;
                if(d > dist[base + cell]) continue;
                int r = cell / N, c = cell % N;
                for(int dir = 0; dir < 4; dir++){
                    int nr = r + dr[dir], nc = c + dc[dir];
                    if(nr < 0 || nr >= M || nc < 0 || nc >= N) continue;
                    char nch = grid[nr][nc];
                    if(nch == '#') continue;
                    if(nch == '*' && mask != fullMask) continue;
                    int ncell = nr * N + nc;
                    int nd = d + w;
                    if(nd < dist[base + ncell]){
                        dist[base + ncell] = nd;
                        pq.push(make_pair(nd, ncell));
                    }
                }
            }

            for(int cell = 0; cell < cells; cell++){
                if(dist[base + cell] >= INF) continue;
                int r = cell / N, c = cell % N;
                char ch = grid[r][c];
                if(ch >= 'A' && ch < 'A' + K){
                    int idx = ch - 'A';
                    if(!(mask & (1 << idx))){
                        int nmask = mask | (1 << idx);
                        int nd = dist[base + cell] + A[idx];
                        int nidx = nmask * cells + cell;
                        if(nd < dist[nidx]) dist[nidx] = nd;
                    }
                }
            }
        }

        int answer = dist[fullMask * cells + startCell];
        if(answer >= INF || answer > P) printf("Impossible\n");
        else printf("%d\n", answer);
    }
    return 0;
}
