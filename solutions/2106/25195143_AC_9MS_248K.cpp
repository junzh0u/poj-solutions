// POJ 2106 - Boolean Expressions
// Model: claude-opus-5
//
// Recursive-descent evaluation of a boolean expression over V/F with
// ! (highest), & , | (lowest) and parentheses:
//   expr   := term ('|' term)*
//   term   := factor ('&' factor)*
//   factor := '!' factor | '(' expr ')' | 'V' | 'F'
// Spaces may appear anywhere, so the line is read with getline (not scanf
// "%s") and stripped of blanks/CR before parsing; the raw line can therefore
// be much longer than the 100-symbol bound the statement states.
// Input runs to EOF and the last line need not end with a newline.
// Whitespace-only lines are skipped so they cannot consume an expression
// number.  Chains such as !!!F work because 'factor' recurses on '!'.
// Output format: one "Expression k: X" per line (the statement's "separate
// with a new line" is the ordinary line terminator).

#include <cstdio>
#include <iostream>
#include <string>

static std::string s;
static size_t pos;

static bool parseExpr();

static bool parseFactor() {
    if (pos >= s.size()) return false;
    char c = s[pos];
    if (c == '!') {
        ++pos;
        return !parseFactor();
    }
    if (c == '(') {
        ++pos;
        bool v = parseExpr();
        if (pos < s.size() && s[pos] == ')') ++pos;
        return v;
    }
    ++pos;
    return c == 'V';
}

static bool parseTerm() {
    bool v = parseFactor();
    while (pos < s.size() && s[pos] == '&') {
        ++pos;
        bool r = parseFactor();
        v = v && r;
    }
    return v;
}

static bool parseExpr() {
    bool v = parseTerm();
    while (pos < s.size() && s[pos] == '|') {
        ++pos;
        bool r = parseTerm();
        v = v || r;
    }
    return v;
}

int main() {
    std::string line;
    int idx = 0;
    while (std::getline(std::cin, line)) {
        s.clear();
        for (size_t i = 0; i < line.size(); ++i) {
            char c = line[i];
            if (c == 'V' || c == 'F' || c == '!' || c == '&' || c == '|' ||
                c == '(' || c == ')')
                s += c;
        }
        if (s.empty()) continue;
        pos = 0;
        bool v = parseExpr();
        ++idx;
        printf("Expression %d: %c\n", idx, v ? 'V' : 'F');
    }
    return 0;
}
