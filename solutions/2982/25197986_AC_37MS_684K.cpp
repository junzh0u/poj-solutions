// POJ 2982 - Time Travel
// Model: claude-sonnet-5
// Dijkstra over grid states (t,p) with 0<=t<=N, 0<=p<=M. Each of the K services
// (a,b,c) is a reusable directed edge (t,p)->(t+a,p+b) costing c, valid only
// while the endpoint stays within [0,N]x[0,M]. Per the statement, time can only
// move forward (a>=0, confirmed on the discuss board) while position may move
// either way (b can be negative); leaving [0,M] means leaving the finite-width
// universe permanently, so such moves are simply illegal (board thread
// confirms position is bounded to [0,M], not infinite). a=0 pure-position moves
// are legal, and edges may be reused any number of times (also confirmed on the
// board). Since a can be 0 and b can be negative, a naive row/column order DP
// is wrong (board explicitly warns about this); Dijkstra over the whole grid
// works because all costs are positive.
#include <cstdio>
#include <queue>
#include <vector>
using namespace std;

const int MAXD = 105;
const long long INF = 1LL<<60;

int N, M, K;
int A[105], B[105], C[105];
long long dist_[MAXD][MAXD];
bool done_[MAXD][MAXD];

struct State {
    long long d;
    int x, y;
};
struct Cmp {
    bool operator()(const State& a, const State& b) const { return a.d > b.d; }
};

int main(){
    while (scanf("%d %d %d", &N, &M, &K) == 3) {
        if (N==0 && M==0 && K==0) break;
        for (int i=0;i<K;i++) scanf("%d %d %d", &A[i], &B[i], &C[i]);
        for (int x=0;x<=N;x++)
            for (int y=0;y<=M;y++) { dist_[x][y]=INF; done_[x][y]=false; }
        dist_[0][0]=0;
        priority_queue<State, vector<State>, Cmp> pq;
        {
            State start;
            start.d = 0; start.x = 0; start.y = 0;
            pq.push(start);
        }
        while (!pq.empty()) {
            State cur = pq.top(); pq.pop();
            if (done_[cur.x][cur.y]) continue;
            done_[cur.x][cur.y] = true;
            if (cur.x==N && cur.y==M) break;
            for (int i=0;i<K;i++) {
                int nx = cur.x + A[i], ny = cur.y + B[i];
                if (nx < 0 || nx > N || ny < 0 || ny > M) continue;
                if (done_[nx][ny]) continue;
                long long nd = cur.d + C[i];
                if (nd < dist_[nx][ny]) {
                    dist_[nx][ny] = nd;
                    State nxt;
                    nxt.d = nd; nxt.x = nx; nxt.y = ny;
                    pq.push(nxt);
                }
            }
        }
        if (dist_[N][M] >= INF) printf("-1\n");
        else printf("%lld\n", dist_[N][M]);
    }
    return 0;
}
