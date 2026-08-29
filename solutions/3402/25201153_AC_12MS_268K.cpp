// POJ 3402 - Add a queen
// Model: claude-sonnet-5
//
// Brute force: try placing the new queen on every vacant cell; for each
// candidate, sweep all K+1 queens in the 8 chess directions, stopping a
// sweep as soon as it hits another piece (queens block line of sight, per
// the discuss board's confirmation that "under attack" means a legal single
// move, which real queens cannot make through an occupied square). Count
// vacant cells left unattacked and keep the candidate maximizing that count
// (equivalently minimizing attacked cells), tie-broken by the smallest
// label under plain STRING lexicographic order (the discuss board's example
// explicitly notes "a12" < "a3" as strings, since '1' < '3').
//
// Board convention: M is the number of rows (lettered a.. from the bottom,
// hence the <=26 bound), N is the number of columns (numbered from 1, left
// to right) -- the usual "M x N" = rows x columns reading.
#include <cstdio>
#include <cstring>
#include <string>
#include <algorithm>
using namespace std;

int M, N, K;
bool occ[26][26]; // occ[row][col]

int dr[8] = {-1,-1,-1,0,0,1,1,1};
int dc[8] = {-1,0,1,-1,1,-1,0,1};

int countNonAttacked(bool board[26][26]) {
    static bool attacked[26][26];
    memset(attacked, 0, sizeof(attacked));
    for (int r = 0; r < M; r++) {
        for (int c = 0; c < N; c++) {
            if (!board[r][c]) continue;
            for (int d = 0; d < 8; d++) {
                int rr = r + dr[d], cc = c + dc[d];
                while (rr >= 0 && rr < M && cc >= 0 && cc < N) {
                    if (board[rr][cc]) break;
                    attacked[rr][cc] = true;
                    rr += dr[d]; cc += dc[d];
                }
            }
        }
    }
    int cnt = 0;
    for (int r = 0; r < M; r++)
        for (int c = 0; c < N; c++)
            if (!board[r][c] && !attacked[r][c]) cnt++;
    return cnt;
}

int main(){
    scanf("%d %d %d", &M, &N, &K);
    for (int i = 0; i < K; i++) {
        char buf[16];
        scanf("%s", buf);
        int row = buf[0] - 'a';
        int col = atoi(buf + 1) - 1;
        occ[row][col] = true;
    }
    int bestCount = -1;
    string bestLabel;
    for (int r = 0; r < M; r++) {
        for (int c = 0; c < N; c++) {
            if (occ[r][c]) continue;
            bool board[26][26];
            memcpy(board, occ, sizeof(occ));
            board[r][c] = true;
            int cnt = countNonAttacked(board);
            char label[16];
            sprintf(label, "%c%d", 'a' + r, c + 1);
            string lbl(label);
            if (cnt > bestCount || (cnt == bestCount && lbl < bestLabel)) {
                bestCount = cnt;
                bestLabel = lbl;
            }
        }
    }
    printf("%s\n%d\n", bestLabel.c_str(), bestCount);
    return 0;
}
