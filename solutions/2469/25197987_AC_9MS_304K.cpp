// POJ 2469 - Stack 'em Up
// Model: claude-sonnet-5
// Approach: simulate cumulative deck shuffles. Each shuffle definition gives,
// for position j, the source position i in the *current* deck whose card
// moves to position j: newDeck[j] = curDeck[shuffle[j]]. Starting from the
// standard ordered deck, apply each observed shuffle index k in sequence to
// the deck as it stands after the previous observation (cumulative, not
// reset to the original deck each time -- confirmed against sample output
// and the discuss board), printing the resulting 52 card names followed by
// a blank line after each observation. Read observations to EOF (only one
// data set per the discuss board).
#include <cstdio>

static const char *VALS[13] = {
    "2","3","4","5","6","7","8","9","10","Jack","Queen","King","Ace"
};
static const char *SUITS[4] = {
    "Clubs","Diamonds","Hearts","Spades"
};

int shuffles[101][53]; // 1-indexed, shuffles[k][j] = i
int cur[53];

void printCard(int c) {
    // c is 1..52
    int idx = c - 1;
    int suit = idx / 13;
    int val = idx % 13;
    printf("%s of %s\n", VALS[val], SUITS[suit]);
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    for (int k = 1; k <= n; ++k) {
        for (int j = 1; j <= 52; ++j) {
            scanf("%d", &shuffles[k][j]);
        }
    }
    for (int i = 1; i <= 52; ++i) cur[i] = i;

    int k;
    int next[53];
    while (scanf("%d", &k) == 1) {
        for (int j = 1; j <= 52; ++j) {
            next[j] = cur[shuffles[k][j]];
        }
        for (int j = 1; j <= 52; ++j) cur[j] = next[j];
        for (int j = 1; j <= 52; ++j) printCard(cur[j]);
        printf("\n");
    }
    return 0;
}
