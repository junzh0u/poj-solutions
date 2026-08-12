// POJ 2550 - Zipf's Law
// Model: gpt-5.6-terra
// Count maximal letter-only words in each line-delimited text case with an
// ordered map.  The sentinel is recognized only when it occupies an entire
// line, because the token may also occur as part of ordinary text.

#include <cctype>
#include <iostream>
#include <map>
#include <string>

using namespace std;

int main() {
    int wanted;
    int caseNumber = 0;
    string line;

    while (cin >> wanted) {
        getline(cin, line);
        map<string, int> count;

        while (getline(cin, line)) {
            if (!line.empty() && line[line.size() - 1] == '\r') {
                line.erase(line.size() - 1);
            }
            if (line == "EndOfText") {
                break;
            }

            string word;
            for (string::size_type i = 0; i <= line.size(); ++i) {
                unsigned char ch = i == line.size() ? 0 : static_cast<unsigned char>(line[i]);
                if (isalpha(ch)) {
                    word += static_cast<char>(tolower(ch));
                } else if (!word.empty()) {
                    ++count[word];
                    word.clear();
                }
            }
        }

        if (caseNumber++ != 0) {
            cout << '\n';
        }
        bool found = false;
        for (map<string, int>::const_iterator it = count.begin(); it != count.end(); ++it) {
            if (it->second == wanted) {
                cout << it->first << '\n';
                found = true;
            }
        }
        if (!found) {
            cout << "There is no such word.\n";
        }
    }
    return 0;
}
