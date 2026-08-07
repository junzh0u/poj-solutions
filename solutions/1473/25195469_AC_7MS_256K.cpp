// POJ 1473 - There's Treasure Everywhere!
// Model: claude-sonnet-5
// Parse each line as a comma-separated list of <length><direction> pairs
// (direction one of N,NE,E,SE,S,SW,W,NW) terminated by '.'. Sum unit-vector
// displacements (diagonals contribute length/sqrt(2) to each axis) to get
// the final (x,y), print it and the Euclidean distance to 3 decimals.
// A tiny negative value that rounds to 0.000 prints as "-0.000" under
// naive printf; the discuss board flags exactly this (and a sqrt(int)
// truncation pitfall), so any coordinate/distance within 5e-4 of zero on
// the negative side is clamped to +0.0 before printing.
#include <cstdio>
#include <cstring>
#include <cctype>
#include <cmath>
#include <string>
#include <iostream>

using namespace std;

static double fixZero(double v) {
    if (v < 0 && v > -0.0005) return 0.0;
    return v;
}

int main() {
    const double INV_SQRT2 = 1.0 / sqrt(2.0);
    string line;
    int mapNo = 0;
    while (getline(cin, line)) {
        // strip trailing \r
        while (!line.empty() && (line[line.size()-1] == '\r' || line[line.size()-1] == '\n'))
            line.erase(line.size()-1);
        if (line.empty()) continue;
        if (line == "END") break;
        // strip trailing '.'
        if (!line.empty() && line[line.size()-1] == '.')
            line.erase(line.size()-1);

        mapNo++;
        double x = 0.0, y = 0.0;
        size_t i = 0, n = line.size();
        while (i < n) {
            // parse number
            size_t j = i;
            long len = 0;
            while (j < n && isdigit((unsigned char)line[j])) {
                len = len * 10 + (line[j] - '0');
                j++;
            }
            // parse direction letters until comma or end
            size_t k = j;
            while (k < n && line[k] != ',') k++;
            string dir = line.substr(j, k - j);

            double dx = 0.0, dy = 0.0;
            double L = (double)len;
            if (dir == "N") { dx = 0.0; dy = L; }
            else if (dir == "S") { dx = 0.0; dy = -L; }
            else if (dir == "E") { dx = L; dy = 0.0; }
            else if (dir == "W") { dx = -L; dy = 0.0; }
            else if (dir == "NE") { dx = L * INV_SQRT2; dy = L * INV_SQRT2; }
            else if (dir == "NW") { dx = -L * INV_SQRT2; dy = L * INV_SQRT2; }
            else if (dir == "SE") { dx = L * INV_SQRT2; dy = -L * INV_SQRT2; }
            else if (dir == "SW") { dx = -L * INV_SQRT2; dy = -L * INV_SQRT2; }

            x += dx;
            y += dy;

            i = (k < n) ? k + 1 : k;
        }

        double dist = sqrt(x * x + y * y);
        x = fixZero(x);
        y = fixZero(y);
        dist = fixZero(dist);

        printf("Map #%d\n", mapNo);
        printf("The treasure is located at (%.3f,%.3f).\n", x, y);
        printf("The distance to the treasure is %.3f.\n", dist);
        printf("\n");
    }
    return 0;
}
