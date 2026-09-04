// POJ 2321 - GYM
// Model: claude-sonnet-5
// Approach: Simulate a discrete-time Markov chain. Each basket's index-card
// counts define a row of an (unnormalized) transition matrix; dividing each
// row by its sum gives the probability of moving from basket i to basket j.
// Starting from P0 = (1,0,...,0) at the first basket, iterate P_{s+1} = P_s * T
// for s = 0..9, printing all ten probability vectors P0..P9 to 5 decimals.
//
// Statement ambiguity: the Input section never says N is given on its own
// line, and the sample shows only the N x N count matrix with no leading N
// and no case count. The discuss board (message 148824/148826/168431)
// confirms the real judge data holds multiple datasets to EOF and N must be
// inferred: each dataset's first line has exactly N whitespace-separated
// integers (since the matrix is square), so N is recovered by counting the
// tokens on that first line, then the remaining N-1 rows (each N numbers)
// are read with the now-known N. Blank lines (including the leftover empty
// line after operator>> stops mid-line) are skipped when looking for the
// next dataset's first line.
#include <cstdio>
#include <cctype>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

int main() {
    string line;
    while (getline(cin, line)) {
        bool blank = true;
        for (size_t i = 0; i < line.size(); ++i) {
            if (!isspace(static_cast<unsigned char>(line[i]))) { blank = false; break; }
        }
        if (blank) continue;

        double mat[15][15];
        int n = 0;
        {
            istringstream iss(line);
            double x;
            while (iss >> x) { mat[0][n++] = x; }
        }
        for (int i = 1; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                cin >> mat[i][j];
            }
        }

        double trans[15][15];
        for (int i = 0; i < n; ++i) {
            double sum = 0.0;
            for (int j = 0; j < n; ++j) sum += mat[i][j];
            for (int j = 0; j < n; ++j) trans[i][j] = mat[i][j] / sum;
        }

        double P[15], nextP[15];
        for (int i = 0; i < n; ++i) P[i] = 0.0;
        P[0] = 1.0;

        for (int step = 0; step < 10; ++step) {
            for (int i = 0; i < n; ++i) {
                printf("%.5f%c", P[i], (i + 1 < n) ? ' ' : '\n');
            }
            for (int j = 0; j < n; ++j) {
                double s = 0.0;
                for (int i = 0; i < n; ++i) s += P[i] * trans[i][j];
                nextP[j] = s;
            }
            for (int i = 0; i < n; ++i) P[i] = nextP[i];
        }
    }
    return 0;
}
