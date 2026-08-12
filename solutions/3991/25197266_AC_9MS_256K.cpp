// POJ 3991 - Seinfeld
// Model: gpt-5.6-terra
// Scan the sequence while counting unmatched opening braces.  An unmatched
// closing brace must be flipped immediately; half of the openings left at the
// end must then be flipped to close the remaining pairs.
#include <iostream>
#include <string>

using namespace std;

int main() {
    string s;
    int caseNumber = 1;
    while (cin >> s && s[0] != '-') {
        int unmatchedOpen = 0;
        int changes = 0;
        int i;
        for (i = 0; i < (int)s.size(); ++i) {
            if (s[i] == '{') {
                ++unmatchedOpen;
            } else if (unmatchedOpen == 0) {
                ++changes;
                unmatchedOpen = 1;
            } else {
                --unmatchedOpen;
            }
        }
        changes += unmatchedOpen / 2;
        cout << caseNumber++ << ". " << changes << '\n';
    }
    return 0;
}
