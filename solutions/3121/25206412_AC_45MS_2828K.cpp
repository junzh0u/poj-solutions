// POJ 3121 - The SetStack Computer
// Model: claude-sonnet-5
// Approach: Represent each distinct set encountered by hash-consing: a set's
// value is the sorted, deduplicated list of ids of its element-sets. A
// map<vector<int>,int> assigns each distinct value a unique canonical id, so
// structurally equal sets (however they were built) collapse to the same id
// in O(log) amortized lookups instead of being compared as nested STL sets
// (which the board reports as too slow / TLE for this judge). PUSH pushes
// the id of {} (the empty vector). DUP duplicates the top id. UNION/INTERSECT
// combine the two popped sets' element-id lists via std::set_union /
// set_intersection (both operand lists are already sorted+unique because
// they are themselves canonical values). ADD's semantics were the one
// ambiguous point: the board (message 64454/64455) confirms the first
// popped (topmost) set becomes a *new element* inserted into the second
// popped set, i.e. result = childrenOf(second) plus the id of first. This
// matches the worked example in the statement (A ADD B with A on top).
// Verified against both sample test cases by hand and against the board's
// worked example (PUSH,DUP,PUSH,ADD,ADD -> {{{}}}, cardinality 1).
#include <cstdio>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T--) {
        int n;
        scanf("%d", &n);
        vector<vector<int> > sets;
        map<vector<int>, int> canon;
        // id 0 reserved for empty set
        vector<int> emptyVec;
        sets.push_back(emptyVec);
        canon[emptyVec] = 0;

        vector<int> stk;
        stk.reserve(n + 1);

        char cmd[16];
        for (int i = 0; i < n; ++i) {
            scanf("%s", cmd);
            if (cmd[0] == 'P') { // PUSH
                stk.push_back(0);
            } else if (cmd[0] == 'D') { // DUP
                stk.push_back(stk.back());
            } else if (cmd[0] == 'U') { // UNION
                int x = stk.back(); stk.pop_back();
                int y = stk.back(); stk.pop_back();
                vector<int> r;
                r.reserve(sets[x].size() + sets[y].size());
                set_union(sets[x].begin(), sets[x].end(),
                          sets[y].begin(), sets[y].end(),
                          back_inserter(r));
                map<vector<int>, int>::iterator it = canon.find(r);
                int id;
                if (it != canon.end()) id = it->second;
                else { id = (int)sets.size(); sets.push_back(r); canon[r] = id; }
                stk.push_back(id);
            } else if (cmd[0] == 'I') { // INTERSECT
                int x = stk.back(); stk.pop_back();
                int y = stk.back(); stk.pop_back();
                vector<int> r;
                set_intersection(sets[x].begin(), sets[x].end(),
                                  sets[y].begin(), sets[y].end(),
                                  back_inserter(r));
                map<vector<int>, int>::iterator it = canon.find(r);
                int id;
                if (it != canon.end()) id = it->second;
                else { id = (int)sets.size(); sets.push_back(r); canon[r] = id; }
                stk.push_back(id);
            } else { // ADD
                int x = stk.back(); stk.pop_back(); // first popped: topmost
                int y = stk.back(); stk.pop_back(); // second popped
                vector<int> r = sets[y];
                r.push_back(x);
                sort(r.begin(), r.end());
                r.erase(unique(r.begin(), r.end()), r.end());
                map<vector<int>, int>::iterator it = canon.find(r);
                int id;
                if (it != canon.end()) id = it->second;
                else { id = (int)sets.size(); sets.push_back(r); canon[r] = id; }
                stk.push_back(id);
            }
            printf("%d\n", (int)sets[stk.back()].size());
        }
        printf("***\n");
    }
    return 0;
}
