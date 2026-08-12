// POJ 1583 - Choose Your Words Carefully
// Model: gpt-5.6-terra
// Approach: scan the input character by character, collecting maximal
// alphanumeric runs after lowercasing letters, then count them in an ordered map.
// Punctuation always separates words; digits remain part of a word.
#include <cctype>
#include <iostream>
#include <map>
#include <string>

using namespace std;

int main() {
    map<string, int> count;
    string word;
    int ch;
    int best = 0;

    while ((ch = cin.get()) != EOF) {
        unsigned char c = static_cast<unsigned char>(ch);
        if (isalnum(c)) {
            if (isalpha(c)) c = static_cast<unsigned char>(tolower(c));
            word += static_cast<char>(c);
        } else if (!word.empty()) {
            int now = ++count[word];
            if (now > best) best = now;
            word.clear();
        }
    }
    if (!word.empty()) {
        int now = ++count[word];
        if (now > best) best = now;
    }

    cout << best << " occurrences\n";
    for (map<string, int>::const_iterator it = count.begin(); it != count.end(); ++it) {
        if (it->second == best) cout << it->first << '\n';
    }
    return 0;
}
