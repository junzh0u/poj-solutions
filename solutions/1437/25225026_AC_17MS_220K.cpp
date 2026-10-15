// POJ 1437 - Chat Rooms
// Model: gpt-6.1-sol
// Keep the previous ten input lines, including rejected and empty lines.
// Reject runs of six consonants immediately; a run of five is rejected
// only after more than two such previous lines. Repetition needs two
// previous copies, and all history tests exclude the current line.
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

bool consonant(char c) {
    if (c >= 'A' && c <= 'Z') c = char(c - 'A' + 'a');
    return c >= 'a' && c <= 'z' && string("aeiouy").find(c) == string::npos;
}
int main() {
    int n;
    if (!(cin >> n)) return 0;
    string s;
    getline(cin, s);
    vector<string> lines(10);
    int longRun[10] = {0};
    for (int i = 0; i < n; ++i) {
        getline(cin, s);
        if (!s.empty() && s[s.size()-1] == '\r') s.erase(s.size()-1);
        int run = 0, longest = 0;
        for (size_t j = 0; j < s.size(); ++j) {
            run = consonant(s[j]) ? run + 1 : 0;
            longest = max(longest, run);
        }
        int copies = 0, bad = 0;
        for (int j = max(0, i-10); j < i; ++j) {
            if (lines[j%10] == s) ++copies;
            bad += longRun[j%10];
        }
        cout << ((longest > 5 || (longest > 4 && bad > 2) || copies > 1) ? 'n' : 'y') << '\n';
        lines[i%10] = s;
        longRun[i%10] = longest > 4;
    }
    return 0;
}
