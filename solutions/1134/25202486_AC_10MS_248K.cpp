// POJ 1134 - Hexagon
// Model: claude-sonnet-5
//
// The board's 19 cells are the hex-coordinate triples (x,y,z) with
// x+y+z=0 and x,y,z in {-2,...,2}; the three primary directions are the
// five "rows" for fixed x, fixed y, and fixed z respectively (sizes
// 3,4,5,4,3, matching the picture). Per the hint, we only consider
// coverings where every row is monochromatic (uses one of the 3 given
// numbers for that direction), since otherwise the row contributes 0 and
// can never be part of an optimum (using the row's number instead is at
// least as good, or we can also choose not to worry about it because the
// hint says to restrict to boards where every row score is positive).
//
// Since only 3 distinct numbers exist per direction but there are 5 rows,
// each row is colored with one of 3 colors, i.e. the 5 row positions are
// partitioned into (at most) 3 color classes. A color class of rows all
// sharing value v in direction d can only use pieces whose direction-d
// coordinate equals v; there are only 3*3=9 such pieces total (the other
// two directions each have 3 choices), so every color class must contain
// at most 9 cells' worth of rows. Checking all subsets of the row sizes
// {3,4,5,4,3} shows the only way to split them into <=3 parts each of
// size <=9 is a partition into two pairs and one singleton (any 3-row
// group already sums to >=10). So each direction's coloring is exactly
// one of the 15 ways to partition the 5 row positions into {pair,pair,
// singleton}.
//
// For a chosen triple of such partitions (one per direction) to be
// realizable by 19 actually-distinct pieces, the map cell -> (color_x,
// color_y, color_z) must be injective over the 19 cells - we just check
// this directly (cheap: 19 cells, 3375 partition combinations).
//
// Given a valid combination, the score is separable per direction: for
// each direction, the three given numbers are assigned to the three
// color classes (of the fixed sizes) to maximize sum(value*size), which
// by the rearrangement inequality is achieved by sorting both descending
// and pairing them up. We take the best score over all valid partition
// combinations.
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

struct Cell { int x, y, z; };
Cell cells[25];
int ncells = 0;

// 15 groupings of positions 0..4 (representing coordinate value-2..2)
// into {pair, pair, singleton}. groupId[k][pos] in {0,1,2}.
int groupId[15][5];
int groupCount = 0;
const int posSize[5] = {3, 4, 5, 4, 3};

void buildGroupings() {
    for (int s = 0; s < 5; s++) {
        int rem[4], ri = 0;
        for (int p = 0; p < 5; p++) if (p != s) rem[ri++] = p;
        int pairings[3][2][2] = {
            {{rem[0], rem[1]}, {rem[2], rem[3]}},
            {{rem[0], rem[2]}, {rem[1], rem[3]}},
            {{rem[0], rem[3]}, {rem[1], rem[2]}}
        };
        for (int pi = 0; pi < 3; pi++) {
            int k = groupCount++;
            for (int p = 0; p < 5; p++) groupId[k][p] = -1;
            groupId[k][s] = 0;
            groupId[k][pairings[pi][0][0]] = 1;
            groupId[k][pairings[pi][0][1]] = 1;
            groupId[k][pairings[pi][1][0]] = 2;
            groupId[k][pairings[pi][1][1]] = 2;
        }
    }
}

int groupSizeOf(int k, int gid) {
    int total = 0;
    for (int p = 0; p < 5; p++) if (groupId[k][p] == gid) total += posSize[p];
    return total;
}

long bestForDirection(const int nums[3], const int sizes[3]) {
    int a[3] = {nums[0], nums[1], nums[2]};
    int b[3] = {sizes[0], sizes[1], sizes[2]};
    sort(a, a + 3, greater<int>());
    sort(b, b + 3, greater<int>());
    return (long)a[0]*b[0] + (long)a[1]*b[1] + (long)a[2]*b[2];
}

int main() {
    buildGroupings();
    for (int x = -2; x <= 2; x++)
        for (int y = -2; y <= 2; y++) {
            int z = -(x + y);
            if (z < -2 || z > 2) continue;
            cells[ncells].x = x; cells[ncells].y = y; cells[ncells].z = z;
            ncells++;
        }

    int n;
    if (scanf("%d", &n) != 1) return 0;
    for (int tc = 1; tc <= n; tc++) {
        int num1[3], num2[3], num3[3];
        for (int i = 0; i < 3; i++) scanf("%d", &num1[i]);
        for (int i = 0; i < 3; i++) scanf("%d", &num2[i]);
        for (int i = 0; i < 3; i++) scanf("%d", &num3[i]);

        long best = 0;
        bool haveBest = false;
        for (int g1 = 0; g1 < 15; g1++) {
            int sz1[3] = {groupSizeOf(g1,0), groupSizeOf(g1,1), groupSizeOf(g1,2)};
            for (int g2 = 0; g2 < 15; g2++) {
                int sz2[3] = {groupSizeOf(g2,0), groupSizeOf(g2,1), groupSizeOf(g2,2)};
                for (int g3 = 0; g3 < 15; g3++) {
                    bool used[27];
                    memset(used, 0, sizeof(used));
                    bool ok = true;
                    for (int c = 0; c < ncells && ok; c++) {
                        int cx = groupId[g1][cells[c].x + 2];
                        int cy = groupId[g2][cells[c].y + 2];
                        int cz = groupId[g3][cells[c].z + 2];
                        int id = cx * 9 + cy * 3 + cz;
                        if (used[id]) { ok = false; break; }
                        used[id] = true;
                    }
                    if (!ok) continue;
                    int sz3[3] = {groupSizeOf(g3,0), groupSizeOf(g3,1), groupSizeOf(g3,2)};
                    long score = bestForDirection(num1, sz1)
                               + bestForDirection(num2, sz2)
                               + bestForDirection(num3, sz3);
                    if (!haveBest || score > best) { best = score; haveBest = true; }
                }
            }
        }
        printf("Test #%d\n%ld\n\n", tc, best);
    }
    return 0;
}
