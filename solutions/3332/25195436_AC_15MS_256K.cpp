// POJ 3332 - Parsing Real Numbers
// Model: claude-sonnet-5
// Approach: grammar is [ws*] [sign] digit+ ['.' digit+] [('e'|'E') [sign] digit+] [ws*],
// matched end to end against the whole line. Manual left-to-right scan (no regex
// engine available in C++98); each optional clause is consumed only when its
// leading digit run is non-empty, per "at least one digit on each side of the
// point" and the symmetric requirement for exponent digits. Trailing '\r' is
// stripped defensively in case the judge's input carries CRLF line endings.
#include <cstdio>
#include <cctype>
#include <string>
#include <iostream>
using namespace std;

static bool isBlank(char c) { return c == ' ' || c == '\t'; }

static bool checkNumber(const string &raw) {
    string s = raw;
    while (!s.empty() && (s[s.size() - 1] == '\r' || s[s.size() - 1] == '\n'))
        s.erase(s.size() - 1);
    size_t n = s.size();
    size_t i = 0;
    while (i < n && isBlank(s[i])) i++;
    if (i < n && (s[i] == '+' || s[i] == '-')) i++;
    size_t digitsStart = i;
    while (i < n && isdigit((unsigned char)s[i])) i++;
    if (i == digitsStart) return false;
    if (i < n && s[i] == '.') {
        i++;
        size_t fracStart = i;
        while (i < n && isdigit((unsigned char)s[i])) i++;
        if (i == fracStart) return false;
    }
    if (i < n && (s[i] == 'e' || s[i] == 'E')) {
        i++;
        if (i < n && (s[i] == '+' || s[i] == '-')) i++;
        size_t expStart = i;
        while (i < n && isdigit((unsigned char)s[i])) i++;
        if (i == expStart) return false;
    }
    while (i < n && isBlank(s[i])) i++;
    return i == n;
}

int main() {
    int T;
    if (!(cin >> T)) return 0;
    string dummy;
    getline(cin, dummy);
    for (int tc = 0; tc < T; tc++) {
        string line;
        if (!getline(cin, line)) line = "";
        cout << (checkNumber(line) ? "LEGAL" : "ILLEGAL") << "\n";
    }
    return 0;
}
