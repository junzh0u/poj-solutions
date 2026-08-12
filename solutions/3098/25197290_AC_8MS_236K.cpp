// POJ 3098 - Frugal Search
// Model: gpt-5.6-terra
// Sort the dictionary, then scan it for each query.  A query is an OR of
// terms; each term requires an unsigned letter, all + letters, and no -
// letters.  The first matching sorted word is the requested answer.
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

bool matchesTerm(const string& word, const string& term) {
    bool have[26] = {false};
    int i;
    for (i = 0; i < (int)word.size(); ++i) have[word[i] - 'a'] = true;
    bool unsignedMatch = false;
    for (i = 0; i < (int)term.size(); ++i) {
        if (term[i] == '+') {
            ++i;
            if (!have[term[i] - 'a']) return false;
        } else if (term[i] == '-') {
            ++i;
            if (have[term[i] - 'a']) return false;
        } else if (have[term[i] - 'a']) {
            unsignedMatch = true;
        }
    }
    return unsignedMatch;
}

bool matchesQuery(const string& word, const string& query) {
    int start = 0;
    int i;
    for (i = 0; i <= (int)query.size(); ++i) {
        if (i == (int)query.size() || query[i] == '|') {
            if (matchesTerm(word, query.substr(start, i - start))) return true;
            start = i + 1;
        }
    }
    return false;
}

int main() {
    string line;
    while (cin >> line && line != "#") {
        vector<string> words;
        while (line != "*") {
            words.push_back(line);
            cin >> line;
        }
        sort(words.begin(), words.end());
        while (cin >> line && line != "**") {
            string answer = "NONE";
            int i;
            for (i = 0; i < (int)words.size(); ++i) {
                if (matchesQuery(words[i], line)) {
                    answer = words[i];
                    break;
                }
            }
            cout << answer << '\n';
        }
        cout << "$\n";
    }
    return 0;
}
