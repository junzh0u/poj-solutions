// POJ 1629 - Fillword
// Model: claude-opus-5
//
// No search is needed.  Every one of the P words occupies |W| pairwise
// distinct cells (a word may not reuse a cell), and no cell belongs to two
// words, so the cells covered by the words carry exactly the multiset of
// letters obtained by concatenating all P words.  The statement guarantees a
// solution always exists, hence the leftover letters are the grid's letter
// multiset minus that concatenation -- the same answer for every valid
// placement, so the placement itself never has to be found.
//
// Read the grid rows and the words as whitespace-delimited tokens so that CR
// line endings and the newline left over after the N/M/P line cannot shift the
// stream.  Output the remaining letters sorted (counting sort over 'A'..'Z').

#include <cstdio>

int main() {
    int n, m, p;
    while (scanf("%d %d %d", &n, &m, &p) == 3) {
        int cnt[26];
        for (int i = 0; i < 26; i++) cnt[i] = 0;

        char buf[256];
        for (int i = 0; i < n; i++) {
            if (scanf("%255s", buf) != 1) break;
            for (int j = 0; buf[j]; j++)
                if (buf[j] >= 'A' && buf[j] <= 'Z') cnt[buf[j] - 'A']++;
        }
        for (int i = 0; i < p; i++) {
            if (scanf("%255s", buf) != 1) break;
            for (int j = 0; buf[j]; j++)
                if (buf[j] >= 'A' && buf[j] <= 'Z') cnt[buf[j] - 'A']--;
        }

        for (int c = 0; c < 26; c++)
            for (int k = 0; k < cnt[c]; k++) putchar('A' + c);
        putchar('\n');
    }
    return 0;
}
