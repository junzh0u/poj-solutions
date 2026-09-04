// POJ 2341 - Spell checker
// Model: claude-sonnet-5
//
// A word is a maximal run of letters. A sentence ends at '.', '?' or '!'
// (the board confirms a newline does NOT end a sentence, and that other
// stray characters such as '&' can appear in the input and must simply be
// treated as non-letter separators). Mistakes:
//   1) the first letter of the first word of a sentence is lowercase;
//   2) any non-first letter of a word is uppercase (each such letter is its
//      own mistake, e.g. "BIG" contributes 2: the 'I' and the 'G').
// A capital first letter of a word that is NOT a sentence start is not an
// error under either stated rule, and is not counted (verified against the
// sample: "Has", "No", "Kes" all sit mid- or post-sentence-reset positions
// consistent with this reading, and the sample's 3 mistakes are exactly
// 1 (lowercase 'a' starting "and") + 2 (the 'I' and 'G' of "BIG")).
// Read raw bytes to EOF; no test-case count is given.
#include <cstdio>
#include <cctype>

int main() {
    int c;
    bool sentenceStart = true;
    bool firstLetterOfWord = true;
    int mistakes = 0;
    while ((c = getchar()) != EOF) {
        if (isalpha((unsigned char)c)) {
            if (firstLetterOfWord) {
                if (sentenceStart && islower((unsigned char)c)) mistakes++;
                sentenceStart = false;
                firstLetterOfWord = false;
            } else {
                if (isupper((unsigned char)c)) mistakes++;
            }
        } else {
            firstLetterOfWord = true;
            if (c == '.' || c == '!' || c == '?') sentenceStart = true;
        }
    }
    printf("%d\n", mistakes);
    return 0;
}
