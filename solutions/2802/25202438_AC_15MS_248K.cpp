// POJ 2802 - Plinko
// Model: claude-sonnet-5
//
// The board is a fixed 5-column, 4-peg-row Plinko layout. Track a chip's
// horizontal position as an integer in [0,8]: columns A..E sit at the even
// positions 0,2,4,6,8, and the four peg rows alternate between "aligned"
// pegs (5 pegs, directly under the column slots, at the even positions)
// and "offset" pegs (4 pegs, between columns, at the odd positions 1..7).
// A chip starts aligned at 2*startColumn and, after bouncing off all four
// peg rows, ends aligned again at 2*endColumn.
//
// For each peg we're given right-probability num/den. At an aligned peg,
// num==0 or num==den forces a single direction (the board-edge pegs A and
// E in row 1/3 always have this form); otherwise both directions are
// live. Path count and probability are propagated forward from the start
// position level by level (DP over the 9 possible positions), tracking
// both the number of distinct paths reaching a position (an edge is only
// "possible" when its probability is strictly nonzero) and the exact
// probability mass there. Denominators are single digits and the DP is
// only 4 levels deep, so plain double arithmetic carries no meaningful
// error; percentages are truncated with a tiny epsilon guard against
// floating-point representation noise before flooring.
//
// Ambiguity: the statement's diagrams are the only place the geometry
// (which pegs are aligned vs offset, edge pegs forced) is specified;
// verified by hand-simulating both sample boards against the given
// sample output before trusting the implementation.
#include <cstdio>
#include <cstring>

struct Frac { int num, den; };

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    for (int tc = 1; tc <= n; tc++) {
        Frac row1[5], row2[4], row3[5], row4[4];
        for (int i = 0; i < 5; i++) scanf("%d/%d", &row1[i].num, &row1[i].den);
        for (int i = 0; i < 4; i++) scanf("%d/%d", &row2[i].num, &row2[i].den);
        for (int i = 0; i < 5; i++) scanf("%d/%d", &row3[i].num, &row3[i].den);
        for (int i = 0; i < 4; i++) scanf("%d/%d", &row4[i].num, &row4[i].den);

        printf("data set #%d\n", tc);

        for (int q = 0; q < 3; q++) {
            char sc[8], ec[8];
            scanf("%s %s", sc, ec);
            int startCol = sc[0] - 'A';
            int endCol = ec[0] - 'A';

            int cnt[9];
            double prob[9];
            memset(cnt, 0, sizeof(cnt));
            memset(prob, 0, sizeof(prob));
            int startPos = 2 * startCol;
            cnt[startPos] = 1;
            prob[startPos] = 1.0;

            // level 0 (aligned) -> level 1 (offset), using row1
            // level 1 (offset)  -> level 2 (aligned), using row2
            // level 2 (aligned) -> level 3 (offset),  using row3
            // level 3 (offset)  -> level 4 (aligned), using row4
            Frac *rows[4] = {row1, row2, row3, row4};
            bool alignedSource[4] = {true, false, true, false};

            for (int lvl = 0; lvl < 4; lvl++) {
                int ncnt[9];
                double nprob[9];
                memset(ncnt, 0, sizeof(ncnt));
                memset(nprob, 0, sizeof(nprob));
                for (int p = 0; p <= 8; p++) {
                    if (cnt[p] == 0 && prob[p] == 0.0) continue;
                    int idx = alignedSource[lvl] ? (p / 2) : ((p - 1) / 2);
                    int num = rows[lvl][idx].num;
                    int den = rows[lvl][idx].den;
                    bool leftPossible = (num < den);
                    bool rightPossible = (num > 0);
                    double pr = (double)num / den;
                    double pl = 1.0 - pr;
                    if (leftPossible) {
                        ncnt[p - 1] += cnt[p];
                        nprob[p - 1] += prob[p] * pl;
                    }
                    if (rightPossible) {
                        ncnt[p + 1] += cnt[p];
                        nprob[p + 1] += prob[p] * pr;
                    }
                }
                memcpy(cnt, ncnt, sizeof(cnt));
                memcpy(prob, nprob, sizeof(prob));
            }

            int finalPos = 2 * endCol;
            int paths = cnt[finalPos];
            double chance = prob[finalPos];
            int pct = (int)(chance * 100.0 + 1e-9);
            printf("%c->%c %d paths, %d%% chance\n", sc[0], ec[0], paths, pct);
        }
    }
    return 0;
}
