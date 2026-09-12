// POJ 2347 - Equations
// Model: claude-sonnet-5
//
// Parse two linear equations in x and y into the form A*x + B*y = C using
// exact integer arithmetic, then solve the 2x2 system.  When the system's
// determinant is non-zero the solution is unique via Cramer's rule.  When
// the determinant is zero the two "direction" vectors (A,B) are parallel
// (or degenerate), so we must reason about whether x and/or y are still
// pinned down: a raw contradiction (0*x+0*y = nonzero) in either equation
// makes the whole system unsatisfiable (print "don't know" for both,
// matching e.g. "1 = 2 / x = 3" from the sample and "x=2 / x=1" from the
// board); otherwise consistency is checked via cross products, and if
// consistent we fall back to whichever equation is non-vacuous (has
// (A,B) != (0,0)) to see whether it pins x (B==0), y (A==0), neither
// (both nonzero -> a full line, both unknown), or -- if both equations
// are entirely vacuous -- nothing at all.
//
// Statement ambiguity: "no unique rational value such that both equations
// hold" was read (and confirmed against board test data, message 170020
// and 142490) to mean BOTH an inconsistent system (no solution at all)
// and an under-determined one (infinitely many solutions) print "don't
// know" for the affected variable -- there is no separate "no solution"
// output.
#include <cstdio>
#include <cctype>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
using namespace std;

typedef long long ll;

ll mygcd(ll a, ll b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b) { ll t = a % b; a = b; b = t; }
    return a;
}

// Parse a single equation line into A*x + B*y = C (as long longs).
void parseEquation(const string& lineIn, ll& A, ll& B, ll& C) {
    string line = lineIn;
    // strip stray CR
    while (!line.empty() && (line[line.size()-1] == '\r' || line[line.size()-1] == '\n'))
        line.erase(line.size()-1);

    istringstream iss(line);
    string tok;
    int side = 1;   // +1 before '=', -1 after
    int opSign = 1; // pending sign from a preceding + or - operator
    ll a = 0, b = 0, c = 0; // accumulate a*x + b*y + c = 0  (LHS - RHS)

    while (iss >> tok) {
        if (tok == "=") { side = -1; opSign = 1; continue; }
        if (tok == "+") { opSign = 1; continue; }
        if (tok == "-") { opSign = -1; continue; }

        // term
        size_t i = 0;
        bool neg = false;
        if (i < tok.size() && tok[i] == '-') { neg = true; i++; }
        else if (i < tok.size() && tok[i] == '+') { i++; } // defensive: tolerate a stray merged '+'
        string digits;
        while (i < tok.size() && isdigit((unsigned char)tok[i])) { digits += tok[i]; i++; }
        char var = 0;
        if (i < tok.size()) var = tok[i];

        ll coeff = digits.empty() ? 1 : atoll(digits.c_str());
        if (neg) coeff = -coeff;

        ll total = (ll)side * (ll)opSign * coeff;
        if (var == 'x') a += total;
        else if (var == 'y') b += total;
        else c += total;

        opSign = 1;
    }

    A = a; B = b; C = -c;
}

void reduceFrac(ll num, ll den, ll& rn, ll& rd) {
    if (den < 0) { den = -den; num = -num; }
    if (num == 0) { rn = 0; rd = 1; return; }
    ll g = mygcd(num, den);
    rn = num / g; rd = den / g;
}

void printVal(bool known, ll num, ll den) {
    if (!known) { printf("don't know\n"); return; }
    if (den == 1) printf("%lld\n", num);
    else printf("%lld/%lld\n", num, den);
}

int main() {
    int n;
    if (!(cin >> n)) return 0;
    cin.ignore(1000000, '\n');

    vector<string> lines;
    string line;
    while (getline(cin, line)) {
        // trim CR
        while (!line.empty() && (line[line.size()-1] == '\r')) line.erase(line.size()-1);
        // check if blank (only whitespace)
        bool blank = true;
        for (size_t i = 0; i < line.size(); i++) {
            if (!isspace((unsigned char)line[i])) { blank = false; break; }
        }
        if (blank) continue;
        lines.push_back(line);
    }

    size_t idx = 0;
    for (int tc = 0; tc < n; tc++) {
        string e1 = (idx < lines.size()) ? lines[idx++] : string("");
        string e2 = (idx < lines.size()) ? lines[idx++] : string("");

        ll A1, B1, C1, A2, B2, C2;
        parseEquation(e1, A1, B1, C1);
        parseEquation(e2, A2, B2, C2);

        ll D = A1 * B2 - A2 * B1;

        bool xKnown = false, yKnown = false;
        ll xn = 0, xd = 1, yn = 0, yd = 1;

        if (D != 0) {
            ll xnum = C1 * B2 - C2 * B1;
            ll ynum = A1 * C2 - A2 * C1;
            reduceFrac(xnum, D, xn, xd);
            reduceFrac(ynum, D, yn, yd);
            xKnown = yKnown = true;
        } else {
            bool contra1 = (A1 == 0 && B1 == 0 && C1 != 0);
            bool contra2 = (A2 == 0 && B2 == 0 && C2 != 0);
            if (contra1 || contra2) {
                xKnown = yKnown = false;
            } else {
                // consistency check via cross products
                bool consistent = (A1 * C2 == A2 * C1) && (B1 * C2 == B2 * C1);
                if (!consistent) {
                    xKnown = yKnown = false;
                } else {
                    ll Aeff, Beff, Ceff;
                    bool haveEff = false;
                    if (A1 != 0 || B1 != 0) { Aeff = A1; Beff = B1; Ceff = C1; haveEff = true; }
                    else if (A2 != 0 || B2 != 0) { Aeff = A2; Beff = B2; Ceff = C2; haveEff = true; }

                    if (!haveEff) {
                        xKnown = yKnown = false;
                    } else {
                        if (Beff == 0) {
                            reduceFrac(Ceff, Aeff, xn, xd);
                            xKnown = true;
                        }
                        if (Aeff == 0) {
                            reduceFrac(Ceff, Beff, yn, yd);
                            yKnown = true;
                        }
                    }
                }
            }
        }

        printVal(xKnown, xn, xd);
        printVal(yKnown, yn, yd);
        printf("\n");
    }
    return 0;
}
