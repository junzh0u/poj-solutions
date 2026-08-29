// POJ 1304 - World's Worst Bus Schedule
// Model: claude-sonnet-5
//
// Approach: each bus repeats a cycle of M route durations (1<=M<=10); it is
// physically at the station at the cumulative prefix-sum instants of that
// cycle (arriving from one route, departing on the next), and the whole
// pattern repeats every S = sum of the bus's durations. Given the arrival
// time T, reduce r = T mod S and find the smallest prefix sum >= r (wrap to
// S - r if none exists, i.e. the next station instant is the start of the
// following cycle). The answer is the minimum such wait over all buses.
//
// Statement ambiguity: the input format text implies a fixed M valid for
// every "route duration line" of a data set, but the sample input's second
// data set proves M varies per bus line (one bus's line holds 9 numbers,
// another only 1, another 2) and is never given explicitly - it must be
// read as "however many integers are on this physical line". The board
// (message 171129, 342298, 5068) independently confirms this: solutions
// read each bus's line with getline/gets and split on whitespace rather
// than reading a fixed count. Verified the algorithm and this parsing model
// by hand against both sample data sets (answers 200 and 20) and against
// the board's own posted test case (message 154255: START 2 / 123 54 /
// 87 23 / 0 / END -> 0).
#include <cstdio>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

int main() {
    string line;
    while (getline(cin, line)) {
        // trim trailing \r
        while (!line.empty() && (line[line.size()-1] == '\r' || line[line.size()-1] == '\n'))
            line.erase(line.size()-1);
        if (line.empty()) continue;
        {
            istringstream check(line);
            string tok;
            check >> tok;
            if (tok == "ENDOFINPUT") break;
        }
        istringstream startLine(line);
        string startTok;
        int n;
        startLine >> startTok >> n;

        vector< vector<long> > prefix(n);
        vector<long> total(n);

        for (int i = 0; i < n; i++) {
            if (!getline(cin, line)) break;
            while (!line.empty() && (line[line.size()-1] == '\r' || line[line.size()-1] == '\n'))
                line.erase(line.size()-1);
            istringstream iss(line);
            long v;
            long sum = 0;
            prefix[i].push_back(0);
            while (iss >> v) {
                sum += v;
                prefix[i].push_back(sum);
            }
            // prefix[i] has m+1 entries: 0 .. S (last one equals total S)
            total[i] = sum;
        }

        // arrival time line
        long T = 0;
        if (getline(cin, line)) {
            while (!line.empty() && (line[line.size()-1] == '\r' || line[line.size()-1] == '\n'))
                line.erase(line.size()-1);
            istringstream iss(line);
            iss >> T;
        }

        // END line - consume and discard
        getline(cin, line);

        long best = -1;
        for (int i = 0; i < n; i++) {
            long S = total[i];
            long r = T % S;
            long wait = -1;
            // prefix[i] has entries 0 .. m-1 real "arrival at station" points
            // (excluding the final one which equals S == the same as 0 of
            // the next cycle), search among indices 0..size-2
            size_t m = prefix[i].size() - 1; // number of distinct station points before wrap
            for (size_t j = 0; j < m; j++) {
                if (prefix[i][j] >= r) {
                    wait = prefix[i][j] - r;
                    break;
                }
            }
            if (wait < 0) {
                wait = S - r;
            }
            if (best < 0 || wait < best) best = wait;
        }

        printf("%ld\n", best);
    }
    return 0;
}
