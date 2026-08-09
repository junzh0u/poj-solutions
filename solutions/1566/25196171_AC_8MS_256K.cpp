// POJ 1566 - Haiku Review
// Model: gpt-5.6-terra
// Count maximal vowel runs in each slash-delimited line and report the first
// line whose count differs from the required 5/7/5 pattern.
#include <iostream>
#include <string>
using namespace std;

bool vowel(char c) {
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || c == 'y';
}

int syllables(const string &s) {
    int count = 0;
    bool inRun = false;
    for (size_t i = 0; i < s.size(); ++i) {
        if (vowel(s[i])) {
            if (!inRun) ++count;
            inRun = true;
        } else {
            inRun = false;
        }
    }
    return count;
}

int main() {
    string line;
    while (getline(cin, line) && line != "e/o/i") {
        int required[3] = {5, 7, 5};
        int part = 0, start = 0;
        char answer = 'Y';
        for (size_t i = 0; i <= line.size(); ++i) {
            if (i == line.size() || line[i] == '/') {
                if (answer == 'Y' && syllables(line.substr(start, i - start)) != required[part])
                    answer = char('1' + part);
                start = (int)i + 1;
                ++part;
            }
        }
        cout << answer << '\n';
    }
    return 0;
}
