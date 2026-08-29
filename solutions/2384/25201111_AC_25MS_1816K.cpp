// POJ 2384 - Harder Sokoban Problem
// Model: claude-sonnet-5
//
// Approach: Choose starting cells for the player and the container to
// MAXIMIZE the number of moves an optimally-playing player needs to push
// the container onto the destination cell.
//
// For a fixed target, compute, for every state (box position B, player
// position P), the minimum number of moves needed to bring the box to the
// destination D. Do this with a single multi-source BFS seeded at every
// state (D, p) for every non-wall cell p != D (distance 0, since the box is
// already at the destination regardless of where the player stands).
//
// The BFS explores the REVERSE of the normal Sokoban move relation, so
// that one BFS from the goal states fills in the answer for every possible
// start simultaneously:
//   - Walk move (forward): (B,P) -> (B,P2) for P2 adjacent to P, P2 not a
//     wall, P2 != B. This relation is symmetric, so it is also its own
//     reverse: from (B,P) we can go to (B,P2) directly.
//   - Push move (forward): from (Bf,Pf) with Pf adjacent to Bf (direction
//     d = Bf - Pf), pushing requires Bn = Bf + d empty; result is
//     (Bn, Bf). So its reverse, from a dequeued state (B,P) (which plays
//     the role of (Bn,Pn) = (Bn,Bf), i.e. Bf = P), recovers the
//     predecessor state (Bf,Pf) = (P, 2P-B), valid when B and P are
//     orthogonally adjacent and 2P-B is a non-wall in-bounds cell.
//
// The answer is the maximum finite BFS distance dist[B][P] over all valid
// (B,P) with B != wall, P != wall, P != B.
//
// Ambiguity check: the discuss board (message 159108) posts a Pascal
// solution using exactly this reverse-BFS idea, and explicitly notes that
// states with the box already at the destination must be forced to
// distance 0 for every player position, not just the ones the BFS walks
// into first from an adjacent seed -- confirming this seeding approach.
//
// Complexity: O(N^4) states/edges, N <= 25 => <= 390625 states, trivial
// for the 1s limit.

#include <cstdio>
#include <cstring>
#include <queue>
#include <algorithm>
using namespace std;

int N;
char grid[26][26];
int dist_[625*625]; // index: B*NN+P

int main(){
    if (scanf("%d", &N) != 1) return 0;
    for (int i = 0; i < N; i++){
        scanf("%s", grid[i]);
    }
    int NN = N*N;
    int D = -1;
    bool wall[625];
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            int idx = i*N+j;
            wall[idx] = (grid[i][j] == '#');
            if (grid[i][j] == '*') D = idx;
        }
    }
    int total = NN*NN;
    for (int i = 0; i < total; i++) dist_[i] = -1;

    queue<int> q;
    for (int p = 0; p < NN; p++){
        if (wall[p]) continue;
        if (p == D) continue;
        int code = D*NN + p;
        dist_[code] = 0;
        q.push(code);
    }

    static const int dr[4] = {-1,1,0,0};
    static const int dc[4] = {0,0,-1,1};

    while (!q.empty()){
        int code = q.front(); q.pop();
        int d = dist_[code];
        int B = code / NN;
        int P = code % NN;
        int br = B / N, bc = B % N;
        int pr = P / N, pc = P % N;

        // walk moves (symmetric relation)
        for (int k = 0; k < 4; k++){
            int nr = pr + dr[k], nc = pc + dc[k];
            if (nr < 0 || nr >= N || nc < 0 || nc >= N) continue;
            int np = nr*N+nc;
            if (wall[np]) continue;
            if (np == B) continue;
            int ncode = B*NN + np;
            if (dist_[ncode] == -1){
                dist_[ncode] = d+1;
                q.push(ncode);
            }
        }

        // push predecessor: requires P adjacent to B
        int ddr = br - pr, ddc = bc - pc;
        if ((ddr == 0 && (ddc == 1 || ddc == -1)) || (ddc == 0 && (ddr == 1 || ddr == -1))){
            int pfr = 2*pr - br;
            int pfc = 2*pc - bc;
            if (pfr >= 0 && pfr < N && pfc >= 0 && pfc < N){
                int Pf = pfr*N+pfc;
                if (!wall[Pf]){
                    int B0 = P;   // new box position
                    int P0 = Pf;  // new player position
                    if (P0 != B0){
                        int ncode = B0*NN + P0;
                        if (dist_[ncode] == -1){
                            dist_[ncode] = d+1;
                            q.push(ncode);
                        }
                    }
                }
            }
        }
    }

    int ans = 0;
    for (int B = 0; B < NN; B++){
        if (wall[B]) continue;
        for (int P = 0; P < NN; P++){
            if (wall[P] || P == B) continue;
            int v = dist_[B*NN+P];
            if (v > ans) ans = v;
        }
    }
    printf("%d\n", ans);
    return 0;
}
