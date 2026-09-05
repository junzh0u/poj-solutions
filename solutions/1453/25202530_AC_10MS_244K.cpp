// POJ 1453 - Delta Encoding and Decoding
// Model: claude-sonnet-5
//
// Approach: maintain a forward map fwd[letter-'A'] = numeric value (0..25)
// and its inverse inv[value] = letter, both starting at the identity
// (default) cipher. For ENCRYPT/DECRYPT, walk the text left to right
// tracking "prev", the numeric value (under the *current* cipher) of the
// previous letter; a non-letter character resets prev to fwd['A'] (rule 2:
// a letter preceded by a non-letter is treated as preceded by an A). For
// ENCRYPT the output letter's value is (value(cur) - prev) mod 26; for
// DECRYPT the decoded value is (prev + value(cur)) mod 26, and prev is then
// advanced using that letter's own (plaintext) value in both directions, so
// encryption and decryption chain through the plaintext's values exactly as
// the cipher construction requires. Case is preserved per character.
//
// Statement ambiguity, settled from the POJ discuss board (message 6039/
// 6052, a widely referenced WA writeup): commands are read with
// scanf("%s")-style tokenizing (here `cin >> word`), and the remainder of
// the line is taken verbatim via getline from right after that token -- it
// is NOT stripped of its leading separator whitespace. Since that leading
// whitespace is non-letter, it passes through unchanged into the
// ENCRYPT/DECRYPT output, which is why "RESULT:" is followed by two spaces
// in the sample (one from the fixed "RESULT: " prefix, one carried over
// from the single mandatory separator space in the input). Any additional
// spaces after a command word are likewise preserved, not collapsed.
//
// CIPHER validity: strip everything but letters from the rest of the line;
// it must yield exactly 26 letters forming a permutation of A-Z, else
// "Bad cipher.  Using default." and the cipher reverts to identity.

#include <cstdio>
#include <cctype>
#include <string>
#include <iostream>
using namespace std;

int fwd[26];
char inv[26];

void setDefaultCipher() {
    for (int i = 0; i < 26; ++i) {
        fwd[i] = i;
        inv[i] = char('A' + i);
    }
}

string processText(const string &text, bool encrypt) {
    string result = text;
    int prev = fwd[0]; // value of 'A' under current cipher
    for (size_t i = 0; i < text.size(); ++i) {
        unsigned char c = (unsigned char)text[i];
        if (isalpha(c)) {
            bool upper = isupper(c) != 0;
            int letterIdx = toupper(c) - 'A';
            int val = fwd[letterIdx];
            int newval;
            if (encrypt) {
                newval = ((val - prev) % 26 + 26) % 26;
            } else {
                newval = (val + prev) % 26;
            }
            char newletter = inv[newval];
            result[i] = upper ? newletter : (char)tolower((unsigned char)newletter);
            // advance prev using the plaintext letter's own value:
            // for ENCRYPT that's val (the current input letter's value);
            // for DECRYPT that's newval (the decoded value we just computed).
            prev = encrypt ? val : newval;
        } else {
            prev = fwd[0];
        }
    }
    return result;
}

int main() {
    ios::sync_with_stdio(false);
    setDefaultCipher();

    string word;
    while (cin >> word) {
        string rest;
        getline(cin, rest);
        if (!rest.empty() && rest[rest.size() - 1] == '\r') {
            rest.erase(rest.size() - 1);
        }

        string upperWord = word;
        for (size_t i = 0; i < upperWord.size(); ++i) {
            upperWord[i] = (char)toupper((unsigned char)upperWord[i]);
        }

        if (upperWord == "ENCRYPT") {
            cout << "RESULT: " << processText(rest, true) << "\n";
        } else if (upperWord == "DECRYPT") {
            cout << "RESULT: " << processText(rest, false) << "\n";
        } else if (upperWord == "CIPHER") {
            string letters;
            for (size_t i = 0; i < rest.size(); ++i) {
                unsigned char c = (unsigned char)rest[i];
                if (isalpha(c)) {
                    letters += (char)toupper(c);
                }
            }
            bool ok = false;
            if (letters.size() == 26) {
                bool seen[26] = {false};
                ok = true;
                for (int i = 0; i < 26; ++i) {
                    int idx = letters[i] - 'A';
                    if (seen[idx]) { ok = false; break; }
                    seen[idx] = true;
                }
            }
            if (ok) {
                for (int i = 0; i < 26; ++i) {
                    inv[i] = letters[i];
                    fwd[letters[i] - 'A'] = i;
                }
                cout << "Good cipher.  Using " << letters << ".\n";
            } else {
                setDefaultCipher();
                cout << "Bad cipher.  Using default.\n";
            }
        } else {
            cout << "Command not understood.\n";
        }
    }
    return 0;
}
