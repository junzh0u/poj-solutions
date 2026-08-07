// POJ 1917 - Automatic Poetry
// Model: claude-opus-5
//
// Pure string simulation. The first line of each pair has the form
// s1<s2>s3<s4>s5; c1 is that line with the two '<' and two '>' removed,
// i.e. s1s2s3s4s5. The second line ends with "..."; c2 replaces those three
// dots with s4s3s2s5 (note the order: the *second* bracketed group first,
// then the text between the groups, then the first bracketed group, then the
// tail). Any si may be empty (the sample's "<>oma" has s2 empty) and may
// contain blanks, so lines must be read whole with getline, never with
// scanf("%s").
//
// Parsing: locate the four bracket characters by position rather than by
// tokenizing, so empty groups fall out naturally. The dots are located as the
// first occurrence of "..." and whatever follows them is kept, which is a
// no-op for well-formed input (the statement says the line ends with them)
// but is what the known-accepted solutions on the discuss board do.
//
// Robustness: strip a trailing '\r' in case the judge data has CRLF endings,
// consume the rest of the line after reading n, and stop early on EOF.

#include <cstdio>
#include <iostream>
#include <string>

using namespace std;

static void chomp(string &s) {
    while (!s.empty() && (s[s.size() - 1] == '\r' || s[s.size() - 1] == '\n')) {
        s.erase(s.size() - 1);
    }
}

int main() {
    int n;
    if (!(cin >> n)) return 0;
    string dummy;
    getline(cin, dummy); // rest of the line holding n

    for (int t = 0; t < n; ++t) {
        string l1, l2;
        if (!getline(cin, l1)) break;
        if (!getline(cin, l2)) break;
        chomp(l1);
        chomp(l2);

        // s1 < s2 > s3 < s4 > s5
        string s1, s2, s3, s4, s5;
        string::size_type a = l1.find('<');
        string::size_type b = (a == string::npos) ? string::npos : l1.find('>', a + 1);
        string::size_type c = (b == string::npos) ? string::npos : l1.find('<', b + 1);
        string::size_type d = (c == string::npos) ? string::npos : l1.find('>', c + 1);

        if (d != string::npos) {
            s1 = l1.substr(0, a);
            s2 = l1.substr(a + 1, b - a - 1);
            s3 = l1.substr(b + 1, c - b - 1);
            s4 = l1.substr(c + 1, d - c - 1);
            s5 = l1.substr(d + 1);
        } else {
            // Malformed (should not happen); degrade to "brackets removed".
            for (string::size_type i = 0; i < l1.size(); ++i) {
                if (l1[i] != '<' && l1[i] != '>') s1 += l1[i];
            }
        }

        cout << s1 << s2 << s3 << s4 << s5 << "\n";

        string::size_type p = l2.find("...");
        if (p == string::npos) {
            cout << l2 << "\n";
        } else {
            cout << l2.substr(0, p) << s4 << s3 << s2 << s5 << l2.substr(p + 3) << "\n";
        }
    }
    return 0;
}
