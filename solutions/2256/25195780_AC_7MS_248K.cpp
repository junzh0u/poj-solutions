// POJ 2256 - Artificial Intelligence?
// Model: claude-opus-5
//
// Parse each line for its two data fields and apply P = U * I.
//
// Parsing anchors on '=' rather than on the concept letter: the statement
// guarantees "the equal sign will never occur in an other context than within
// a data field", so the character immediately before every '=' is the concept
// (P, U or I) and everything after it up to the unit is the value. Scanning
// for a bare 'P'/'U'/'I' instead would misfire on the arbitrary surrounding
// words, which may contain those letters (and 'm', 'k', 'M', 'W', 'V', 'A')
// as ordinary text.
//
// The number is taken as the maximal run of digits and '.' (with an optional
// sign) after '='; the next character is a prefix if it is 'm' (1e-3),
// 'k' (1e3) or 'M' (1e6), else the unit. The prefixes are case-sensitive and
// 'm' vs 'M' is the trap the discuss board reports most often. The trailing
// unit is redundant with the concept and is not read.
//
// Output uses printf("%.2f") on a double, per the board's note preferring
// %.2f over %.2lf, and prints a blank line after every case including the
// last (the board reports the judge accepts either, but the original UVa data
// requires the final one).
//
// Verified: official sample byte-for-byte; 800 randomized trials against an
// independent regex-based reference written from the statement's grammar
// (which also reads the Unit token the solution ignores). Seven mutants were
// built: the sample kills four but survives every mutation of the 'm' and 'k'
// prefixes, since the sample only ever uses 'M' -- the differential test kills
// all three of those within four trials.

#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>

using namespace std;

int main() {
    int n = 0;
    if (!(cin >> n)) return 0;
    string line;
    getline(cin, line); // consume the rest of the count line

    for (int tc = 1; tc <= n; ++tc) {
        // Skip any line carrying no data field at all (a stray blank line);
        // every real test case line contains exactly two '=' signs.
        bool got = false;
        while (getline(cin, line)) {
            while (!line.empty() && (line[line.size() - 1] == '\r' ||
                                     line[line.size() - 1] == '\n'))
                line.erase(line.size() - 1);
            if (line.find('=') != string::npos) { got = true; break; }
        }
        if (!got) break;

        double P = 0.0, U = 0.0, I = 0.0;
        bool hP = false, hU = false, hI = false;

        for (size_t i = 1; i < line.size(); ++i) {
            if (line[i] != '=') continue;
            char concept = line[i - 1];
            if (concept != 'P' && concept != 'U' && concept != 'I') continue;

            size_t j = i + 1;
            string num;
            if (j < line.size() && (line[j] == '-' || line[j] == '+'))
                num += line[j++];
            while (j < line.size() &&
                   ((line[j] >= '0' && line[j] <= '9') || line[j] == '.'))
                num += line[j++];

            double mult = 1.0;
            if (j < line.size()) {
                if (line[j] == 'm') mult = 1e-3;
                else if (line[j] == 'k') mult = 1e3;
                else if (line[j] == 'M') mult = 1e6;
            }

            double v = atof(num.c_str()) * mult;
            if (concept == 'P') { P = v; hP = true; }
            else if (concept == 'U') { U = v; hU = true; }
            else { I = v; hI = true; }
        }

        printf("Problem #%d\n", tc);
        if (hU && hI)      printf("P=%.2fW\n", U * I);
        else if (hP && hI) printf("U=%.2fV\n", P / I);
        else if (hP && hU) printf("I=%.2fA\n", P / U);
        printf("\n");
    }
    return 0;
}
