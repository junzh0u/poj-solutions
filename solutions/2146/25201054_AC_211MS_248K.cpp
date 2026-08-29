// POJ 2146 - Confusing Login Names
// Model: claude-sonnet-5
// Approach: true (unrestricted) Damerau-Levenshtein distance via the
// Lowrance-Wagner algorithm. The statement's own example -- distance
// "ant"->"neat" is 2, via swap 'a','n' then insert 'e' -- rules out the
// common restricted/OSA swap recurrence (dp[i-2][j-2]+1 when
// s1[i]==s2[j-1] && s1[i-1]==s2[j]), which forbids editing a just-swapped
// segment again and yields 3 for that pair (verified by hand). Only the
// full Lowrance-Wagner DP, which lets a transposed pair still be split by
// a later insertion, reproduces the required distance of 2. The board
// (message 35158) also confirms the swap operates on adjacent characters
// only, matching the statement.
// For each dataset, brute force all C(n,2) pairs (n<=200, len<16) with the
// O(len1*len2) DP; comfortably inside the 10s limit.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

int damerau(const string &a, const string &b) {
    int la = (int)a.size(), lb = (int)b.size();
    int maxdist = la + lb;
    vector<vector<int> > d(la + 2, vector<int>(lb + 2, 0));
#define D(i,j) d[(i)+1][(j)+1]
    D(-1,-1) = maxdist;
    for (int i = 0; i <= la; i++) { D(i,-1) = maxdist; D(i,0) = i; }
    for (int j = 0; j <= lb; j++) { D(-1,j) = maxdist; D(0,j) = j; }

    int da[26];
    memset(da, 0, sizeof(da));

    for (int i = 1; i <= la; i++) {
        int db = 0;
        for (int j = 1; j <= lb; j++) {
            int k = da[b[j-1]-'a'];
            int l = db;
            int cost;
            if (a[i-1] == b[j-1]) { cost = 0; db = j; }
            else cost = 1;
            int sub = D(i-1,j-1) + cost;
            int ins = D(i, j-1) + 1;
            int del = D(i-1, j) + 1;
            int trans = D(k-1, l-1) + (i-k-1) + 1 + (j-l-1);
            int best = sub;
            if (ins < best) best = ins;
            if (del < best) best = del;
            if (trans < best) best = trans;
            D(i,j) = best;
        }
        da[a[i-1]-'a'] = i;
    }
    int result = D(la, lb);
#undef D
    return result;
}

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        int d;
        scanf("%d", &d);
        vector<string> names(n);
        for (int i = 0; i < n; i++) {
            char buf[32];
            scanf("%s", buf);
            names[i] = buf;
        }
        vector<pair<string,string> > pairs;
        for (int i = 0; i < n; i++) {
            for (int j = i+1; j < n; j++) {
                int diff = damerau(names[i], names[j]);
                if (diff <= d) {
                    if (names[i] < names[j]) pairs.push_back(make_pair(names[i], names[j]));
                    else pairs.push_back(make_pair(names[j], names[i]));
                }
            }
        }
        sort(pairs.begin(), pairs.end());
        for (size_t k = 0; k < pairs.size(); k++) {
            printf("%s,%s\n", pairs[k].first.c_str(), pairs[k].second.c_str());
        }
        printf("%d\n", (int)pairs.size());
    }
    return 0;
}
