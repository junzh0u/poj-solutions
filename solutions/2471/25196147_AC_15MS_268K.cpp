// POJ 2471 - Bullshit Bingo
// Model: gpt-5.6-terra
// Split the lecture text at every non-letter, normalize words to lowercase,
// and count the distinct words separately for each completed game.
// Only the uppercase terminator BULLSHIT ends a game; an unfinished suffix is ignored.
#include <iostream>
#include <set>
#include <string>

using namespace std;

static bool isLetter(char ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

static char lowerLetter(char ch) {
    if (ch >= 'A' && ch <= 'Z') return char(ch - 'A' + 'a');
    return ch;
}

int main() {
    set<string> words;
    string word;
    long total = 0;
    long games = 0;
    char ch;

    while (cin.get(ch)) {
        if (isLetter(ch)) {
            word += lowerLetter(ch);
        } else if (!word.empty()) {
            if (word == "bullshit") {
                total += long(words.size());
                ++games;
                words.clear();
            } else {
                words.insert(word);
            }
            word.clear();
        }
    }
    if (!word.empty() && word == "bullshit") {
        total += long(words.size());
        ++games;
    }

    long a = total, b = games;
    while (b != 0) {
        long r = a % b;
        a = b;
        b = r;
    }
    cout << total / a << " / " << games / a << '\n';
    return 0;
}
