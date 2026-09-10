// POJ 1370 - Gossiping
// Model: claude-sonnet-5
//
// Each of the n bus lines is a fixed cycle of stops; buses on a line all
// travel it in lockstep (same speed, same direction), just possibly starting
// at different stops. Two drivers ever meet iff their buses are ever at the
// same physical stop on the same tick. Because the whole system is
// periodic, a meeting that happens once recurs every period, so for any
// chain of pairwise meetings we can always re-time later links to occur
// after earlier ones (just wait enough extra periods) - meaning eventual
// information flow along a path of "ever meet" edges always succeeds
// regardless of the order the meetings were first observed in. So the
// answer is: build a graph on the d drivers, connect two drivers if their
// buses ever coincide at a stop, and answer Yes iff that graph is
// connected (confirmed by the problem's own discuss board, message
// 359860: pairwise-match all buses, union on meeting, check for one
// component).
//
// For one pair of drivers on lines of length La, Lb, positions repeat with
// period lcm(La, Lb) <= 49*49, so checking a single pair only requires
// scanning that many ticks - no need to touch the (possibly astronomical)
// LCM of all n line lengths together.
//
// Input is line-oriented: each bus line's stop sequence and its start-stop
// list are each one physical line with a variable token count, so parsing
// must be done per getline rather than with free-form operator>>.

#include <cstdio>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>

using namespace std;

static vector<int> parent;

int find(int x) {
    while (parent[x] != x) {
        parent[x] = parent[parent[x]];
        x = parent[x];
    }
    return x;
}

void unite(int a, int b) {
    a = find(a);
    b = find(b);
    if (a != b) parent[a] = b;
}

int gcdInt(int a, int b) {
    while (b) {
        int t = a % b;
        a = b;
        b = t;
    }
    return a;
}

// Read exactly the next physical line from stdin, splitting into ints.
// A bus line can legitimately have zero buses assigned to it, in which
// case its "start stops" line is blank - so blank lines must NOT be
// skipped here, only genuine end-of-file should stop us.
bool nextIntLine(vector<int>& out) {
    string line;
    if (!std::getline(cin, line)) return false;
    out.clear();
    istringstream iss(line);
    int v;
    while (iss >> v) out.push_back(v);
    return true;
}

// Header (n d s) line: unlike an assigns line this must be non-blank, but
// skip over any accidental blank lines before it defensively.
bool nextHeaderLine(vector<int>& out) {
    while (true) {
        if (!nextIntLine(out)) return false;
        if (!out.empty()) return true;
    }
}

int main() {
    vector<int> header;
    while (nextHeaderLine(header)) {
        if (header.size() < 3) continue; // defensive
        int n = header[0];
        int d = header[1];
        int s = header[2];
        (void)s;
        if (n == 0 && d == 0 && header[2] == 0) break;

        vector< vector<int> > lineSeq(n);
        vector<int> driverLine(d + 1, -1);
        vector<int> driverOffset(d + 1, -1);

        for (int i = 0; i < n; ++i) {
            vector<int> seq;
            nextIntLine(seq);
            lineSeq[i] = seq;

            vector<int> pairs;
            nextIntLine(pairs);
            for (size_t k = 0; k + 1 < pairs.size(); k += 2) {
                int stopNo = pairs[k];
                int drv = pairs[k + 1];
                // find index of stopNo in seq
                int idx = 0; // defensive default; the problem guarantees
                             // stopNo is always a stop on this very line
                for (size_t j = 0; j < seq.size(); ++j) {
                    if (seq[j] == stopNo) { idx = (int)j; break; }
                }
                driverLine[drv] = i;
                driverOffset[drv] = idx;
            }
        }

        parent.assign(d + 1, 0);
        for (int i = 1; i <= d; ++i) parent[i] = i;

        for (int u = 1; u <= d; ++u) {
            for (int v = u + 1; v <= d; ++v) {
                if (find(u) == find(v)) continue;
                int lu = driverLine[u], lv = driverLine[v];
                int ou = driverOffset[u], ov = driverOffset[v];
                int La = (int)lineSeq[lu].size();
                int Lb = (int)lineSeq[lv].size();
                int g = La / gcdInt(La, Lb) * Lb;
                for (int t = 0; t < g; ++t) {
                    int posU = lineSeq[lu][(ou + t) % La];
                    int posV = lineSeq[lv][(ov + t) % Lb];
                    if (posU == posV) {
                        unite(u, v);
                        break;
                    }
                }
            }
        }

        bool connected = true;
        if (d >= 1) {
            int root = find(1);
            for (int i = 2; i <= d; ++i) {
                if (find(i) != root) { connected = false; break; }
            }
        }
        printf("%s\n", connected ? "Yes" : "No");
    }
    return 0;
}
