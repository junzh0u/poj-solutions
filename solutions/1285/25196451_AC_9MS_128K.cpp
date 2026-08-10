// POJ 1285 - Combinations, Once Again
// Model: gpt-5.6-terra
// Approach: Count each distinct label's multiplicity, then multiply the
// generating factors (1 + x + ... + x^count).  The coefficient of x^r is the
// number of different label-multisets of size r.  Query r=0 therefore returns
// one for the empty group.

#include <algorithm>
#include <iostream>

using namespace std;

__extension__ typedef unsigned long long Count;

const int MAX_OBJECTS = 50;

int main() {
    int caseNumber = 0;
    int objectCount;
    int queryCount;
    while (cin >> objectCount >> queryCount && objectCount != 0) {
        int labelCount[MAX_OBJECTS + 1] = {0};
        for (int index = 0; index < objectCount; ++index) {
            int label;
            cin >> label;
            ++labelCount[label];
        }

        Count ways[MAX_OBJECTS + 1] = {0};
        ways[0] = 1;
        int processed = 0;
        for (int label = 1; label <= objectCount; ++label) {
            if (labelCount[label] == 0) {
                continue;
            }
            Count next[MAX_OBJECTS + 1] = {0};
            for (int chosen = 0; chosen <= processed; ++chosen) {
                for (int take = 0; take <= labelCount[label]; ++take) {
                    next[chosen + take] += ways[chosen];
                }
            }
            processed += labelCount[label];
            copy(next, next + objectCount + 1, ways);
        }

        cout << "Case " << ++caseNumber << ":\n";
        for (int query = 0; query < queryCount; ++query) {
            int groupSize;
            cin >> groupSize;
            cout << ways[groupSize] << '\n';
        }
    }
    return 0;
}
