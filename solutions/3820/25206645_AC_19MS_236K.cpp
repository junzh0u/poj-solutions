// POJ 3820 - XML
// Model: claude-sonnet-5
// Approach: hand-written scanner over each line, maintaining a stack of open tag
// names. Recognizes &lt; &gt; &amp; escapes, &xHEX; byte escapes (HEX = even,
// positive count of upper/lower hex digits), <tag> (push), <tag/> (no push),
// </tag> (pop, must match top of stack), and plain text restricted to ASCII
// 32..127 excluding '<' '>' '&'. Document valid iff the whole line parses with
// no leftover error and the stack ends empty.
// Ambiguity: the statement never says tag names must be non-empty. POJ's
// discuss board for 3820 (messages 164131, 166288, 350744) settles it: the
// intended/accepted behavior treats tag names as required non-empty, i.e.
// "<>" / "</>" is invalid, not a degenerate valid tag.
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

static bool isHexDigit(char c) {
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}
static bool isLowerAlnum(char c) {
    return (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9');
}

static bool matches(const string &s, int pos, const char *lit) {
    int len = 0;
    while (lit[len]) len++;
    if (pos + len > (int)s.size()) return false;
    for (int k = 0; k < len; k++) {
        if (s[pos + k] != lit[k]) return false;
    }
    return true;
}

static bool isValidDocument(const string &s) {
    int n = (int)s.size();
    vector<string> stk;
    int i = 0;
    while (i < n) {
        char c = s[i];
        if (c == '&') {
            if (matches(s, i + 1, "lt;")) { i += 4; continue; }
            if (matches(s, i + 1, "gt;")) { i += 4; continue; }
            if (matches(s, i + 1, "amp;")) { i += 5; continue; }
            if (i + 1 < n && s[i + 1] == 'x') {
                int j = i + 2;
                while (j < n && isHexDigit(s[j])) j++;
                int cnt = j - (i + 2);
                if (cnt >= 2 && cnt % 2 == 0 && j < n && s[j] == ';') {
                    i = j + 1;
                    continue;
                }
                return false;
            }
            return false;
        } else if (c == '<') {
            if (i + 1 < n && s[i + 1] == '/') {
                int j = i + 2;
                while (j < n && isLowerAlnum(s[j])) j++;
                if (j == i + 2) return false;
                string tag = s.substr(i + 2, j - (i + 2));
                if (j < n && s[j] == '>') {
                    if (stk.empty() || stk.back() != tag) return false;
                    stk.pop_back();
                    i = j + 1;
                    continue;
                }
                return false;
            } else {
                int j = i + 1;
                while (j < n && isLowerAlnum(s[j])) j++;
                if (j == i + 1) return false;
                string tag = s.substr(i + 1, j - (i + 1));
                if (j < n && s[j] == '>') {
                    stk.push_back(tag);
                    i = j + 1;
                    continue;
                }
                if (j + 1 < n && s[j] == '/' && s[j + 1] == '>') {
                    i = j + 2;
                    continue;
                }
                return false;
            }
        } else if (c == '>') {
            return false;
        } else {
            unsigned char uc = (unsigned char)c;
            if (uc < 32 || uc > 127) return false;
            i++;
        }
    }
    return stk.empty();
}

int main() {
    string line;
    while (getline(cin, line)) {
        if (!line.empty() && line[line.size() - 1] == '\r') {
            line.erase(line.size() - 1);
        }
        puts(isValidDocument(line) ? "valid" : "invalid");
    }
    return 0;
}
