// POJ 2222 - Deeper Blue
// Model: claude-sonnet-5
// Approach: for each dataset, collect the <=15 non-empty squares as pieces.
// Build a conflict graph on pieces: two pieces conflict if one attacks the
// other's square according to its movement rule (King/Queen/Rook/Bishop/
// Knight), using unobstructed geometric lines (no blocking by intervening
// pieces -- confirmed by the discuss board, which describes a plain
// O(2^15) brute force over subsets with no mention of line-of-sight
// blocking, and matches both sample outputs). The answer is pieces minus
// the maximum independent set of the conflict graph, found by enumerating
// all 2^m subsets (m <= 15) and keeping the largest conflict-free one.
#include <cstdio>
#include <cstring>
#include <cstdlib>
using namespace std;

int main(){
    char line[64];
    while (scanf("%s", line) == 1) {
        // line should be START
        int w, h;
        scanf("%d %d", &w, &h);
        int px[20], py[20];
        char pt[20];
        int m = 0;
        char tok[8];
        for (int r = 0; r < h; r++) {
            for (int c = 0; c < w; c++) {
                scanf("%s", tok);
                if (tok[0] != 'E') {
                    px[m] = c;
                    py[m] = r;
                    pt[m] = tok[0];
                    m++;
                }
            }
        }
        scanf("%s", line); // END

        int adj[20];
        memset(adj, 0, sizeof(adj));
        for (int i = 0; i < m; i++) {
            for (int j = i+1; j < m; j++) {
                int dx = px[i]-px[j];
                int dy = py[i]-py[j];
                int adx = dx<0?-dx:dx;
                int ady = dy<0?-dy:dy;
                bool conflict = false;
                char a = pt[i], b = pt[j];
                // King
                if ((a=='K'||b=='K') && adx<=1 && ady<=1) conflict = true;
                // Queen
                if ((a=='Q'||b=='Q') && (dx==0 || dy==0 || adx==ady)) conflict = true;
                // Rook
                if ((a=='R'||b=='R') && (dx==0 || dy==0)) conflict = true;
                // Bishop
                if ((a=='B'||b=='B') && (adx==ady)) conflict = true;
                // Knight
                if ((a=='N'||b=='N') && ((adx==1&&ady==2)||(adx==2&&ady==1))) conflict = true;
                if (conflict) {
                    adj[i] |= (1<<j);
                    adj[j] |= (1<<i);
                }
            }
        }

        int best = 0;
        int total = 1<<m;
        for (int mask = 0; mask < total; mask++) {
            // check independent
            bool ok = true;
            for (int i = 0; i < m && ok; i++) {
                if (mask & (1<<i)) {
                    if (adj[i] & mask) ok = false;
                }
            }
            if (ok) {
                int cnt = __builtin_popcount(mask);
                if (cnt > best) best = cnt;
            }
        }

        printf("Minimum Number of Pieces to be removed: %d\n", m - best);
    }
    return 0;
}
