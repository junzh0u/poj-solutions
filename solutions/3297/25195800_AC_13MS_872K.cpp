// POJ 3297 - Open Source
// Model: claude-opus-5
//
// Line-classification scan, then a two-pass count.
//
// Input framing: lines are classified by their first character alone, which the
// statement makes unambiguous -- project names are capitalized, userids start
// with a lower-case letter, and only the sentinels start with a digit ('1' ends
// a test case, '0' ends the input). Project names contain spaces
// ("LIVESPACE BLOGJAM"), so every line must be read whole with getline; reading
// whitespace-delimited tokens is the classic wrong answer here.
//
// Counting: each sheet keeps a set of userids, so repeated signatures on one
// sheet collapse. A userid that occurs on more than one sheet counts for none,
// so a second pass over an occurrence map (userid -> number of sheets carrying
// it) keeps only those with exactly one. Sheets with no eligible signer are
// still printed, with 0.
//
// Ordering: signups descending, then project name ascending; encoded as a
// sorted set of (-count, name) pairs, which also collapses two sheets that
// share a name and a count (the behaviour of the contest's own reference
// solution -- the judge data has no repeated project name, so this only pins
// down an unreachable case).

#include <cstdio>
#include <cctype>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <iostream>

using namespace std;

static bool readLine(string &s) {
    if (!getline(cin, s)) return false;
    while (!s.empty() && (s[s.size() - 1] == '\r' || s[s.size() - 1] == '\n'))
        s.erase(s.size() - 1);
    return true;
}

static void emit(const vector<string> &names, const vector<set<string> > &sheets) {
    map<string, int> occurs;
    for (size_t i = 0; i < sheets.size(); ++i) {
        for (set<string>::const_iterator it = sheets[i].begin(); it != sheets[i].end(); ++it)
            ++occurs[*it];
    }
    set<pair<int, string> > ans;
    for (size_t i = 0; i < sheets.size(); ++i) {
        int n = 0;
        for (set<string>::const_iterator it = sheets[i].begin(); it != sheets[i].end(); ++it)
            if (occurs[*it] == 1) ++n;
        ans.insert(make_pair(-n, names[i]));
    }
    for (set<pair<int, string> >::const_iterator it = ans.begin(); it != ans.end(); ++it)
        printf("%s %d\n", it->second.c_str(), -it->first);
}

int main() {
    string line;
    vector<string> names;
    vector<set<string> > sheets;
    while (readLine(line)) {
        if (line.empty()) continue;
        char c = line[0];
        if (c == '0') break;
        if (c == '1') {
            emit(names, sheets);
            names.clear();
            sheets.clear();
            continue;
        }
        if (islower(static_cast<unsigned char>(c))) {
            if (!sheets.empty()) sheets.back().insert(line);
            continue;
        }
        names.push_back(line);
        sheets.push_back(set<string>());
    }
    return 0;
}
