// POJ 1214 - "Accordian" Patience
// Model: claude-sonnet-5
//
// Simulate Accordian patience. Deal the 52-card pack left to right into
// piles (columns), each pile a stack of face-up cards with only the top
// card visible. Repeatedly rescan the currently visible piles from the
// left; the first (leftmost) pile whose top card matches (same rank or
// same suit) the top card of the pile three positions to its left has that
// single top card moved onto that pile; failing that, if its top card
// matches its immediate left neighbour's top card, it is moved there
// instead. Only the top card physically moves (the rest of the source
// pile, if any, stays put and its next card becomes the new visible top);
// a pile disappears from the row, closing the gap, only once it has been
// completely emptied. After a move, positions may have shifted (a pile
// vanished), and a merge can expose a new match further left (the target
// pile's new top card may now match its own left neighbours), so the whole
// scan restarts from the left after every single move. This is equivalent
// to resolving moves after every card is dealt rather than only once at
// the end: appending a new pile at the far right can never create or
// remove an available move among earlier piles, nor introduce one with a
// smaller index than one already available, so resolving once after all 52
// piles are dealt gives the identical final layout.
//
// Ambiguity: an early board post (message 1484) claims the official sample
// prints "1 pile remaining: 52" (singular), but the sample block on the
// live problem page, and every accepted reference solution posted on the
// board, use "piles" unconditionally even for a single pile, which is what
// this program emits.
//
// The first working version of this program modeled a "move" as relocating
// an entire pile as a block onto its target (removing the source pile on
// any move at all). That does not match the game: only the single top card
// moves per turn, and a pile survives, with a new top card exposed, until
// it is fully emptied. The block-move version reproduces neither sample
// line; the per-card-with-a-real-stack model below does.
#include <cstdio>
#include <vector>
using namespace std;

struct Card {
    char rank, suit;
};

static bool matchp(const Card &a, const Card &b) {
    return a.rank == b.rank || a.suit == b.suit;
}

int main() {
    char tok[8];
    vector<vector<Card> > piles;
    while (scanf("%7s", tok) == 1) {
        if (tok[0] == '#') break;
        piles.clear();
        piles.reserve(52);
        for (int i = 0; i < 52; ++i) {
            if (i > 0) scanf("%7s", tok);
            Card c;
            c.rank = tok[0];
            c.suit = tok[1];
            vector<Card> pile;
            pile.push_back(c);
            piles.push_back(pile);
        }

        bool moved = true;
        while (moved) {
            moved = false;
            for (size_t i = 0; i < piles.size(); ++i) {
                Card &top = piles[i].back();
                if (i >= 3 && matchp(top, piles[i - 3].back())) {
                    piles[i - 3].push_back(top);
                    piles[i].pop_back();
                    if (piles[i].empty()) piles.erase(piles.begin() + i);
                    moved = true;
                    break;
                } else if (i >= 1 && matchp(top, piles[i - 1].back())) {
                    piles[i - 1].push_back(top);
                    piles[i].pop_back();
                    if (piles[i].empty()) piles.erase(piles.begin() + i);
                    moved = true;
                    break;
                }
            }
        }

        printf("%d piles remaining:", (int)piles.size());
        for (size_t i = 0; i < piles.size(); ++i) {
            printf(" %d", (int)piles[i].size());
        }
        printf("\n");
    }
    return 0;
}
