// POJ 1576 - Colorville
// Model: claude-sonnet-5
// Approach: direct simulation. Each player has a position (0 = before the
// first square, N = the last square / winning). Cards are drawn round-robin
// starting at player 1. A single-letter card moves the current player
// forward to the next occurrence of that color after their current
// position; a doubled-letter card requires finding two occurrences of that
// color after the current position (moving to the second one). If the
// required occurrence(s) cannot be found ahead, the player instead advances
// straight to the last square, winning immediately (this also covers the
// board's own noted special case: a doubled card that finds only the first
// occurrence still wins). The deck for a game must be fully read up front
// (even past a win) so the input stream stays aligned with the next game.
#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

int main() {
    int nPlayers, nSquares, nCards;
    while (cin >> nPlayers >> nSquares >> nCards) {
        if (nPlayers == 0) break;
        string board;
        cin >> board;
        vector<string> cards(nCards);
        for (int i = 0; i < nCards; i++) cin >> cards[i];

        int pos[5] = {0, 0, 0, 0, 0}; // players 1..4
        int winner = -1;
        int cardsDrawn = nCards;

        for (int i = 0; i < nCards; i++) {
            int player = (i % nPlayers) + 1;
            const string &card = cards[i];
            char c = card[0];
            int curpos = pos[player];
            int newpos;

            if (card.size() == 1) {
                int k = -1;
                for (int j = curpos + 1; j <= nSquares; j++) {
                    if (board[j - 1] == c) { k = j; break; }
                }
                newpos = (k == -1) ? nSquares : k;
            } else {
                int k1 = -1, k2 = -1;
                for (int j = curpos + 1; j <= nSquares; j++) {
                    if (board[j - 1] == c) { k1 = j; break; }
                }
                if (k1 != -1) {
                    for (int j = k1 + 1; j <= nSquares; j++) {
                        if (board[j - 1] == c) { k2 = j; break; }
                    }
                }
                newpos = (k1 == -1 || k2 == -1) ? nSquares : k2;
            }

            pos[player] = newpos;
            if (newpos == nSquares) {
                winner = player;
                cardsDrawn = i + 1;
                break;
            }
        }

        if (winner != -1) {
            printf("Player %d won after %d cards.\n", winner, cardsDrawn);
        } else {
            printf("No player won after %d cards.\n", nCards);
        }
    }
    return 0;
}
