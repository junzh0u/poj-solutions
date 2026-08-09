// POJ 1501 - Word-Search Wonder
// Model: claude-sonnet-5
// Approach: brute-force search over all 8 directions (N,S,E,W and the 4
// diagonals) from every starting cell, for each query word. The statement's
// prose literally says only horizontal and diagonal lines "may proceed
// right to left (backwards)", implying vertical is top-to-bottom only, but
// the discuss board (message 29594, 89188, 144771) confirms the judge data
// actually requires all 8 directions, including bottom-to-top vertical and
// both diagonal orientations reversed. The input guarantees each word
// occurs at most once, so the first match found is reported.
#include <cstdio>
#include <cstring>

static char grid[105][105];
static char word[105];
static int L;

int main() {
    if (scanf("%d", &L) != 1) return 0;
    for (int i = 0; i < L; ++i) {
        scanf("%s", grid[i]);
    }

    static const int dr[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    static const int dc[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

    while (scanf("%s", word) == 1) {
        if (word[0] == '0' && word[1] == '\0') break;
        int wl = (int)strlen(word);
        int fr = -1, fc = -1, lr = -1, lc = -1;
        bool found = false;
        for (int r = 0; r < L && !found; ++r) {
            for (int c = 0; c < L && !found; ++c) {
                if (grid[r][c] != word[0]) continue;
                for (int d = 0; d < 8; ++d) {
                    int endr = r + dr[d] * (wl - 1);
                    int endc = c + dc[d] * (wl - 1);
                    if (endr < 0 || endr >= L || endc < 0 || endc >= L) continue;
                    bool ok = true;
                    for (int k = 1; k < wl; ++k) {
                        if (grid[r + dr[d] * k][c + dc[d] * k] != word[k]) {
                            ok = false;
                            break;
                        }
                    }
                    if (ok) {
                        fr = r; fc = c; lr = endr; lc = endc;
                        found = true;
                        break;
                    }
                }
            }
        }
        if (found) {
            printf("%d,%d %d,%d\n", fr + 1, fc + 1, lr + 1, lc + 1);
        } else {
            printf("Not found\n");
        }
    }
    return 0;
}
