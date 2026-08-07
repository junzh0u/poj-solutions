// POJ 2973 - Scrabble
// Model: claude-opus-5
//
// Multiple test cases until n == 0.  For each test case: n dictionary words
// (1..7 uppercase letters), then a rack of 1..7 tiles ('A'..'Z' or '_' for a
// blank).  Count the dictionary words spellable from the rack, each tile used
// at most once, blanks standing for any letter.
//
// The tiles are an unordered multiset (the discuss board's one warning: it is
// not a substring/subsequence question), so a word is spellable exactly when
//     sum over letters c of max(0, need[c] - have[c])  <=  (number of blanks).
// The deficit sum is the count of word positions no lettered tile can cover;
// each must be paid for by a distinct blank.  This also subsumes the length
// check, since every unmatched position consumes one blank.
//
// Ambiguity note: none in the rule itself.  The only input framing worth
// stating is that n == 0 terminates the file and carries no rack line after it.

#include <cstdio>
#include <cstring>

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        static char words[1000][16];
        for (int i = 0; i < n; ++i)
            if (scanf("%15s", words[i]) != 1) return 0;

        char rack[32];
        if (scanf("%31s", rack) != 1) return 0;

        int have[26];
        memset(have, 0, sizeof(have));
        int blanks = 0;
        for (int i = 0; rack[i] != '\0'; ++i) {
            if (rack[i] == '_') ++blanks;
            else if (rack[i] >= 'A' && rack[i] <= 'Z') ++have[rack[i] - 'A'];
        }

        int count = 0;
        for (int i = 0; i < n; ++i) {
            int need[26];
            memset(need, 0, sizeof(need));
            for (int j = 0; words[i][j] != '\0'; ++j)
                ++need[words[i][j] - 'A'];
            int deficit = 0;
            for (int c = 0; c < 26; ++c)
                if (need[c] > have[c]) deficit += need[c] - have[c];
            if (deficit <= blanks) ++count;
        }
        printf("%d\n", count);
    }
    return 0;
}
