// POJ 3371 - Flesch Reading Ease
// Model: claude-sonnet-5
//
// Approach: single pass over the input character stream.
//   Sentences: count every occurrence of '.', '!', ':', ';' (the board
//   confirms literally only these four -- NOT '?', despite '?' being in
//   the allowed input punctuation set; the author's "!"/explanation point"
//   wording means the exclamation mark only, question marks never split
//   a sentence).
//   Words: a maximal run of non-blank characters, with any leading/trailing
//   non-letter characters stripped (internal punctuation such as an
//   apostrophe or hyphen is kept). A run that has no letters at all is not
//   a word.
//   Syllables per word:
//     - if the (post-strip) word has length <= 3, it counts as exactly one
//       syllable, full stop (checked on the *original* stripped length,
//       before any suffix removal -- confirmed by the board: a 2-letter
//       "es" is still one syllable via this rule, not zero).
//     - otherwise, drop a trailing "es" or "ed" (case-insensitively), or a
//       trailing "e" unless it is part of "-le"; then count maximal runs of
//       consecutive vowels (a e i o u y, case-insensitive) in what's left,
//       each run worth one syllable.
//
// Verified against: the statement's own sample (26.09), and a case from
// the discuss board ("A. Ole drei end dead fucked. AES. DES. Maerlyn's.
// Gute. TTOLE." -> 105.26) which exercises the <=3-letter shortcut, the
// -es/-ed/-e suffix rule, and the -le exception (TTOLE) simultaneously.
#include <cstdio>
#include <cctype>
#include <cstring>
#include <string>
using namespace std;

static bool isVowel(char c) {
    c = (char)tolower((unsigned char)c);
    return c=='a'||c=='e'||c=='i'||c=='o'||c=='u'||c=='y';
}

static int countSyllables(const string &word) {
    int len = (int)word.length();
    if (len <= 3) return 1;
    string core = word;
    // lower-cased view for suffix matching
    string lower = word;
    for (int i = 0; i < len; ++i) lower[i] = (char)tolower((unsigned char)lower[i]);
    if (len >= 2 && lower[len-2]=='e' && lower[len-1]=='s') {
        core = word.substr(0, len-2);
    } else if (len >= 2 && lower[len-2]=='e' && lower[len-1]=='d') {
        core = word.substr(0, len-2);
    } else if (lower[len-1]=='e') {
        if (len >= 2 && lower[len-2]=='l') {
            // -le ending: exception, keep the e
        } else {
            core = word.substr(0, len-1);
        }
    }
    int syl = 0;
    bool prevVowel = false;
    for (size_t i = 0; i < core.length(); ++i) {
        bool v = isVowel(core[i]);
        if (v && !prevVowel) syl++;
        prevVowel = v;
    }
    return syl;
}

int main() {
    long sentences = 0, words = 0, syllables = 0;
    string token;
    int c;
    while ((c = getchar()) != EOF) {
        if (isspace(c)) {
            if (!token.empty()) {
                int len = (int)token.length();
                int first = -1, last = -1;
                for (int i = 0; i < len; ++i) if (isalpha((unsigned char)token[i])) { first = i; break; }
                for (int i = len-1; i >= 0; --i) if (isalpha((unsigned char)token[i])) { last = i; break; }
                if (first != -1) {
                    string word = token.substr(first, last-first+1);
                    words++;
                    syllables += countSyllables(word);
                }
                token.clear();
            }
        } else {
            if (c=='.'||c=='!'||c==':'||c==';') sentences++;
            token += (char)c;
        }
    }
    if (!token.empty()) {
        int len = (int)token.length();
        int first = -1, last = -1;
        for (int i = 0; i < len; ++i) if (isalpha((unsigned char)token[i])) { first = i; break; }
        for (int i = len-1; i >= 0; --i) if (isalpha((unsigned char)token[i])) { last = i; break; }
        if (first != -1) {
            string word = token.substr(first, last-first+1);
            words++;
            syllables += countSyllables(word);
        }
    }

    double score = 206.835
        - 1.015 * ((double)words / (double)sentences)
        - 84.6 * ((double)syllables / (double)words);
    if (score < 0 && score > -0.005) score = 0.0;
    printf("%.2f\n", score);
    return 0;
}
