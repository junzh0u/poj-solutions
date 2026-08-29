// POJ 2357 - Labyrinth
// Model: claude-sonnet-5
// Approach: N*N grid of 3x3m cells; '#' cells are solid stone blocks, and a
// border wall surrounds the whole labyrinth except at the two entrance
// corners (0,0) and (N-1,N-1), which are guaranteed to be '.'. A visitor may
// enter through either corner, and (per the discuss board) the two corners
// need not be mutually reachable, so BFS/flood-fill (4-connectivity, which
// naturally respects the "no diagonal peeking/passing between blocks that
// touch at a corner" rule) is run independently from both corners and the
// reachable sets are unioned. Only wall faces adjacent to a reachable open
// cell count as "visible to a visitor"; faces bordering a sealed-off pocket
// of '.' cells (unreachable from either entrance) are excluded, matching the
// sample's isolated 1x2 pocket. Each exposed unit face (one grid edge, 3m
// wide, 3m tall) contributes 9 square meters. Verified this model against
// the official sample (198) by hand before coding.
#include <cstdio>
#include <cstring>
#include <queue>
using namespace std;

int n;
char grid[35][36];
bool reach_[35][35];

int main(){
    if(scanf("%d", &n) != 1) return 0;
    for(int i = 0; i < n; ++i){
        scanf("%s", grid[i]);
    }
    memset(reach_, 0, sizeof(reach_));
    int dr[4] = {-1,1,0,0};
    int dc[4] = {0,0,-1,1};

    int starts[2][2] = {{0,0},{n-1,n-1}};
    queue<pair<int,int> > q;
    for(int s = 0; s < 2; ++s){
        int r = starts[s][0], c = starts[s][1];
        if(!reach_[r][c] && grid[r][c] == '.'){
            reach_[r][c] = true;
            q.push(make_pair(r,c));
        }
    }
    while(!q.empty()){
        pair<int,int> cur = q.front(); q.pop();
        int r = cur.first, c = cur.second;
        for(int d = 0; d < 4; ++d){
            int nr = r + dr[d], nc = c + dc[d];
            if(nr < 0 || nr >= n || nc < 0 || nc >= n) continue;
            if(grid[nr][nc] != '.') continue;
            if(reach_[nr][nc]) continue;
            reach_[nr][nc] = true;
            q.push(make_pair(nr,nc));
        }
    }

    int units = 0;
    for(int r = 0; r < n; ++r){
        for(int c = 0; c < n; ++c){
            if(grid[r][c] == '#'){
                for(int d = 0; d < 4; ++d){
                    int nr = r + dr[d], nc = c + dc[d];
                    if(nr < 0 || nr >= n || nc < 0 || nc >= n) continue;
                    if(grid[nr][nc] == '.' && reach_[nr][nc]) units++;
                }
            } else {
                // '.' cell
                if(!reach_[r][c]) continue;
                bool topEntrance = (r == 0 && c == 0);
                bool botEntrance = (r == n-1 && c == n-1);
                if(r == 0 && !topEntrance) units++;           // top border
                if(c == 0 && !topEntrance) units++;           // left border
                if(r == n-1 && !botEntrance) units++;         // bottom border
                if(c == n-1 && !botEntrance) units++;         // right border
            }
        }
    }

    printf("%d\n", units * 9);
    return 0;
}
