// POJ 2582 - Finding Nemo
// Model: claude-sonnet-5
// Approach: Each dynamite stick dropped at (x,y) sinks at constant speed
// while its fuse burns at the same rate, so it detonates at depth
// z = min(f, D) (it stops sinking at the bottom but keeps burning there).
// A fish at integer coordinates is killed if its squared Euclidean
// distance to any explosion point is <= 1; since all coordinates are
// integers this stays exact in integer arithmetic (no floating point
// or sqrt needed). Read the input line-by-line rather than token-by-token,
// since a dynamite entry "x,y,f" and a fish entry "x,y,z" are
// syntactically identical (three comma-separated ints) and are
// distinguished only by which physical line they occupy.
// No statement ambiguity found; verified against both sample cases by hand.
#include <cstdio>
#include <cstring>
#include <cctype>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
using namespace std;

static string rstrip(const string& s) {
    size_t end = s.size();
    while (end > 0 && (s[end-1] == '\r' || s[end-1] == '\n' || isspace((unsigned char)s[end-1]))) end--;
    return s.substr(0, end);
}

static bool readLine(string& line) {
    while (getline(cin, line)) {
        line = rstrip(line);
        if (!line.empty()) return true;
        // skip blank lines defensively, though statement says none occur
    }
    return false;
}

struct Triple { int a, b, c; };

static vector<Triple> parseTriples(const string& line) {
    vector<Triple> res;
    istringstream iss(line);
    string tok;
    while (iss >> tok) {
        // replace commas with spaces
        for (size_t i = 0; i < tok.size(); i++) if (tok[i] == ',') tok[i] = ' ';
        istringstream ts(tok);
        Triple t;
        ts >> t.a >> t.b >> t.c;
        res.push_back(t);
    }
    return res;
}

int main() {
    string line;
    while (readLine(line)) {
        if (line == "ENDOFINPUT") break;
        int L, W, D;
        // line should be "START L W D"
        {
            istringstream iss(line);
            string kw;
            iss >> kw >> L >> W >> D;
        }
        string dynLine, fishLine, endLine;
        readLine(dynLine);
        readLine(fishLine);
        readLine(endLine); // should be "END"

        vector<Triple> dyn = parseTriples(dynLine);
        vector<Triple> fish = parseTriples(fishLine);

        vector<Triple> explosions;
        for (size_t i = 0; i < dyn.size(); i++) {
            int x = dyn[i].a, y = dyn[i].b, f = dyn[i].c;
            int z = f < D ? f : D;
            Triple t; t.a = x; t.b = y; t.c = z;
            explosions.push_back(t);
        }

        int killed = 0;
        for (size_t i = 0; i < fish.size(); i++) {
            int fx = fish[i].a, fy = fish[i].b, fz = fish[i].c;
            bool dead = false;
            for (size_t j = 0; j < explosions.size(); j++) {
                int dx = fx - explosions[j].a;
                int dy = fy - explosions[j].b;
                int dz = fz - explosions[j].c;
                int d2 = dx*dx + dy*dy + dz*dz;
                if (d2 <= 1) { dead = true; break; }
            }
            if (dead) killed++;
        }

        if (killed > 0) {
            printf("AIEE, I got %d fish, me!\n", killed);
        } else {
            printf("None of dem fish blowed up!\n");
        }
    }
    return 0;
}
