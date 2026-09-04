// POJ 2691 - Request for Proposal
// Model: claude-sonnet-5
//
// For each RFP we need the proposal with the highest compliance
// (met_requirements / total_requirements), breaking ties by lowest
// price, then by earliest occurrence in the input. A fully compliant
// proposal has compliance == 1.0, which is the maximum possible value
// of the ratio, so the "pick highest compliance" rule already reduces
// to "prefer a fully compliant proposal" without any special case.
// Compliance fractions are compared by cross multiplication (r1*n2
// vs r2*n1) to avoid floating point issues; n and r are small integers
// (n <= 1000) so the cross products fit comfortably in a 32-bit int.
//
// The requirement names themselves (both the RFP's own list and the
// per-proposal "met requirements" list) never need to be stored or
// matched against anything -- only their counts matter -- so those
// lines are simply consumed. Proposal and requirement names may
// contain embedded spaces (confirmed on the discuss board), so every
// name is read with a full line read (getline), never with %s /
// operator>>.
#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

static string readLine() {
    string s;
    if (!getline(cin, s)) return s;
    if (!s.empty() && s[s.size() - 1] == '\r') s.erase(s.size() - 1);
    return s;
}

int main() {
    ios::sync_with_stdio(false);
    int n, p;
    int rfpNum = 0;
    bool firstOutput = true;
    // First line of the whole input is numeric; read it with the
    // stream, then consume the rest of that line before switching to
    // getline-based line reads for the string data.
    while (cin >> n >> p) {
        string dummy;
        getline(cin, dummy); // consume rest of "n p" line
        if (n == 0 && p == 0) break;
        rfpNum++;

        for (int i = 0; i < n; i++) {
            readLine(); // requirement name, unused
        }

        string bestName;
        int bestR = -1, bestN = n; // compliance = bestR / bestN
        double bestPrice = 0.0;
        bool have = false;

        for (int j = 0; j < p; j++) {
            string name = readLine();
            string line2 = readLine();
            istringstream iss(line2);
            double price;
            int r;
            iss >> price >> r;
            for (int k = 0; k < r; k++) {
                readLine(); // met requirement name, unused
            }

            bool better = false;
            if (!have) {
                better = true;
            } else {
                // compare r/n vs bestR/bestN (r,n <= 1000, so the
                // cross products fit well within a 32-bit int)
                int lhs = r * bestN;
                int rhs = bestR * n;
                if (lhs > rhs) {
                    better = true;
                } else if (lhs == rhs) {
                    if (price < bestPrice - 1e-9) {
                        better = true;
                    }
                    // equal compliance and equal (or higher) price ->
                    // keep the earlier one, so 'better' stays false
                }
            }

            if (better) {
                have = true;
                bestName = name;
                bestR = r;
                bestN = n;
                bestPrice = price;
            }
        }

        if (!firstOutput) {
            cout << "\n";
        }
        firstOutput = false;
        cout << "RFP #" << rfpNum << "\n" << bestName << "\n";
    }
    return 0;
}
