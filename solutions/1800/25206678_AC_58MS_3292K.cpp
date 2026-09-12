// POJ 1800 - Magic Trick
// Model: claude-sonnet-5
//
// Approach: Concatenate the three paragraphs' words into one array indexed
// 0..N-1 (paragraph1 first, then paragraph2, then paragraph3). From index i,
// the trick jumps to i + len(word[i]). Following the chain from a starting
// word in paragraph1, the outcome is the first landing index that falls in
// paragraph3 (report that word, lower-cased) or beyond the end (report
// "-outside-"). Since the jump strictly increases the index, results for all
// indices in [0, n1+n2) can be computed by simple memoized DP processed in
// decreasing index order: dest[i] = dest[i+len[i]] when that stays before
// paragraph3, otherwise it is decided directly. dest[] stores only an int
// (-1 for outside, else the paragraph-3 word index) rather than a copied
// string -- an earlier version stored the outcome text itself in every one
// of up to ~1e5 dest[] slots, and when many positions chained to the same
// long paragraph-3 word that word's text was independently duplicated that
// many times, blowing the 30000K memory limit (confirmed: run 25206673,
// MLE). Materializing text only for the handful of paragraph-3 indices
// actually reached from paragraph 1 keeps memory at O(total input size).
//
// A word is a maximal run of A-Z/a-z; everything else is a separator.
// Paragraph-1/2 words only need their lengths, so those are extracted
// directly into a shared length array without ever storing the substrings.
// Distinct outcomes are collected case-insensitively (confirmed correct by
// the discuss board: two paragraph-3 occurrences of the same word in
// different case count as a single outcome), sorted lexicographically
// (ASCII, so "-outside-" sorts before any lower-case word), and printed;
// more than three distinct outcomes prints "-too many-" instead of the list.
#include <cstdio>
#include <cctype>
#include <string>
#include <vector>
#include <set>
#include <iostream>
#include <limits>
using namespace std;

// Appends the length of each maximal letters-only run in `line` to `out`.
static void extractLensInto(const string& line, vector<int>& out) {
    size_t i = 0, n = line.size();
    while (i < n) {
        if (isalpha((unsigned char)line[i])) {
            size_t j = i;
            while (j < n && isalpha((unsigned char)line[j])) j++;
            out.push_back((int)(j - i));
            i = j;
        } else {
            i++;
        }
    }
}

static vector<string> extractWords(const string& line) {
    vector<string> words;
    size_t i = 0, n = line.size();
    while (i < n) {
        if (isalpha((unsigned char)line[i])) {
            size_t j = i;
            while (j < n && isalpha((unsigned char)line[j])) j++;
            words.push_back(line.substr(i, j - i));
            i = j;
        } else {
            i++;
        }
    }
    return words;
}

static string toLower(const string& s) {
    string r = s;
    for (size_t i = 0; i < r.size(); i++) r[i] = (char)tolower((unsigned char)r[i]);
    return r;
}

int main() {
    int T;
    if (!(cin >> T)) return 0;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int tc = 1; tc <= T; tc++) {
        string l1, l2, l3;
        getline(cin, l1);
        getline(cin, l2);
        getline(cin, l3);

        vector<int> len;
        extractLensInto(l1, len);
        int n1 = (int)len.size();
        extractLensInto(l2, len);
        int M = (int)len.size();
        vector<string> w3 = extractWords(l3);
        int n3 = (int)w3.size();
        int N = M + n3;

        // dest[i]: -1 means the chain from i ends outside paragraph 3,
        // otherwise it is the index into w3 of the landed word.
        vector<int> dest(M);
        for (int i = M - 1; i >= 0; i--) {
            int nxt = i + len[i];
            if (nxt >= N) {
                dest[i] = -1;
            } else if (nxt >= M) {
                dest[i] = nxt - M;
            } else {
                dest[i] = dest[nxt];
            }
        }

        vector<char> mark(n3, 0);
        bool outsideReached = false;
        for (int i = 0; i < n1; i++) {
            if (dest[i] == -1) outsideReached = true;
            else mark[dest[i]] = 1;
        }

        set<string> outcomes;
        if (outsideReached) outcomes.insert("-outside-");
        for (int idx = 0; idx < n3; idx++) {
            if (mark[idx]) outcomes.insert(toLower(w3[idx]));
        }

        printf("Scenario #%d:\n", tc);
        if ((int)outcomes.size() > 3) {
            printf("-too many-\n");
        } else {
            for (set<string>::iterator it = outcomes.begin(); it != outcomes.end(); ++it) {
                printf("%s\n", it->c_str());
            }
        }
        printf("\n");
    }
    return 0;
}
