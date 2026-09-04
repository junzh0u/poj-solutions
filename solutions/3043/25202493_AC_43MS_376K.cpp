// POJ 3043 - Walk the Talk
// Model: claude-sonnet-5
//
// For each word, count paths in the grid that spell it, where a path is a
// sequence of cells (r0,c0),(r1,c1),...,(rk-1,ck-1) with, at every step,
// r_{i+1} <= r_i and c_{i+1} >= c_i and the two cells distinct (i.e. only
// moves to the right and/or above are allowed, any distance).  Sum the
// counts over all words in the (EOF-terminated) word list.
//
// DP: f[r][c] = number of partial paths ending at (r,c) that spell the
// prefix of the current word seen so far.  To extend by one more required
// letter, every predecessor cell (r',c') with r'>=r, c'<=c, (r',c')!=(r,c)
// contributes.  That predecessor set is a rectangle (rows r..H-1, columns
// 0..c), so a single O(H*W) sweep computes a suffix-in-rows/prefix-in-cols
// sum S[r][c] over the whole board, and the new f is S[r][c]-f[r][c] (to
// drop the self term) wherever the letter matches, else 0.  Summing the
// final layer over all cells gives the word's total path count; these are
// added up over all words for the answer.
//
// Ambiguity: the statement's own sample input elides the real word list
// with "..." after "A / AA / AAA" but the linked dict-twalk.txt begins
// exactly that way and has 2265 words (max length 4 in that file, though
// the algorithm makes no assumption about word length) -- confirming the
// sample's word list is that entire file.  The discuss board (msgs 139689,
// 71101) flags a possible 32-bit overflow, so counts are accumulated in
// unsigned long long; output goes through iostream to sidestep any
// %lld/%I64d formatting question on the older compiler.
#include <iostream>
#include <string>
#include <cstring>
using namespace std;

typedef unsigned long long u64;

int H, W;
char grid[32][32];

int main() {
    ios::sync_with_stdio(false);
    if (!(cin >> H >> W)) return 0;
    for (int r = 0; r < H; ++r) {
        string row;
        cin >> row;
        for (int c = 0; c < W; ++c) grid[r][c] = (c < (int)row.size() ? row[c] : ' ');
    }

    u64 answer = 0;
    static u64 f[32][32];
    static u64 nf[32][32];
    static u64 S[34][34];

    string word;
    while (cin >> word) {
        int L = (int)word.size();
        if (L == 0) continue;
        for (int r = 0; r < H; ++r)
            for (int c = 0; c < W; ++c)
                f[r][c] = (grid[r][c] == word[0]) ? 1ULL : 0ULL;

        for (int k = 1; k < L; ++k) {
            // S[r][c+1] = sum_{r'>=r, c'<=c} f[r'][c'], with sentinel row H
            // and sentinel column -1 (stored at index 0) both zero.
            for (int c = 0; c <= W; ++c) S[H][c] = 0;
            for (int r = H - 1; r >= 0; --r) {
                S[r][0] = 0;
                for (int c = 0; c < W; ++c)
                    S[r][c + 1] = f[r][c] + S[r + 1][c + 1] + S[r][c] - S[r + 1][c];
            }
            char ch = word[k];
            for (int r = 0; r < H; ++r) {
                for (int c = 0; c < W; ++c) {
                    if (grid[r][c] == ch) {
                        u64 rect = S[r][c + 1];
                        nf[r][c] = rect - f[r][c];
                    } else {
                        nf[r][c] = 0;
                    }
                }
            }
            memcpy(f, nf, sizeof(f));
        }

        for (int r = 0; r < H; ++r)
            for (int c = 0; c < W; ++c)
                answer += f[r][c];
    }

    cout << answer << "\n";
    return 0;
}
