// POJ 2808 - Artificial Strife
// Model: claude-sonnet-5
//
// Multiple Conway-like species share one grid. Each species runs its own
// Game of Life independently (only its own current cells count as live
// neighbours / as the cell's own state for its ruleset), producing a
// candidate "wants to be alive here" grid per species per turn. Conflicts
// at a cell are resolved by picking the alphabetically-first species that
// wants the cell (this includes the "only one wants it" case, since that
// is just the first-and-only candidate). Track max/min population per
// species over turns 0..T inclusive.
//
// No ambiguity in the statement; the board discussion only notes T's
// bound isn't given but is small in practice (observed <=~30), which a
// direct O(X*Y*(8+S)) per turn simulation handles trivially regardless.

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
using namespace std;

int main(){
    int n;
    if(scanf("%d", &n) != 1) return 0;
    for(int tc = 1; tc <= n; tc++){
        int X, Y, S;
        scanf("%d %d %d", &X, &Y, &S);
        vector<string> grid(Y);
        for(int r = 0; r < Y; r++){
            char buf[64];
            scanf("%s", buf);
            grid[r] = buf;
        }
        bool survive[26][9];
        bool birth[26][9];
        memset(survive, 0, sizeof(survive));
        memset(birth, 0, sizeof(birth));
        for(int s = 0; s < S; s++){
            char buf[40];
            scanf("%s", buf);
            string str(buf);
            size_t pos = str.find('/');
            string sv = (pos == string::npos) ? str : str.substr(0, pos);
            string bt = (pos == string::npos) ? "" : str.substr(pos + 1);
            for(size_t i = 0; i < sv.size(); i++) survive[s][sv[i] - '0'] = true;
            for(size_t i = 0; i < bt.size(); i++) birth[s][bt[i] - '0'] = true;
        }
        int T;
        scanf("%d", &T);

        vector<vector<int> > owner(Y, vector<int>(X, -1));
        for(int r = 0; r < Y; r++)
            for(int c = 0; c < X; c++)
                owner[r][c] = (grid[r][c] == '.') ? -1 : (grid[r][c] - 'A');

        vector<int> maxPop(S, 0), minPop(S, 1 << 30);
        {
            vector<int> cnt(S, 0);
            for(int r = 0; r < Y; r++)
                for(int c = 0; c < X; c++)
                    if(owner[r][c] >= 0) cnt[owner[r][c]]++;
            for(int s = 0; s < S; s++){ maxPop[s] = cnt[s]; minPop[s] = cnt[s]; }
        }

        for(int t = 0; t < T; t++){
            vector<vector<int> > newOwner(Y, vector<int>(X, -1));
            for(int r = 0; r < Y; r++){
                for(int c = 0; c < X; c++){
                    int cnt[26];
                    memset(cnt, 0, sizeof(int) * S);
                    for(int dr = -1; dr <= 1; dr++){
                        for(int dc = -1; dc <= 1; dc++){
                            if(dr == 0 && dc == 0) continue;
                            int nr = r + dr, nc = c + dc;
                            if(nr < 0 || nr >= Y || nc < 0 || nc >= X) continue;
                            int ow = owner[nr][nc];
                            if(ow >= 0) cnt[ow]++;
                        }
                    }
                    int cur = owner[r][c];
                    int winner = -1;
                    for(int s = 0; s < S; s++){
                        bool alive = (cur == s) ? survive[s][cnt[s]] : birth[s][cnt[s]];
                        if(alive){ winner = s; break; }
                    }
                    newOwner[r][c] = winner;
                }
            }
            owner = newOwner;
            vector<int> cnt(S, 0);
            for(int r = 0; r < Y; r++)
                for(int c = 0; c < X; c++)
                    if(owner[r][c] >= 0) cnt[owner[r][c]]++;
            for(int s = 0; s < S; s++){
                if(cnt[s] > maxPop[s]) maxPop[s] = cnt[s];
                if(cnt[s] < minPop[s]) minPop[s] = cnt[s];
            }
        }

        printf("Simulation #%d\n", tc);
        for(int s = 0; s < S; s++){
            printf("Species %c: At most %d live, at least %d live.\n", 'A' + s, maxPop[s], minPop[s]);
        }
    }
    return 0;
}
