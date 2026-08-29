// POJ 2089 - Magic Trick
// Model: claude-sonnet-5
// Approach: reverse the Fitch-Cheney five-card trick. The first shown card y
// fixes the hidden card's suit and gives its face value as a base point on the
// cyclic 1..13 ring. The remaining three cards, compared under the stated
// suit-then-face order to get z1<z2<z3, are shown in one of six permutations;
// which permutation matches the presented order gives an offset n in 1..6.
// The hidden card's face value is (y.face - 1 + n) mod 13 + 1, same suit as y.
// No statement ambiguity found; verified against both samples and against the
// discuss board postings (no algorithmic pitfall reported there, only local
// coding slips in other people's submissions).
#include <cstdio>
#include <algorithm>
using namespace std;

int rankOf(char c) {
    if (c >= '1' && c <= '9') return c - '0';
    if (c == 'T') return 10;
    if (c == 'J') return 11;
    if (c == 'Q') return 12;
    return 13; // 'K'
}

char rankChar(int r) {
    if (r <= 9) return '0' + r;
    if (r == 10) return 'T';
    if (r == 11) return 'J';
    if (r == 12) return 'Q';
    return 'K';
}

int suitOf(char c) {
    // H < C < D < S
    if (c == 'H') return 0;
    if (c == 'C') return 1;
    if (c == 'D') return 2;
    return 3; // 'S'
}

struct Card {
    int rank, suit;
    bool operator==(const Card& o) const { return rank == o.rank && suit == o.suit; }
};

bool cardLess(const Card& a, const Card& b) {
    if (a.suit != b.suit) return a.suit < b.suit;
    return a.rank < b.rank;
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    char buf[8];
    for (int t = 0; t < n; ++t) {
        Card c[4];
        for (int i = 0; i < 4; ++i) {
            scanf("%s", buf);
            c[i].rank = rankOf(buf[0]);
            c[i].suit = suitOf(buf[1]);
        }
        Card z[3] = { c[1], c[2], c[3] };
        Card sorted[3] = { c[1], c[2], c[3] };
        sort(sorted, sorted + 3, cardLess);
        // sorted[0]=z1 < sorted[1]=z2 < sorted[2]=z3
        int n6 = -1;
        // z[0..2] is the presented order; determine which of z1,z2,z3 each is.
        int idx[3];
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (z[i] == sorted[j]) { idx[i] = j; break; }
            }
        }
        // idx[i] in {0,1,2} meaning z1,z2,z3 respectively, presented order idx[0],idx[1],idx[2]
        if (idx[0]==0 && idx[1]==1 && idx[2]==2) n6 = 1;
        else if (idx[0]==0 && idx[1]==2 && idx[2]==1) n6 = 2;
        else if (idx[0]==1 && idx[1]==0 && idx[2]==2) n6 = 3;
        else if (idx[0]==1 && idx[1]==2 && idx[2]==0) n6 = 4;
        else if (idx[0]==2 && idx[1]==0 && idx[2]==1) n6 = 5;
        else if (idx[0]==2 && idx[1]==1 && idx[2]==0) n6 = 6;

        int hiddenRank = ((c[0].rank - 1 + n6) % 13) + 1;
        int hiddenSuit = c[0].suit;
        char suitChar = (hiddenSuit==0?'H':hiddenSuit==1?'C':hiddenSuit==2?'D':'S');
        printf("%c%c\n", rankChar(hiddenRank), suitChar);
    }
    return 0;
}
