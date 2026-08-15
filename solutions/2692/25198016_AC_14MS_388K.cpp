// POJ 2692 - Australian Voting
// Model: claude-sonnet-5
//
// Instant-runoff voting simulation. Each round, tally every remaining
// ballot's highest-ranked non-eliminated candidate. If some candidate's
// tally exceeds half the total ballot count, that candidate wins. Otherwise
// find the minimum tally among remaining candidates: if it equals the
// maximum (all remaining candidates tied), print every remaining name and
// stop; otherwise eliminate everyone at that minimum tally and repeat.
// Termination is guaranteed since with exactly two remaining candidates any
// inequality already implies a majority, so ties (the only non-terminating
// case) can only persist among the full remaining set once it shrinks that
// far -- the loop either finds a majority or the all-tied stop condition.
//
// Ambiguity check: the statement's own board (message 155222) posts a
// 6-candidate example designed to end in an all-tied 2-way stop; hand
// simulation of it matches this algorithm exactly (rounds eliminate the two
// 0-vote candidates, then the two 1-vote candidates, leaving two candidates
// tied 3-3 out of 6 ballots), confirming the "all tied" reading rather than
// eliminating down to a single arbitrary survivor.
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

static void rstrip(string &s) {
    while (!s.empty() && (s[s.size()-1] == '\r' || s[s.size()-1] == '\n'))
        s.erase(s.size()-1);
}

int main() {
    string line;
    if (!getline(cin, line)) return 0;
    rstrip(line);
    int n = atoi(line.c_str());

    vector<string> names(n+1);
    for (int i = 1; i <= n; i++) {
        getline(cin, line);
        rstrip(line);
        names[i] = line;
    }

    vector<vector<int> > ballots;
    while (true) {
        vector<int> r(n);
        bool ok = true;
        for (int i = 0; i < n; i++) {
            if (!(cin >> r[i])) { ok = false; break; }
        }
        if (!ok) break;
        ballots.push_back(r);
    }
    int M = (int)ballots.size();

    vector<bool> eliminated(n+1, false);

    while (true) {
        vector<int> tally(n+1, 0);
        for (int b = 0; b < M; b++) {
            for (int i = 0; i < n; i++) {
                int c = ballots[b][i];
                if (!eliminated[c]) { tally[c]++; break; }
            }
        }

        int maxV = -1, maxC = -1;
        for (int c = 1; c <= n; c++)
            if (!eliminated[c] && tally[c] > maxV) { maxV = tally[c]; maxC = c; }

        if (maxV * 2 > M) {
            cout << names[maxC] << "\n";
            return 0;
        }

        int minV = maxV;
        for (int c = 1; c <= n; c++)
            if (!eliminated[c] && tally[c] < minV) minV = tally[c];

        if (minV == maxV) {
            for (int c = 1; c <= n; c++)
                if (!eliminated[c]) cout << names[c] << "\n";
            return 0;
        }

        for (int c = 1; c <= n; c++)
            if (!eliminated[c] && tally[c] == minV) eliminated[c] = true;
    }
}
