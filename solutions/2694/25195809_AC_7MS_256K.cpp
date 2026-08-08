// POJ 2694 - A Simple Poker Game
// Model: claude-opus-5
//
// Score each 5-card hand by the best category it qualifies for.  Cards are
// "<suit><rank>" with ranks coded A,2..9,X,J,Q,K, so rank letters map to 1..13.
// Sort the ranks, take the multiplicities of the distinct ranks, and test the
// categories in descending point order; the first match wins, which is exactly
// the statement's "apply only the rule that wins the largest amount".
//
// Ambiguity - which sequences count as a straight.  The statement blesses
// AKQJX explicitly, and the sample's second hand (A5432, scored 250) shows A
// also plays low, but neither says whether other wrap-arounds are legal.  The
// discuss board settles it: XJQKA is a straight and JQKA2 is not.  So a
// straight is a strictly consecutive run of the coded order, plus the single
// special case {A,X,J,Q,K} - i.e. exactly standard poker's ten sequences.
//
// The statement writes the lowest category as "O point"; the board confirms
// the output is the digit 0.  A 2005 board post reports the sample once
// carried a lower-case 'x', so input letters are upper-cased defensively.

#include <cstdio>
#include <cctype>
#include <algorithm>

using namespace std;

static const char RANKS[] = "A23456789XJQK"; // index 0 -> A (1), index 12 -> K (13)

static int rankOf(char c) {
    for (int i = 0; i < 13; i++)
        if (RANKS[i] == c) return i + 1;
    return 0;
}

static int score(int r[5], char s[5]) {
    sort(r, r + 5);

    bool flush = true;
    for (int i = 1; i < 5; i++)
        if (s[i] != s[0]) flush = false;

    bool run = true;
    for (int i = 1; i < 5; i++)
        if (r[i] != r[i - 1] + 1) run = false;
    // AKQJX is the only wrap-around accepted; JQKA2 and friends are not.
    bool akqjx = (r[0] == 1 && r[1] == 10 && r[2] == 11 && r[3] == 12 && r[4] == 13);
    bool straight = run || akqjx;

    // multiplicities of the distinct ranks present, largest first
    int mult[5], nd = 0;
    for (int i = 0; i < 5; i++) {
        if (i == 0 || r[i] != r[i - 1]) mult[nd++] = 1;
        else mult[nd - 1]++;
    }
    sort(mult, mult + nd);
    reverse(mult, mult + nd);

    if (straight && flush) return 1000;
    if (mult[0] == 4) return 750;
    if (mult[0] == 3 && mult[1] == 2) return 500;
    if (flush) return 350;
    if (straight) return 250;
    if (mult[0] == 3) return 200;
    if (mult[0] == 2 && mult[1] == 2) return 100;
    if (mult[0] == 2) return 50;
    return 0;
}

int main() {
    int w;
    if (scanf("%d", &w) != 1) return 0;
    for (int t = 0; t < w; t++) {
        int r[5];
        char s[5];
        for (int i = 0; i < 5; i++) {
            char buf[64];
            if (scanf("%63s", buf) != 1) return 0;
            s[i] = (char)toupper((unsigned char)buf[0]);
            r[i] = rankOf((char)toupper((unsigned char)buf[1]));
        }
        printf("%d\n", score(r, s));
    }
    return 0;
}
