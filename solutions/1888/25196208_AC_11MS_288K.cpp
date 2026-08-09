// POJ 1888 - Crossword Answers
// Model: claude-sonnet-5
// Approach: simulate the numbering rule directly. A white square is
// "eligible" (gets a number) iff it starts an across word (no white
// square immediately to its left) or starts a down word (no white
// square immediately above it) -- these are exactly the two conditions
// in the statement, joined by OR. Numbers are assigned in row-major
// order over eligible squares only. Because both the numbering pass and
// the across/down extraction pass walk the grid in the same row-major
// order, collecting across-starts (in order) and down-starts (in order)
// during a single pass already yields each list sorted by its word's
// number, with no separate sort needed.
// Output format confirmed against the sample (byte-exact, not just
// visually): each entry is printed as "%3d.%s" (number right-justified
// in a 3-char field, no space before the word), and a single blank line
// separates consecutive puzzles' output blocks -- but there is no
// trailing blank line after the last puzzle in the sample, so puzzles
// are joined with a blank-line separator rather than each one being
// followed by a blank line unconditionally.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
using namespace std;

int main() {
    int r, c;
    int puzzle = 0;
    char grid[12][12];
    int num[12][12];
    bool first = true;
    while (scanf("%d", &r) == 1 && r != 0) {
        scanf("%d", &c);
        for (int i = 0; i < r; i++) {
            scanf("%s", grid[i]);
        }
        // assign numbers
        int counter = 1;
        for (int i = 0; i < r; i++) {
            for (int j = 0; j < c; j++) {
                bool white = grid[i][j] != '*';
                if (!white) { num[i][j] = 0; continue; }
                bool leftBlack = (j == 0) || (grid[i][j-1] == '*');
                bool aboveBlack = (i == 0) || (grid[i-1][j] == '*');
                if (leftBlack || aboveBlack) num[i][j] = counter++;
                else num[i][j] = 0;
            }
        }

        vector<pair<int, string> > across, down;
        for (int i = 0; i < r; i++) {
            for (int j = 0; j < c; j++) {
                if (grid[i][j] == '*') continue;
                bool leftBlack = (j == 0) || (grid[i][j-1] == '*');
                bool aboveBlack = (i == 0) || (grid[i-1][j] == '*');
                if (leftBlack) {
                    string w;
                    int jj = j;
                    while (jj < c && grid[i][jj] != '*') { w += grid[i][jj]; jj++; }
                    across.push_back(make_pair(num[i][j], w));
                }
                if (aboveBlack) {
                    string w;
                    int ii = i;
                    while (ii < r && grid[ii][j] != '*') { w += grid[ii][j]; ii++; }
                    down.push_back(make_pair(num[i][j], w));
                }
            }
        }

        puzzle++;
        if (!first) printf("\n");
        first = false;
        printf("puzzle #%d:\n", puzzle);
        printf("Across\n");
        for (size_t k = 0; k < across.size(); k++) {
            printf("%3d.%s\n", across[k].first, across[k].second.c_str());
        }
        printf("Down\n");
        for (size_t k = 0; k < down.size(); k++) {
            printf("%3d.%s\n", down[k].first, down[k].second.c_str());
        }
    }
    return 0;
}
