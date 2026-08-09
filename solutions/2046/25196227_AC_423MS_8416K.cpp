// POJ 2046 - Gap
// Model: claude-sonnet-5
//
// Approach: plain BFS with full state deduplication (a std::string of the
// 32 board cells as the hash key). After the fixed initial step -- moving
// every value-1 card to the left end of its own suit's row, which is not
// counted in the answer -- the board is 4 rows x 8 columns; column 0 of
// each row permanently holds that row's value-1 card and never moves again
// (no legal move can ever displace it, since a card only moves when it is
// the successor of some other card, and value-1 cards are nobody's
// successor).
//
// A move: pick a gap cell (r,c) with c>0; if its left neighbor L exists (is
// a real card, not a gap) and has a successor S = L+1 within the same suit
// (L's value < 7), move the card S (wherever it currently sits on the
// board) into the gap; S's old cell becomes the new gap. This is exactly
// what the statement forbids otherwise: "cannot move any card to the right
// of a card of value 7, nor to the right of a gap".
//
// The goal is each row holding, in columns 0..6, that suit's cards 1..7 in
// order, with a single gap at column 7.
//
// This is an unweighted shortest-path search over the move graph, so BFS
// with a visited-state set gives the exact minimum move count, and -1 falls
// out naturally once the frontier empties without ever reaching the goal
// (i.e. the whole reachable state space has been exhausted).
//
// An initial attempt used IDA* with the classic "count cards whose left
// neighbor isn't their true predecessor" admissible heuristic and a
// "don't immediately undo the last move" prune. That heuristic is genuine
// (each move corrects at most one card's placement) but IDA* without a
// transposition table can still loop through longer (>2-move) cycles that
// the single-ply undo-prune doesn't catch, and on this puzzle that blew up
// badly (didn't finish in tens of seconds on the 33-move sample case, and
// a bug in an incremental heuristic update -- which double-subtracted a
// cell's contribution whenever the moved card sat immediately right of the
// gap, the most common move shape -- separately produced a wrong, too-small
// answer). The problem's own discuss board confirms this: several posters
// note the reachable state space from any deal is small enough that even a
// naive BFS with a std::set<string> for dedup passes comfortably within the
// time limit ("限制足够的强，状态足够的少" -- the constraints are strong
// enough, the state count is small enough), and that matches what a
// reference BFS in Python confirmed locally in well under a second per
// case. So BFS with a std::set<string> of encoded states is used here
// instead of IDA*, trading a heuristic for straightforward robustness
// (std::set rather than a hash table, to stay clear of any doubt about
// <unordered_set> on POJ's old compiler).
//
// Statement ambiguity: none found; case 1 of the sample (answer 0) pins
// down the fixed initial-setup detail unambiguously.

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <set>

static int board[4][8];

static std::string encode() {
    std::string s;
    s.resize(32);
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 8; c++)
            s[r * 8 + c] = (char) board[r][c];
    return s;
}

static bool isGoal(const std::string &s) {
    for (int r = 0; r < 4; r++) {
        for (int v = 1; v <= 7; v++)
            if ((unsigned char) s[r * 8 + (v - 1)] != (unsigned) ((r + 1) * 10 + v)) return false;
        if ((unsigned char) s[r * 8 + 7] != 0) return false;
    }
    return true;
}

// generate all legal successor states from encoded state s
static void neighbors(const std::string &s, std::vector<std::string> &out) {
    unsigned char cell[32];
    for (int i = 0; i < 32; i++) cell[i] = (unsigned char) s[i];

    for (int r = 0; r < 4; r++) {
        for (int c = 1; c < 8; c++) {
            int gp = r * 8 + c;
            if (cell[gp] != 0) continue;
            int L = cell[gp - 1];
            if (L == 0) continue;
            if (L % 10 == 7) continue;
            int S = L + 1;
            int srcPos = -1;
            for (int i = 0; i < 32; i++) {
                if (cell[i] == S) { srcPos = i; break; }
            }
            // S must exist somewhere on the board
            std::string t = s;
            t[gp] = (char) S;
            t[srcPos] = (char) 0;
            out.push_back(t);
        }
    }
}

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T--) {
        int raw[4][7];
        for (int r = 0; r < 4; r++)
            for (int c = 0; c < 7; c++)
                scanf("%d", &raw[r][c]);

        // Original layout: 4 rows x 7 cards at columns 1..7 (column 0 is an
        // empty space in every row). Every value-1 card is then removed
        // from wherever it sits and placed at column 0 of the row matching
        // its own suit (leaving a gap at its old spot); every other card
        // stays in its original row, shifted one column right. Each of
        // these placements targets a distinct board cell, so this leaves
        // exactly 4 empty cells (the old homes of the four value-1 cards).
        //
        // Column indices are then shifted down by one relative to the
        // statement's own (col1..col7,col0-gap) picture, so that internally
        // col0..col6 hold values 1..7 and col7 is the gap -- purely a
        // bookkeeping choice, made consistent throughout this file.
        memset(board, 0, sizeof(board));
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 7; c++) {
                int card = raw[r][c];
                int suit = card / 10;
                int val = card % 10;
                if (val == 1) board[suit - 1][0] = card;
                else board[r][c + 1] = card;
            }
        }

        std::string start = encode();

        int answer = -1;
        if (isGoal(start)) {
            answer = 0;
        } else {
            std::set<std::string> visited;
            visited.insert(start);
            std::vector<std::string> frontier;
            frontier.push_back(start);
            int dist = 0;
            bool done = false;
            while (!frontier.empty() && !done) {
                dist++;
                std::vector<std::string> next;
                std::vector<std::string> nb;
                for (size_t i = 0; i < frontier.size() && !done; i++) {
                    nb.clear();
                    neighbors(frontier[i], nb);
                    for (size_t j = 0; j < nb.size(); j++) {
                        if (visited.find(nb[j]) != visited.end()) continue;
                        if (isGoal(nb[j])) { answer = dist; done = true; break; }
                        visited.insert(nb[j]);
                        next.push_back(nb[j]);
                    }
                }
                frontier.swap(next);
            }
        }
        printf("%d\n", answer);
    }
    return 0;
}
