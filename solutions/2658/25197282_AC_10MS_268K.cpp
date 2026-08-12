// POJ 2658 - Song
// Model: gpt-5.6-terra
// Approach: extract the case-insensitive last syllable of each line's final word,
// then test perfect, even, cross, and shell rhyme in that priority order.
// The discussion confirms that y is not a vowel and that comparison ignores all case.
#include <iostream>
#include <string>
#include <cctype>

using namespace std;

bool vowel(char c) {
    c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

string syllable(const string& line) {
    size_t start = line.rfind(' ');
    start = (start == string::npos) ? 0 : start + 1;
    size_t lastVowel = string::npos;
    for (size_t i = start; i < line.size(); ++i) {
        if (vowel(line[i])) lastVowel = i;
    }
    if (lastVowel != string::npos) start = lastVowel;
    string result = line.substr(start);
    for (size_t i = 0; i < result.size(); ++i) {
        result[i] = static_cast<char>(tolower(static_cast<unsigned char>(result[i])));
    }
    return result;
}

int main() {
    int n;
    if (!(cin >> n)) return 0;
    string line;
    getline(cin, line);
    while (n--) {
        string s[4];
        for (int i = 0; i < 4; ++i) getline(cin, s[i]);
        string r[4];
        for (int i = 0; i < 4; ++i) r[i] = syllable(s[i]);
        if (r[0] == r[1] && r[1] == r[2] && r[2] == r[3]) cout << "perfect\n";
        else if (r[0] == r[1] && r[2] == r[3]) cout << "even\n";
        else if (r[0] == r[2] && r[1] == r[3]) cout << "cross\n";
        else if (r[0] == r[3] && r[1] == r[2]) cout << "shell\n";
        else cout << "free\n";
    }
    return 0;
}
