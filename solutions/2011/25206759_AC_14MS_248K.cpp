// POJ 2011 - Primary X-Subfactor Series
// Model: claude-sonnet-5
// Approach: For each integer, find its "primary x-subfactor series": at each step,
// choose a "subfactor" (a subsequence of the current number's digit string, value >1,
// dividing the current number) and discard exactly those digit occurrences; the
// remaining digits, in original order with leading zeros stripped, form the next
// number. Repeat until a number with no subfactor remains. The primary series has
// maximal length; ties are broken by the smallest next number, then the smallest one
// after that, and so on.
//
// Since discarding a subfactor's digits (whose length is at least 1, as the
// subfactor is itself a proper subsequence) always strictly shortens the digit
// string, the transition relation is a DAG on values, so results are memoized per
// value with a map: children(V) is found by brute-forcing all 2^L - 2 nontrivial
// masks of which of the (<=9) digits to keep (removed digits, taken in original
// order, must have no leading zero, be >1, and divide V); maxLen(V) = 1 + the best
// child's maxLen (or 1 if V has no children). The primary series is then built
// greedily: from the current value, among its children achieving maxLen(cur)-1, take
// the smallest one, and repeat until a value with no children is reached.
//
// No statement ambiguity found. Verified by hand against the worked example in the
// statement (2004 -> 200 -> 0, beating the shorter 2004 -> 4 alternative) and against
// all four sample lines locally; the discuss board only confirms the memoized-search
// approach and to be careful with the subfactor validity check.

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

map<int, int> maxLenMemo;
map<int, vector<int> > childrenMemo;

vector<int> computeChildren(int v) {
    vector<int> result;
    if (v <= 0) return result; // 0 has no subfactor
    char buf[16];
    sprintf(buf, "%d", v);
    int L = (int)strlen(buf);
    if (L <= 1) return result; // single digit: no subfactor possible

    int total = 1 << L;
    for (int mask = 1; mask < total - 1; mask++) {
        // bit i set => keep digit i (i = 0 is the leftmost digit)
        char removed[16];
        int rlen = 0;
        for (int i = 0; i < L; i++) {
            if (!(mask & (1 << i))) removed[rlen++] = buf[i];
        }
        removed[rlen] = '\0';
        if (rlen == 0) continue;
        if (removed[0] == '0') continue; // subfactor cannot have a leading zero
        long d = atol(removed);
        if (d <= 1) continue;
        if (v % d != 0) continue;

        char kept[16];
        int klen = 0;
        for (int i = 0; i < L; i++) {
            if (mask & (1 << i)) kept[klen++] = buf[i];
        }
        kept[klen] = '\0';
        int start = 0;
        while (start < klen - 1 && kept[start] == '0') start++;
        int kval = atoi(kept + start);
        result.push_back(kval);
    }
    sort(result.begin(), result.end());
    result.erase(unique(result.begin(), result.end()), result.end());
    return result;
}

int maxLen(int v) {
    map<int, int>::iterator it = maxLenMemo.find(v);
    if (it != maxLenMemo.end()) return it->second;
    vector<int> ch = computeChildren(v);
    int best = 1;
    for (size_t i = 0; i < ch.size(); i++) {
        int cl = 1 + maxLen(ch[i]);
        if (cl > best) best = cl;
    }
    childrenMemo[v] = ch;
    maxLenMemo[v] = best;
    return best;
}

int main() {
    int n;
    while (scanf("%d", &n) == 1) {
        if (n == 0) break;
        int cur = n;
        int curLen = maxLen(cur);
        printf("%d", cur);
        while (curLen > 1) {
            vector<int> &ch = childrenMemo[cur];
            int target = curLen - 1;
            int best = -1;
            for (size_t i = 0; i < ch.size(); i++) {
                if (maxLen(ch[i]) == target) { best = ch[i]; break; }
            }
            cur = best;
            printf(" %d", cur);
            curLen = maxLen(cur);
        }
        printf("\n");
    }
    return 0;
}
