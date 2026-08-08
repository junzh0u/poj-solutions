// POJ 3484 - Showstopper
// Model: claude-sonnet-5
//
// Each data set is a list of arithmetic sequences X, X+Z, ..., <=Y.
// Over the multiset union of all sequences in a data set, at most one
// value occurs an odd number of times; find it (and its count), or
// report "no corruption" if every value occurs an even number of times.
//
// Approach: binary search on the value range [1, 2^32-1]. For a
// candidate half-range [lo,mid], sum (mod 2) over all lines the count
// of sequence terms falling in that half-range; that parity tells
// whether the odd-count value (if any) lies in the left half. Per line,
// the count of terms of an arithmetic sequence inside an interval is
// computed in O(1) via floor/ceil arithmetic on the step index, so a
// whole binary search is O(numLines * 32). First check the parity over
// the full range to short-circuit "no corruption": if it's 0, no value
// has odd count and there's nothing to localize (a spurious binary
// search would otherwise converge on an arbitrary point in the range).
// Once localized to a single candidate value, its exact total count is
// recomputed by direct membership tests and printed.
//
// Statement/board notes: X,Y,Z are 32-bit positive integers, so sums
// like X+K*Z can exceed 32 bits -- the discuss board explicitly flags
// this as the standard WA cause, hence long long throughout. Data sets
// are separated by one-or-more blank lines, and the last data set in
// the file has no trailing blank line before EOF -- the board also
// flags this (RE / dropped last case) as a common bug, handled here by
// flushing whatever is buffered when getline hits EOF.

#include <cstdio>
#include <cctype>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;

struct Seq { long long X, Y, Z; };

static long long countInRange(long long X, long long Y, long long Z, long long lo, long long hi) {
    if (X > Y) return 0;
    long long K = (Y - X) / Z;      // last term is X + K*Z <= Y
    long long last = X + K * Z;
    if (hi < X || lo > last) return 0;
    long long lo2 = lo > X ? lo : X;
    long long hi2 = hi < last ? hi : last;
    if (lo2 > hi2) return 0;
    long long kmin = (lo2 - X + Z - 1) / Z; // lo2>=X so diff>=0
    long long kmax = (hi2 - X) / Z;         // hi2<=last so diff>=0
    if (kmax < kmin) return 0;
    return kmax - kmin + 1;
}

static const long long LO_BOUND = 1;
static const long long HI_BOUND = 4294967295LL; // 2^32 - 1

static void processDataset(const vector<Seq>& lines, vector<string>& results) {
    int totalParity = 0;
    for (size_t i = 0; i < lines.size(); ++i)
        totalParity ^= (int)(countInRange(lines[i].X, lines[i].Y, lines[i].Z, LO_BOUND, HI_BOUND) & 1);

    if (totalParity == 0) {
        results.push_back("no corruption");
        return;
    }

    long long lo = LO_BOUND, hi = HI_BOUND;
    while (lo < hi) {
        long long mid = lo + (hi - lo) / 2;
        int par = 0;
        for (size_t i = 0; i < lines.size(); ++i)
            par ^= (int)(countInRange(lines[i].X, lines[i].Y, lines[i].Z, lo, mid) & 1);
        if (par == 1) hi = mid;
        else lo = mid + 1;
    }

    long long candidate = lo;
    long long cnt = 0;
    for (size_t i = 0; i < lines.size(); ++i) {
        const Seq& s = lines[i];
        if (candidate >= s.X && candidate <= s.Y && (candidate - s.X) % s.Z == 0) ++cnt;
    }

    ostringstream oss;
    oss << candidate << " " << cnt;
    results.push_back(oss.str());
}

int main() {
    ios::sync_with_stdio(false);
    vector<Seq> lines;
    vector<string> results;
    string line;

    while (true) {
        if (!std::getline(cin, line)) {
            if (!lines.empty()) {
                processDataset(lines, results);
                lines.clear();
            }
            break;
        }
        while (!line.empty() && (line[line.size() - 1] == '\r' || line[line.size() - 1] == '\n'))
            line.erase(line.size() - 1);

        bool blank = true;
        for (size_t i = 0; i < line.size(); ++i) {
            if (!isspace((unsigned char)line[i])) { blank = false; break; }
        }

        if (blank) {
            if (!lines.empty()) {
                processDataset(lines, results);
                lines.clear();
            }
            continue;
        }

        istringstream iss(line);
        long long X, Y, Z;
        if (iss >> X >> Y >> Z) {
            Seq s; s.X = X; s.Y = Y; s.Z = Z;
            lines.push_back(s);
        }
    }

    for (size_t i = 0; i < results.size(); ++i) cout << results[i] << "\n";
    return 0;
}
