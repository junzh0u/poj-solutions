// POJ 2713 - Tire Dimensions
// Model: claude-sonnet-5
// Approach: each input line is a tire descriptor "TYPE WIDTH / RATIO [SPEED]
// CONSTRUCTION RIM" tokenized by single spaces. Regardless of whether the
// optional one-letter speed symbol is present, the token positions we need
// are fixed: token[1] is the section width (mm), token[3] is the aspect
// ratio (%), and the LAST token is always the nominal rim diameter (inches),
// since type/speed/construction letters never carry numeric value we need.
// Overall diameter = 2*(width*ratio/100) + rim_diameter_in*25.4 (mm).
// Circumference = pi * overall diameter, converted mm->cm, rounded to the
// nearest integer. No statement ambiguity found; discuss board (checked,
// message ids 37739/37743/37745/90349/155389/342181) only had unrelated
// chatter calling the problem trivial, nothing about input framing.
#include <cstdio>
#include <cmath>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
using namespace std;

int main() {
    string line;
    while (getline(cin, line)) {
        // strip trailing CR (in case of CRLF input) and trailing whitespace
        while (!line.empty() && (line[line.size()-1] == '\r' || line[line.size()-1] == '\n'))
            line.erase(line.size()-1);
        // skip blank lines (e.g. trailing blank line at EOF)
        bool hasContent = false;
        for (size_t i = 0; i < line.size(); ++i) {
            if (!isspace((unsigned char)line[i])) { hasContent = true; break; }
        }
        if (!hasContent) continue;

        istringstream iss(line);
        vector<string> tok;
        string t;
        while (iss >> t) tok.push_back(t);
        if (tok.size() < 4) continue; // malformed, shouldn't happen

        double width = atof(tok[1].c_str());
        double ratio = atof(tok[3].c_str());
        double rimIn = atof(tok.back().c_str());

        double heightMm = width * ratio / 100.0;
        double rimMm = rimIn * 25.4;
        double overallDiameterMm = rimMm + 2.0 * heightMm;
        double circumferenceMm = M_PI * overallDiameterMm;
        double circumferenceCm = circumferenceMm / 10.0;
        long rounded = (long)floor(circumferenceCm + 0.5);

        cout << line << ": " << rounded << "\n";
    }
    return 0;
}
