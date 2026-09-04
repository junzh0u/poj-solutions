// POJ 2326 - Major Scales
// Model: claude-sonnet-5
// Approach: for each of the 12 chromatic keys, precompute the 7-note pitch-class
// set of its major scale (root offsets 0,2,4,5,7,9,11 mod 12, matching the
// tone-tone-semi-tone-tone-tone-tone-semi-tone pattern). For each input line of
// notes, a key qualifies iff every note's pitch class lies in that key's set.
// Output the qualifying keys, in chromatic order, space separated (a blank
// line when none qualify). Read lines until a line equal to "END" (tokenized
// with istringstream, robust to trailing whitespace/CR).
#include <cstdio>
#include <cstring>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

int main() {
    const char *names[12] = {"C","C#","D","D#","E","F","F#","G","G#","A","A#","B"};
    int noteIndex[256];
    // build lookup by string compare since names have 1-2 chars
    string line;
    // precompute scale sets: scaleHas[root][pitchClass]
    int offsets[7] = {0,2,4,5,7,9,11};
    bool scaleHas[12][12];
    memset(scaleHas, 0, sizeof(scaleHas));
    for (int r = 0; r < 12; r++) {
        for (int k = 0; k < 7; k++) {
            int pc = (r + offsets[k]) % 12;
            scaleHas[r][pc] = true;
        }
    }
    (void)noteIndex;

    while (getline(cin, line)) {
        // strip trailing \r
        while (!line.empty() && (line[line.size()-1] == '\r' || line[line.size()-1] == '\n'))
            line.erase(line.size()-1);
        // check if line is exactly "END" (trim leading/trailing spaces)
        {
            size_t a = line.find_first_not_of(" \t");
            size_t b = line.find_last_not_of(" \t");
            string trimmed = (a == string::npos) ? "" : line.substr(a, b - a + 1);
            if (trimmed == "END") break;
        }
        istringstream iss(line);
        string tok;
        bool possible[12];
        for (int i = 0; i < 12; i++) possible[i] = true;
        while (iss >> tok) {
            int pc = -1;
            for (int i = 0; i < 12; i++) {
                if (tok == names[i]) { pc = i; break; }
            }
            if (pc < 0) continue; // shouldn't happen with valid input
            for (int r = 0; r < 12; r++) {
                if (possible[r] && !scaleHas[r][pc]) possible[r] = false;
            }
        }
        bool first = true;
        for (int r = 0; r < 12; r++) {
            if (possible[r]) {
                if (!first) printf(" ");
                printf("%s", names[r]);
                first = false;
            }
        }
        printf("\n");
    }
    return 0;
}
