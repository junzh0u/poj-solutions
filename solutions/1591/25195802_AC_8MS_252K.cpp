// POJ 1591 - M*A*S*H
// Model: claude-opus-5
//
// Straight simulation of a *linear* (non-circular) Josephus elimination.
// The line holds the survivors in original order.  A card of value k drives one
// pass: walk from the head, counting 1..k over the people still in line; every
// time the count reaches k that person leaves and the count restarts at 1 with
// the next person.  The pass ends the moment the walk falls off the end of the
// line (whatever the partial count was) -- then the next card is drawn and the
// walk restarts at the head of the remaining line.  Stop as soon as exactly X
// people remain; print their original positions in line order.
//
// Ambiguity: "when the end of the line has been reached" could be read as
// wrapping around (a circular Josephus).  It does not: the statement's own
// sample separates the two readings -- the linear reading gives "1 8" for
// 10 people / 2 survivors / cards 3 5 4 3 2, which is what is printed.
//
// Output format: "Selection #A", the survivors space-separated, then a blank
// line after every selection including the last.  The sample block in the
// statement shows trailing blanks on those lines, but two accepted sources on
// the discuss board emit none, so the judge tolerates their absence.
//
// Input is a stream of 22-integer records read to EOF (N, X, then 20 cards);
// the deck is guaranteed to finish the job within those 20 cards.

#include <cstdio>
#include <vector>

int main() {
    int n, x;
    int selection = 0;
    while (scanf("%d %d", &n, &x) == 2) {
        int card[20];
        for (int i = 0; i < 20; ++i) {
            if (scanf("%d", &card[i]) != 1) card[i] = 1;
        }
        ++selection;

        std::vector<int> line;
        for (int i = 1; i <= n; ++i) line.push_back(i);

        for (int c = 0; c < 20 && (int)line.size() > x; ++c) {
            int k = card[c];
            if (k < 1) continue;              /* defensive: statement says 1..11 */
            int i = 0, cnt = 0;
            while (i < (int)line.size()) {
                ++cnt;
                if (cnt == k) {
                    line.erase(line.begin() + i);
                    cnt = 0;
                    if ((int)line.size() == x) break;
                    /* i already indexes the next person */
                } else {
                    ++i;
                }
            }
        }

        printf("Selection #%d\n", selection);
        for (size_t i = 0; i < line.size(); ++i) {
            if (i) printf(" ");
            printf("%d", line[i]);
        }
        printf("\n\n");
    }
    return 0;
}
