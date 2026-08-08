// POJ 3785 - The Next Permutation
// Model: claude-opus-5
//
// Each data set is "<label> <digits>" with up to 80 decimal digits.  Emit the
// permutation of those digits with the next larger decimal value, or BIGGEST.
//
// Since every permutation is a string of the same length, comparing the digit
// strings lexicographically is exactly comparing the numbers, so the answer is
// the lexicographic successor of the digit multiset arrangement -- plain
// std::next_permutation, which returns false precisely on the largest
// arrangement (the BIGGEST case).  No arithmetic, so 80 digits costs nothing.
//
// Input framing: the first line's count is read and then ignored; data sets are
// consumed to EOF, and blank lines are skipped, so a miscounted or padded input
// file cannot desynchronise the labels.  Each line is split on whitespace after
// trailing CR/blanks are stripped, so CRLF data is handled.

#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

int main() {
    string line;
    bool first = true;
    while (getline(cin, line)) {
        while (!line.empty() && (line[line.size() - 1] == '\r' ||
                                 line[line.size() - 1] == '\n' ||
                                 line[line.size() - 1] == ' ' ||
                                 line[line.size() - 1] == '\t'))
            line.erase(line.size() - 1);
        if (line.empty()) continue;
        if (first) { first = false; continue; }  // the count P
        istringstream ss(line);
        string label, digits;
        if (!(ss >> label)) continue;
        ss >> digits;
        if (next_permutation(digits.begin(), digits.end()))
            cout << label << ' ' << digits << '\n';
        else
            cout << label << ' ' << "BIGGEST" << '\n';
    }
    return 0;
}
