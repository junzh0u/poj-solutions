// POJ 2119 - God of the Vile Baskers
// Model: gpt-6.1-sol
// Slide over consecutive alphabetic characters, keeping their 26 counts.
// An ordered set stores exact count vectors, with no hashing collisions.
// Substrings may overlap; the first repeated vector ends the valid prefix
// immediately before its final letter, including intervening punctuation.
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <limits>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int k;
    while (cin >> k && k) {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        string text;
        getline(cin, text);
        if (!text.empty() && text[text.size()-1] == '\r') text.erase(text.size()-1);
        vector<int> letters;
        string counts(26, '\0');
        set<string> seen;
        int answer = (int)text.size();
        for (int i = 0; i < (int)text.size(); ++i) {
            char c = text[i];
            int x = -1;
            if (c >= 'a' && c <= 'z') x = c - 'a';
            if (c >= 'A' && c <= 'Z') x = c - 'A';
            if (x < 0) continue;
            letters.push_back(x);
            ++counts[x];
            int n = (int)letters.size();
            if (n > k) --counts[letters[n-k-1]];
            if (n >= k && !seen.insert(counts).second) {
                answer = i;
                break;
            }
        }
        cout << answer << '\n';
    }
    return 0;
}
