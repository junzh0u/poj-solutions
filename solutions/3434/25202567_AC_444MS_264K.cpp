// POJ 3434 - Terrarium
// Model: claude-sonnet-5
//
// Each snake slides forward 1 cell/second: it tries straight ahead (relative
// to its own current heading), then a right turn, then a left turn; if all
// three target cells are obstacles it waits. Snakes move in alphabetical
// order every second and the board is updated immediately after each snake's
// move, so later snakes in the same second see earlier snakes' new positions.
//
// A snake's body is a fixed-length path; moving it is: add a new head cell,
// drop the old tail cell (the middle segments never actually move). This is
// implemented as a per-snake circular buffer of length L: storing the path
// from head (index head_idx) going backward through the buffer to the tail
// (index head_idx+1, mod L, i.e. head_idx-1 mod L). Advancing simply
// overwrites the tail slot with the new head cell and moves head_idx to that
// slot -- O(1) regardless of snake length, which matters since T can be 1e6
// and a snake can be almost the whole N*N board (the board's discuss thread
// warns that std::list/std::deque based sliding is too slow).
//
// Obstacle = out of bounds, a wall, another snake's body, OR this snake's own
// body -- except the cell equal to its own current tail, which is vacated by
// this exact move and so is free for the new head to occupy. Self-collision
// must block: since only the tail is removed and every other segment stays
// put, moving the head onto a non-tail cell of the same snake would require
// that cell to be occupied by two different path positions simultaneously,
// which is impossible; the statement's "wall or another snake" phrasing is
// just the common case. Confirmed against the sample: sample 1's snake A
// ends as a C-shape whose two open ends are geometrically adjacent (an
// adjacency that is NOT a path edge), which only makes sense if you track
// occupancy by exact cell identity rather than by re-deriving path shape from
// spatial adjacency in the output.
//
// "Right" and "left" turns are relative to the snake's own heading. With row
// increasing downward and column increasing rightward, a clockwise (right)
// turn maps heading (dr,dc) -> (dc,-dr), and counter-clockwise (left) maps
// (dr,dc) -> (-dc,dr). Verified against the board's worked example
// (".aA./..#B/..#b/...."): snake A (heading right) moves straight into the
// empty cell to its right; snake B (heading up) is blocked straight (A just
// took that cell), blocked turning either way (off-board one way, a wall the
// other), so it waits for a second -- exactly what the thread's discussion
// concludes ("A moves, then B can't move").
//
// The discuss board also flags that some judge inputs have a stray space
// after the second integer; scanf("%s", ...) for each grid row skips all
// whitespace (spaces and newlines alike) so this never affects row parsing.
//
// No statement ambiguity beyond the above; verified against both provided
// samples exactly, byte for byte.

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
using namespace std;

int main(){
    int N, T;
    if (scanf("%d %d", &N, &T) != 2) return 0;
    vector<string> grid(N);
    {
        vector<char> buf(N + 2);
        for (int i = 0; i < N; i++) {
            scanf("%s", &buf[0]);
            grid[i] = &buf[0];
        }
    }

    vector<int> owner(N * N, 0); // 0 empty, -1 wall, s+1 snake id (1..26)
    bool exists[26];
    memset(exists, 0, sizeof(exists));

    for (int r = 0; r < N; r++)
        for (int c = 0; c < N; c++)
            if (grid[r][c] == '#') owner[r * N + c] = -1;

    vector<int> pathR[26], pathC[26];
    const int dr4[4] = {-1, 1, 0, 0};
    const int dc4[4] = {0, 0, -1, 1};

    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            char ch = grid[r][c];
            if (ch < 'A' || ch > 'Z') continue;
            int s = ch - 'A';
            exists[s] = true;
            int pr = -1, pc = -1;
            int cr = r, cc = c;
            pathR[s].push_back(cr);
            pathC[s].push_back(cc);
            int guard = N * N + 5;
            while (guard-- > 0) {
                int nr = -1, nc = -1;
                bool found = false;
                for (int k = 0; k < 4; k++) {
                    int tr = cr + dr4[k], tc = cc + dc4[k];
                    if (tr < 0 || tr >= N || tc < 0 || tc >= N) continue;
                    if (tr == pr && tc == pc) continue;
                    char tch = grid[tr][tc];
                    bool same;
                    if (tch >= 'A' && tch <= 'Z') same = (tch - 'A') == s;
                    else if (tch >= 'a' && tch <= 'z') same = (tch - 'a') == s;
                    else same = false;
                    if (same) { nr = tr; nc = tc; found = true; break; }
                }
                if (!found) break;
                pathR[s].push_back(nr);
                pathC[s].push_back(nc);
                pr = cr; pc = cc; cr = nr; cc = nc;
            }
        }
    }

    int offset[26], length[26];
    int total = 0;
    for (int s = 0; s < 26; s++) {
        offset[s] = total;
        length[s] = exists[s] ? (int)pathR[s].size() : 0;
        total += length[s];
    }

    vector<int> bufR(total), bufC(total);
    int headIdx[26];
    for (int s = 0; s < 26; s++) {
        if (!exists[s]) continue;
        int L = length[s];
        for (int k = 0; k < L; k++) {
            bufR[offset[s] + k] = pathR[s][k];
            bufC[offset[s] + k] = pathC[s][k];
            owner[pathR[s][k] * N + pathC[s][k]] = s + 1;
        }
        headIdx[s] = 0;
    }

    for (int t = 0; t < T; t++) {
        for (int s = 0; s < 26; s++) {
            if (!exists[s]) continue;
            int L = length[s];
            if (L < 2) continue;
            int off = offset[s];
            int hIdx = headIdx[s];
            int secIdx = hIdx + 1; if (secIdx >= L) secIdx -= L;
            int tailIdx = hIdx - 1; if (tailIdx < 0) tailIdx += L;

            int hr = bufR[off + hIdx], hc = bufC[off + hIdx];
            int sr = bufR[off + secIdx], sc = bufC[off + secIdx];
            int ddr = hr - sr, ddc = hc - sc;

            int candR[3], candC[3];
            candR[0] = hr + ddr; candC[0] = hc + ddc;       // straight
            candR[1] = hr + ddc; candC[1] = hc - ddr;       // right (clockwise)
            candR[2] = hr - ddc; candC[2] = hc + ddr;       // left (counter-clockwise)

            int tailR = bufR[off + tailIdx], tailC = bufC[off + tailIdx];
            int tailCell = tailR * N + tailC;

            for (int k = 0; k < 3; k++) {
                int nr = candR[k], nc = candC[k];
                if (nr < 0 || nr >= N || nc < 0 || nc >= N) continue;
                int ncell = nr * N + nc;
                int ow = owner[ncell];
                bool freeCell = (ow == 0) || (ow == s + 1 && ncell == tailCell);
                if (freeCell) {
                    owner[tailCell] = 0;
                    owner[ncell] = s + 1;
                    bufR[off + tailIdx] = nr;
                    bufC[off + tailIdx] = nc;
                    headIdx[s] = tailIdx;
                    break;
                }
            }
        }
    }

    int headCellFlat[26];
    for (int s = 0; s < 26; s++) {
        if (!exists[s]) { headCellFlat[s] = -1; continue; }
        int off = offset[s], hIdx = headIdx[s];
        headCellFlat[s] = bufR[off + hIdx] * N + bufC[off + hIdx];
    }

    string outBuf;
    outBuf.reserve((size_t)(N + 1) * N);
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            int cell = r * N + c;
            int ow = owner[cell];
            char ch;
            if (ow == -1) ch = '#';
            else if (ow == 0) ch = '.';
            else {
                int s = ow - 1;
                ch = (cell == headCellFlat[s]) ? (char)('A' + s) : (char)('a' + s);
            }
            outBuf.push_back(ch);
        }
        outBuf.push_back('\n');
    }
    fwrite(outBuf.data(), 1, outBuf.size(), stdout);
    return 0;
}
