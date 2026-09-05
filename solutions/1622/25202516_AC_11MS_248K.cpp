// POJ 1622 - Pushing Boxes
// Model: claude-sonnet-5
// Approach: Each wall push only interacts with boxes along the axis it moves
// (down/up affect only a box's row, and only boxes sharing a column can block
// each other; left/right affect only a box's column, and only boxes sharing a
// row can block each other). So each push decomposes into independent 1-D
// "wall sweeps in from outside" simulations, one per column (down/up) or row
// (left/right).
//
// For a push that increases the moving coordinate (down: rows increase,
// right: columns increase) with room size D along that axis: conceptually the
// near wall starts outside the room and sweeps in, clearing out the first d
// cells; a box only moves if it would otherwise lie in the swept region or be
// bumped by another box ahead of it that is moving. Sorting a line's boxes by
// increasing coordinate (nearest the sweeping wall first) gives:
//   new[0] = max(orig[0], d)
//   new[i] = max(orig[i], new[i-1] + 1)
// and it is a crush if new[last] > D-1.
// The decreasing case (up: rows decrease, left: columns decrease) is the
// mirror image, processed from the far end (largest coordinate first):
//   new[last] = min(orig[last], D-1-d)
//   new[i]    = min(orig[i], new[i+1] - 1)
// and it is a crush if new[0] < 0.
//
// Since the wall is one rigid straight edge, the actual distance it moves is
// the same across every line (row or column) it affects, so the true
// distance used is the minimum, over all such lines, of the largest d <= m
// that avoids a crush on that line. This was verified by hand against both
// sample data sets, including the crush case (right wall stopping at 13 of
// the requested 14 in data set 1) and a case with no crush (data set 2).
//
// Ambiguity: the statement's prose alone does not make clear that a box far
// from the wall does not move until the sweeping wall (or a chain of pushed
// boxes) actually reaches it -- naively pushing every box in a line by the
// full requested amount (capped only by collisions) gives a different, wrong
// answer. This was caught by reconstructing the worked example by hand: a
// naive "shift every box down by d" model reproduced the first move (down 3,
// no crush) but diverged from the given intermediate/final box rows once the
// second move (left 13) was applied, whereas the "sweeping wall" model above
// reproduces the official sample output exactly for both data sets.

#include <cstdio>
#include <algorithm>
#include <vector>
#include <map>
#include <string>
#include <iostream>
using namespace std;

int H, W, n;
int r[15], c[15];

bool cmpRow(int a, int b) { return r[a] < r[b]; }
bool cmpCol(int a, int b) { return c[a] < c[b]; }

// Compute new values for a sorted-ascending list of original coordinates.
vector<int> applyLine(const vector<int>& vals, int d, int D, bool increasing) {
    int k = (int)vals.size();
    vector<int> nv(k);
    if (increasing) {
        nv[0] = max(vals[0], d);
        for (int i = 1; i < k; i++) nv[i] = max(vals[i], nv[i-1] + 1);
    } else {
        nv[k-1] = min(vals[k-1], D - 1 - d);
        for (int i = k - 2; i >= 0; i--) nv[i] = min(vals[i], nv[i+1] - 1);
    }
    return nv;
}

bool feasible(const vector<int>& vals, int d, int D, bool increasing) {
    vector<int> nv = applyLine(vals, d, D, increasing);
    if (increasing) return nv.back() <= D - 1;
    else return nv.front() >= 0;
}

void doPush(bool moveRow, bool increasing, int m) {
    int D = moveRow ? H : W;
    map<int, vector<int> > groups; // fixed coord -> list of box indices
    for (int i = 0; i < n; i++) {
        int key = moveRow ? c[i] : r[i];
        groups[key].push_back(i);
    }
    // Sort each group's indices by the moving coordinate ascending.
    for (map<int, vector<int> >::iterator it = groups.begin(); it != groups.end(); ++it) {
        if (moveRow) sort(it->second.begin(), it->second.end(), cmpRow);
        else sort(it->second.begin(), it->second.end(), cmpCol);
    }
    int capD = min(m, D - 1);
    if (capD < 0) capD = 0;
    int globalD = m;
    for (map<int, vector<int> >::iterator it = groups.begin(); it != groups.end(); ++it) {
        vector<int>& idxs = it->second;
        vector<int> vals(idxs.size());
        for (size_t j = 0; j < idxs.size(); j++) vals[j] = moveRow ? r[idxs[j]] : c[idxs[j]];
        int lineMax = 0;
        for (int d = capD; d >= 0; d--) {
            if (feasible(vals, d, D, increasing)) { lineMax = d; break; }
        }
        if (lineMax < globalD) globalD = lineMax;
    }
    for (map<int, vector<int> >::iterator it = groups.begin(); it != groups.end(); ++it) {
        vector<int>& idxs = it->second;
        vector<int> vals(idxs.size());
        for (size_t j = 0; j < idxs.size(); j++) vals[j] = moveRow ? r[idxs[j]] : c[idxs[j]];
        vector<int> nv = applyLine(vals, globalD, D, increasing);
        for (size_t j = 0; j < idxs.size(); j++) {
            if (moveRow) r[idxs[j]] = nv[j];
            else c[idxs[j]] = nv[j];
        }
    }
}

bool cmpBox(int a, int b) {
    if (r[a] != r[b]) return r[a] < r[b];
    return c[a] < c[b];
}

int main() {
    int dataset = 0;
    while (cin >> H >> W) {
        if (H == 0 && W == 0) break;
        dataset++;
        cin >> n;
        for (int i = 0; i < n; i++) cin >> r[i] >> c[i];
        while (true) {
            string dir;
            cin >> dir;
            if (dir == "done") break;
            int m;
            cin >> m;
            if (dir == "down") doPush(true, true, m);
            else if (dir == "up") doPush(true, false, m);
            else if (dir == "right") doPush(false, true, m);
            else if (dir == "left") doPush(false, false, m);
        }
        vector<int> order(n);
        for (int i = 0; i < n; i++) order[i] = i;
        sort(order.begin(), order.end(), cmpBox);
        printf("Data set %d ends with boxes at locations", dataset);
        for (int i = 0; i < n; i++) {
            printf(" (%d,%d)", r[order[i]], c[order[i]]);
        }
        printf(".\n");
    }
    return 0;
}
