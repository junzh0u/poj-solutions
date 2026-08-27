// POJ 1420 - Spreadsheet
// Model: gpt-5.6-terra
// Approach: Parse every formula into references and evaluate the acyclic
// dependency graph with memoized depth-first search. Column labels are decoded
// as base-26 numbers, so labels through ZZZ are handled directly.
// The input's leading spreadsheet count is honored exactly.
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

class Sheet {
    int columns;
    vector<string> cell;
    vector<long> value;
    vector<int> done;

    int referenceIndex(const string& name) const {
        int p = 0;
        int col = 0;
        while (p < (int)name.size() && name[p] >= 'A' && name[p] <= 'Z') {
            col = col * 26 + (name[p] - 'A' + 1);
            ++p;
        }
        int row = 0;
        while (p < (int)name.size()) {
            row = row * 10 + (name[p] - '0');
            ++p;
        }
        return (row - 1) * columns + (col - 1);
    }

    long evaluate(int at) {
        if (done[at]) return value[at];
        done[at] = 1;
        if (cell[at][0] != '=') {
            istringstream in(cell[at]);
            in >> value[at];
            return value[at];
        }
        long sum = 0;
        int begin = 1;
        int i;
        for (i = 1; i <= (int)cell[at].size(); ++i) {
            if (i == (int)cell[at].size() || cell[at][i] == '+') {
                sum += evaluate(referenceIndex(cell[at].substr(begin, i - begin)));
                begin = i + 1;
            }
        }
        value[at] = sum;
        return value[at];
    }

public:
    Sheet(int c, int r) : columns(c), cell(c * r), value(c * r), done(c * r, 0) {}

    void read() {
        int i;
        for (i = 0; i < (int)cell.size(); ++i) cin >> cell[i];
    }

    void print() {
        int i;
        for (i = 0; i < (int)cell.size(); ++i) {
            if (i % columns) cout << ' ';
            cout << evaluate(i);
            if (i % columns == columns - 1) cout << '\n';
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    int tests;
    if (!(cin >> tests)) return 0;
    while (tests--) {
        int columns, rows;
        cin >> columns >> rows;
        Sheet sheet(columns, rows);
        sheet.read();
        sheet.print();
    }
    return 0;
}
