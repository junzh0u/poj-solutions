// POJ 3654 - Electronic Document Security
// Model: claude-sonnet-5
//
// For each log line, maintain a rights bitmask (26 bits, one per lowercase
// letter) per entity (26 uppercase letters). An entry is parsed as a run of
// uppercase letters (the entity set E), one operator (+, -, =), then a run
// of lowercase letters (the right set R). Apply the operator to every
// entity in E: '+' ORs R in, '-' ANDs the complement of R, '=' assigns R.
// Entries are processed left to right (log order = chronological order, and
// later entries take precedence, which falls out naturally from applying
// them in order).
//
// After processing a whole log line, walk entities A..Z in order; skip any
// with an empty rights mask (rule: entities with no current rights are not
// listed, even if mentioned in the log). Consecutive entities (in this
// alphabetical walk) that share the exact same nonempty rights mask are
// grouped: their letters are printed together, followed by their shared
// rights (sorted, printed once). Verified by hand against all four sample
// cases, including the grouping rule (case 4: BHJ share {f,w}, LP share
// {a,w}) and the individual-letters case (case 2, where A/B/M/Y all differ
// or are adjacent-but-different).
//
// No ambiguity found in the statement; the discuss board's only content
// relevant here is confirmation of the same reading (message 163389 walks
// through the sample by hand) plus unrelated G++/C++ compiler-quirk noise
// (message 97279) that doesn't bear on the algorithm.
#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>
using namespace std;

int main() {
    string line;
    int caseNum = 0;
    while (getline(cin, line)) {
        while (!line.empty() && (line[line.size() - 1] == '\r' || line[line.size() - 1] == '\n'))
            line.erase(line.size() - 1);
        if (line == "#") break;
        if (line.empty()) continue;
        caseNum++;
        int rights[26];
        memset(rights, 0, sizeof(rights));
        size_t i = 0, n = line.size();
        while (i < n) {
            size_t start = i;
            while (i < n && line[i] >= 'A' && line[i] <= 'Z') i++;
            size_t eEnd = i;
            char op = line[i];
            i++;
            size_t rstart = i;
            while (i < n && line[i] >= 'a' && line[i] <= 'z') i++;
            size_t rEnd = i;
            int rmask = 0;
            for (size_t k = rstart; k < rEnd; k++) rmask |= (1 << (line[k] - 'a'));
            for (size_t k = start; k < eEnd; k++) {
                int e = line[k] - 'A';
                if (op == '+') rights[e] |= rmask;
                else if (op == '-') rights[e] &= ~rmask;
                else rights[e] = rmask;
            }
            if (i < n && line[i] == ',') i++;
        }
        // "Consecutive" for the grouping rule means consecutive among the
        // entities that actually get listed (nonempty rights), not
        // consecutive letters of the alphabet: e.g. C and S group together
        // in sample case 1 even though B, D..R are skipped between them.
        int idx[26], cnt = 0;
        for (int j = 0; j < 26; j++)
            if (rights[j] != 0) idx[cnt++] = j;
        printf("%d:", caseNum);
        int p = 0;
        while (p < cnt) {
            int q = p;
            while (q < cnt && rights[idx[q]] == rights[idx[p]]) {
                putchar('A' + idx[q]);
                q++;
            }
            for (int b = 0; b < 26; b++)
                if (rights[idx[p]] & (1 << b)) putchar('a' + b);
            p = q;
        }
        printf("\n");
    }
    return 0;
}
