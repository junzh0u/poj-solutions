// POJ 1572 - Automatic Editing
// Model: claude-opus-5
//
// Direct simulation of the rule list.  For each rule in order: repeatedly find
// the FIRST occurrence of the find string, starting the search at the beginning
// of the text every time (statement note 1), and splice in the replace-by
// string; stop when the find string no longer occurs, then move on to the next
// rule and never revisit an earlier one (statement note 2).
//
// Two readings the official sample cannot distinguish, both settled by those
// notes rather than by the sample:
//   * resuming the search after the just-inserted replacement instead of
//     restarting at position 0 -- differs on e.g. find "ban" over "banana boat"
//     only in later cases, and note (1) forbids it;
//   * rescanning the rules from rule 1 whenever any rule fires -- reproduces
//     the sample byte-for-byte and is what the discuss board reports as the
//     cause of Time Limit Exceeded (it can cycle forever).  Note (2) forbids it.
//
// Every line is read with getline: find, replace-by and text strings may contain
// spaces ("ba b" -> "hind the g") and a replace-by string may be empty, which
// deletes the match and can leave an empty output line.
//
// The loop guards are pure anti-hang insurance: the statement bounds the text at
// 255 characters throughout, so they cannot fire on valid input.

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

static void strip_cr(string &s) {
    while (!s.empty() && s[s.size() - 1] == '\r') s.erase(s.size() - 1);
}

int main() {
    string line;
    while (getline(cin, line)) {
        strip_cr(line);
        int n = atoi(line.c_str());
        if (n <= 0) break;

        string find_s[16], repl_s[16];
        for (int i = 0; i < n; i++) {
            getline(cin, find_s[i]);
            strip_cr(find_s[i]);
            getline(cin, repl_s[i]);
            strip_cr(repl_s[i]);
        }
        string text;
        getline(cin, text);
        strip_cr(text);

        for (int i = 0; i < n; i++) {
            if (find_s[i].empty()) continue;
            long guard = 0;
            while (guard++ < 1000000L && text.size() < 100000u) {
                string::size_type p = text.find(find_s[i]);
                if (p == string::npos) break;
                text.replace(p, find_s[i].size(), repl_s[i]);
            }
        }
        cout << text << "\n";
    }
    return 0;
}
