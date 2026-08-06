// POJ 1548 - Robots
// Model: claude-sonnet-5
//
// Approach: A robot moves only East/South from NW to SE, so along a single
// robot's path the garbage cells it visits, in the order visited, have both
// row and column non-decreasing. The input already lists garbage cells in
// row-major order (row non-decreasing, and within a row, column increasing),
// so for two cells A appearing before B in the input, row(A) <= row(B)
// always holds; A and B can be served by the same robot iff additionally
// col(A) <= col(B). Hence the problem reduces to: partition the sequence of
// columns (in input order) into the minimum number of non-decreasing
// subsequences. By Dilworth's theorem this minimum equals the length of the
// longest strictly decreasing subsequence of the column sequence, and it can
// be computed greedily in O(n log n):
//
// Maintain a multiset of "tail" column values, one per robot currently in
// use (its last-visited column so far). For each new column c, find the
// robot whose tail is the largest value <= c (i.e. the "tightest fit") and
// reassign that robot's tail to c; this keeps robots with smaller tails
// available for future, smaller columns. If no such robot exists (all tails
// are > c), a new robot is needed.
//
// Statement ambiguity: none found needing resolution beyond what's already
// exploited above (row-major order guarantees the row condition is free).

#include <cstdio>
#include <set>

int main() {
    int r, c;
    while (scanf("%d %d", &r, &c) == 2) {
        if (r == -1 && c == -1) break;
        std::multiset<int> tails;
        // r, c already consumed as the first garbage location of this map
        // (or it's the immediate 0 0 terminator for an empty map).
        while (!(r == 0 && c == 0)) {
            std::multiset<int>::iterator it = tails.upper_bound(c);
            if (it == tails.begin()) {
                // no tail <= c exists
                tails.insert(c);
            } else {
                --it;
                tails.erase(it);
                tails.insert(c);
            }
            if (scanf("%d %d", &r, &c) != 2) break;
        }
        printf("%d\n", (int)tails.size());
    }
    return 0;
}
