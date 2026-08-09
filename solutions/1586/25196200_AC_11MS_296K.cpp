// POJ 1586 - Three Sides Make a Triangle
// Model: claude-sonnet-5
// Approach: parse each coordinate (given to the nearest 0.001) into an exact fixed-point
// integer (value * 1000) so squared side lengths are exact 64-bit integers with zero
// floating-point noise from the input itself. Degenerate/collinear detection uses an exact
// integer cross product (safe: any real data-entry mistake gives an exactly-zero cross
// product at this precision). Side-length equality (Scalene/Isosceles/Equilateral) and the
// right-angle test (Acute/Right/Obtuse), however, need tolerances: the sample's "equilateral"
// triangle (0,0)-(4,0)-(2,3.464) has side lengths 4.000 and 3.999912 after rounding to 0.001,
// a ~0.00009 discrepancy from rounding the true sqrt(3) apex point, yet must be judged equal.
// The statement guarantees no two *intentionally unequal* sides are within 0.01 of each other,
// and no non-right angle falls within 88-92 degrees, so an epsilon of 0.005 (length, absolute,
// scale-independent since rounding noise is an absolute quantity) and 0.02 (on the cosine of
// the angle opposite the longest side, so it is scale-invariant) sit safely between the
// rounding noise (~1e-4) and the guaranteed gaps (0.01 length / ~0.035 = cos(88 deg) on cosine).
// Discuss board note: the terminating "-1" must be the ONLY token on its line -- "-1" can
// legitimately appear as one of the six coordinates on a real triangle line, so termination is
// detected by tokenizing the line, not by scanning for the value -1 among parsed numbers.
#include <cstdio>
#include <cctype>
#include <cmath>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
using namespace std;

typedef long long ll;

static ll parseFixed(const string &tok) {
    size_t i = 0;
    bool neg = false;
    if (i < tok.size() && (tok[i] == '+' || tok[i] == '-')) {
        neg = (tok[i] == '-');
        i++;
    }
    ll intPart = 0;
    while (i < tok.size() && isdigit((unsigned char)tok[i])) {
        intPart = intPart * 10 + (tok[i] - '0');
        i++;
    }
    ll frac = 0;
    int fracDigits = 0;
    if (i < tok.size() && tok[i] == '.') {
        i++;
        while (i < tok.size() && isdigit((unsigned char)tok[i]) && fracDigits < 3) {
            frac = frac * 10 + (tok[i] - '0');
            fracDigits++;
            i++;
        }
    }
    while (fracDigits < 3) { frac *= 10; fracDigits++; }
    ll val = intPart * 1000 + frac;
    return neg ? -val : val;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    string line;
    const double EPS_LEN = 0.005;   // real length units, guaranteed gap is 0.01
    const double EPS_COS = 0.02;    // cos(88 deg) ~= 0.0349, rounding noise ~1e-4

    while (getline(cin, line)) {
        while (!line.empty() && (line[line.size() - 1] == '\r' || line[line.size() - 1] == '\n'))
            line.erase(line.size() - 1);

        istringstream iss(line);
        vector<string> toks;
        string tok;
        while (iss >> tok) toks.push_back(tok);

        if (toks.empty()) continue;
        if (toks.size() == 1 && toks[0] == "-1") break;
        if (toks.size() < 6) continue; // defensive: malformed line

        ll x1 = parseFixed(toks[0]), y1 = parseFixed(toks[1]);
        ll x2 = parseFixed(toks[2]), y2 = parseFixed(toks[3]);
        ll x3 = parseFixed(toks[4]), y3 = parseFixed(toks[5]);

        ll cross = (x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1);
        if (cross == 0) {
            cout << "Not a Triangle\n";
            continue;
        }

        ll a2 = (x2 - x3) * (x2 - x3) + (y2 - y3) * (y2 - y3); // side P2P3, opposite P1
        ll b2 = (x1 - x3) * (x1 - x3) + (y1 - y3) * (y1 - y3); // side P1P3, opposite P2
        ll c2 = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2); // side P1P2, opposite P3

        double la = sqrt((double)a2) / 1000.0;
        double lb = sqrt((double)b2) / 1000.0;
        double lc = sqrt((double)c2) / 1000.0;

        bool eqAB = fabs(la - lb) < EPS_LEN;
        bool eqBC = fabs(lb - lc) < EPS_LEN;
        bool eqAC = fabs(la - lc) < EPS_LEN;

        const char *lengthClass;
        if (eqAB && eqBC && eqAC) lengthClass = "Equilateral";
        else if (eqAB || eqBC || eqAC) lengthClass = "Isosceles";
        else lengthClass = "Scalene";

        // angle: only the angle opposite the longest side can be >= 90 degrees
        ll mx = a2, other1 = b2, other2 = c2;
        if (b2 > mx) { mx = b2; other1 = a2; other2 = c2; }
        if (c2 > mx) { mx = c2; other1 = a2; other2 = b2; }

        double cosVal = ((double)other1 + (double)other2 - (double)mx) /
                         (2.0 * sqrt((double)other1) * sqrt((double)other2));

        const char *angleClass;
        if (fabs(cosVal) < EPS_COS) angleClass = "Right";
        else if (cosVal > 0) angleClass = "Acute";
        else angleClass = "Obtuse";

        cout << lengthClass << " " << angleClass << "\n";
    }

    cout << "End of Output\n";
    return 0;
}
