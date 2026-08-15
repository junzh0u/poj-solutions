// POJ 1481 - The Die Is Cast
// Model: claude-sonnet-5
//
// Approach: two independent flood fills.
//  1) Flood-fill over all non-'.' pixels (4-connectivity, edge-adjacency
//     only) to find the maximal connected components -- these are the dice.
//  2) Flood-fill over 'X' pixels only (again 4-connectivity) to find the
//     maximal connected components of dot pixels -- these are the dots.
//     A dot's die is determined by looking up the die id of any one of its
//     cells (dot cells are a subset of exactly one die's cells, since two
//     different dice can never have adjacent pixels of any kind -- if they
//     did they would have been the same connected component in step 1).
// The board discussion (message 164267) warns that sharing a single
// "visited" array between the die-flood and the dot-flood can silently
// give a wrong dot count depending on neighbor-visit order; using two
// separate visited arrays (dieId[][] vs dotVisited[][]) avoids that trap
// entirely, so that is what this solution does.
//
// Output: "Throw n" then the sorted dot counts, then a blank line after
// every test case (including the last), per the statement's explicit
// instruction -- the sample's raw HTML happens not to show a trailing
// blank line only because it is the sole test case immediately followed
// by the closing </pre>.

#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;

const int MAXN = 55;
char grid[MAXN][MAXN];
int dieId[MAXN][MAXN];
bool dotVisited[MAXN][MAXN];
int w, h;

const int dx4[4] = {-1, 1, 0, 0};
const int dy4[4] = {0, 0, -1, 1};

int main() {
    int throwNum = 0;
    while (scanf("%d %d", &w, &h) == 2) {
        if (w == 0 && h == 0) break;
        for (int i = 0; i < h; i++) {
            scanf("%s", grid[i]);
        }
        memset(dieId, -1, sizeof(dieId));
        memset(dotVisited, 0, sizeof(dotVisited));

        // Step 1: flood fill dice (any non-'.' pixel), 4-connected.
        int numDice = 0;
        vector<int> qx, qy;
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                if (grid[i][j] != '.' && dieId[i][j] == -1) {
                    int id = numDice++;
                    qx.clear(); qy.clear();
                    qx.push_back(j); qy.push_back(i);
                    dieId[i][j] = id;
                    size_t head = 0;
                    while (head < qx.size()) {
                        int cx = qx[head], cy = qy[head];
                        head++;
                        for (int d = 0; d < 4; d++) {
                            int nx = cx + dy4[d];
                            int ny = cy + dx4[d];
                            if (nx < 0 || nx >= w || ny < 0 || ny >= h) continue;
                            if (grid[ny][nx] == '.' || dieId[ny][nx] != -1) continue;
                            dieId[ny][nx] = id;
                            qx.push_back(nx); qy.push_back(ny);
                        }
                    }
                }
            }
        }

        // Step 2: flood fill dots (only 'X' pixels), 4-connected.
        vector<int> dotsPerDie(numDice, 0);
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                if (grid[i][j] == 'X' && !dotVisited[i][j]) {
                    int die = dieId[i][j];
                    qx.clear(); qy.clear();
                    qx.push_back(j); qy.push_back(i);
                    dotVisited[i][j] = true;
                    size_t head = 0;
                    while (head < qx.size()) {
                        int cx = qx[head], cy = qy[head];
                        head++;
                        for (int d = 0; d < 4; d++) {
                            int nx = cx + dy4[d];
                            int ny = cy + dx4[d];
                            if (nx < 0 || nx >= w || ny < 0 || ny >= h) continue;
                            if (grid[ny][nx] != 'X' || dotVisited[ny][nx]) continue;
                            dotVisited[ny][nx] = true;
                            qx.push_back(nx); qy.push_back(ny);
                        }
                    }
                    dotsPerDie[die]++;
                }
            }
        }

        sort(dotsPerDie.begin(), dotsPerDie.end());

        throwNum++;
        printf("Throw %d\n", throwNum);
        for (int i = 0; i < numDice; i++) {
            if (i) printf(" ");
            printf("%d", dotsPerDie[i]);
        }
        printf("\n\n");
    }
    return 0;
}
