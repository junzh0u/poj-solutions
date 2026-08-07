// POJ 3781 - Nth Largest Value
// Model: claude-opus-5
//
// Approach: for each of the P data sets read the data set number followed by
// exactly 10 integers, sort the 10 values ascending and print the number
// together with a[7] -- the 3rd largest. P <= 1000 and values are in [1,1000],
// so an int and a plain insertion/std::sort over 10 elements is far inside the
// 1000MS / 65536K limits.
//
// Ambiguity: "the 3rd largest value" could mean the 3rd largest *distinct*
// value when the ten integers repeat. The official sample has no duplicates,
// so it cannot separate the two readings. The multiset reading (3rd element
// from the top of the sorted list, duplicates counted) is taken here: it is
// the plain reading of "the 3rd largest value of the corresponding 10
// integers" -- the phrase ranks the 10 integers, not their distinct values --
// and it is what every accepted program quoted on the problem's discuss board
// does (sort of 10, print a[7]). The distinct reading would also be undefined
// whenever fewer than 3 distinct values occur, which the statement never
// mentions; that silence is further evidence it is not the intended reading.
//
// The data set number is echoed from the input rather than reproduced from the
// loop counter -- the statement calls it "the data set number", never promising
// the sets are numbered 1..P in order. (The discuss board shows this exact slip
// costing a Wrong Answer.)

#include <cstdio>
#include <algorithm>

int main() {
    int p;
    if (scanf("%d", &p) != 1) return 0;
    while (p-- > 0) {
        int id;
        if (scanf("%d", &id) != 1) break;
        int a[10];
        for (int i = 0; i < 10; ++i) scanf("%d", &a[i]);
        std::sort(a, a + 10);
        printf("%d %d\n", id, a[7]);
    }
    return 0;
}
