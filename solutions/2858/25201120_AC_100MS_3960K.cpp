// POJ 2858 - Crabbles
// Model: claude-sonnet-5
//
// For each dictionary word, only its multiset of letters matters (any
// permutation of the chosen tiles that spells the word is fine), so each
// word (length <= 10, since a hand has at most 10 tiles) is stored as its
// sorted letter signature in a hash set, deduping anagrams for free.
// Words longer than 10 letters can never fit in any hand and are dropped.
//
// For a hand, for each letter present, sort its tile values descending and
// prefix-sum them: using k tiles of that letter, the best choice is always
// the top k values (order among identical-letter tiles never matters). A
// hand has at most 10 tiles, hence at most 10 distinct letters present, so
// the Cartesian product of "how many of each present letter to use"
// (product over present letters of count_l+1) is at most 2^10 = 1024,
// maximized exactly when every tile is a distinct letter. Enumerate that
// product directly by DFS, build each candidate's sorted-letter signature
// (letters are visited in a-z order and each letter's occurrences are
// contiguous, so the built string is already sorted), and look it up in
// the signature set. This replaces "scan every dictionary word per hand"
// (documented on the discuss board as TLE'ing at N=100000, M=1000) with
// work bounded by ~1024 signature lookups per hand regardless of N.
//
// Ambiguity: the statement never says what to print when no dictionary
// word can be formed at all from a hand's tiles. Treated as score 0
// (she simply forms no word), which is also the accumulator's natural
// starting value, so no special-casing is needed.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
using namespace std;

set<string> dict;

int numPresent;
char letterOf[11];
int presentCount[11];
int prefixSum[11][11];
int bestScore;
char buf[16];

void dfs(int pos, int len, int scoreSoFar) {
    if (pos == numPresent) {
        if (len > 0) {
            string s(buf, buf + len);
            if (dict.find(s) != dict.end()) {
                if (scoreSoFar > bestScore) bestScore = scoreSoFar;
            }
        }
        return;
    }
    int cnt = presentCount[pos];
    char ch = letterOf[pos];
    for (int k = 0; k <= cnt; k++) {
        for (int i = 0; i < k; i++) buf[len + i] = ch;
        dfs(pos + 1, len + k, scoreSoFar + prefixSum[pos][k]);
    }
}

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        static char w[64];
        scanf("%s", w);
        int len = (int)strlen(w);
        if (len >= 1 && len <= 10) {
            string s(w, w + len);
            sort(s.begin(), s.end());
            dict.insert(s);
        }
    }

    int m;
    scanf("%d", &m);
    while (m--) {
        int p;
        scanf("%d", &p);
        vector<int> vals[26];
        for (int i = 0; i < p; i++) {
            char c;
            int v;
            scanf(" %c %d", &c, &v);
            vals[c - 'a'].push_back(v);
        }

        numPresent = 0;
        for (int l = 0; l < 26; l++) {
            if (!vals[l].empty()) {
                sort(vals[l].rbegin(), vals[l].rend());
                int cnt = (int)vals[l].size();
                letterOf[numPresent] = (char)('a' + l);
                presentCount[numPresent] = cnt;
                prefixSum[numPresent][0] = 0;
                for (int k = 1; k <= cnt; k++) {
                    prefixSum[numPresent][k] = prefixSum[numPresent][k - 1] + vals[l][k - 1];
                }
                numPresent++;
            }
        }

        bestScore = 0;
        dfs(0, 0, 0);
        printf("%d\n", bestScore);
    }
    return 0;
}
