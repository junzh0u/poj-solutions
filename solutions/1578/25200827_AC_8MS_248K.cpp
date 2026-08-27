// POJ 1578 - Instruens Fabulam
// Model: gpt-5.6-terra
// Parse each table up to the next format line, compute each column's widest
// entry, then render borders and cells with the requested alignment. A center
// with an odd spare space puts that space on the right.
#include <iostream>
#include <string>
#include <vector>

using namespace std;

static vector<string> split(const string& line) {
    vector<string> result;
    string current;
    size_t i;
    for (i = 0; i < line.size(); ++i) {
        if (line[i] == '&') {
            result.push_back(current);
            current.clear();
        } else {
            current += line[i];
        }
    }
    result.push_back(current);
    return result;
}

static void border(const vector<int>& width, char end, char middle) {
    int i;
    cout << end;
    for (i = 0; i < (int)width.size(); ++i) {
        cout << string(width[i], '-');
        cout << (i + 1 == (int)width.size() ? end : middle);
    }
    cout << '\n';
}

int main() {
    string format;
    if (!getline(cin, format)) return 0;
    while (format[0] != '*') {
        int columns = (int)format.size();
        vector< vector<string> > rows;
        vector<int> width(columns, 0);
        string line;
        int r, c;

        while (getline(cin, line) && line[0] != '<' && line[0] != '=' && line[0] != '>' && line[0] != '*') {
            vector<string> row = split(line);
            rows.push_back(row);
            for (c = 0; c < columns; ++c) {
                if ((int)row[c].size() > width[c]) width[c] = (int)row[c].size();
            }
        }
        for (c = 0; c < columns; ++c) width[c] += 2;

        border(width, '@', '-');
        for (r = 0; r < (int)rows.size(); ++r) {
            cout << '|';
            for (c = 0; c < columns; ++c) {
                int spare = width[c] - (int)rows[r][c].size();
                int left, right;
                if (format[c] == '<') {
                    left = 1;
                    right = spare - left;
                } else if (format[c] == '>') {
                    right = 1;
                    left = spare - right;
                } else {
                    left = spare / 2;
                    right = spare - left;
                }
                cout << string(left, ' ') << rows[r][c] << string(right, ' ') << '|';
            }
            cout << '\n';
            if (r == 0) border(width, '|', '+');
        }
        border(width, '@', '-');
        if (line.empty()) break;
        format = line;
    }
    return 0;
}
