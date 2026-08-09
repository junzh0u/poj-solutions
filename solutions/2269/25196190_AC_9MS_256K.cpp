// POJ 2269 - Friends
// Model: claude-sonnet-5
// Approach: sets of the 26 letters A-Z are represented as a 32-bit
// bitmask. Each input line is one expression built from set literals
// {...}, the binary operators + (union), - (difference), * (intersection),
// and parentheses for grouping. '*' binds tighter than '+'/'-', and
// operators of equal precedence associate left to right (per the
// statement, and reconfirmed by the discuss board, where WAs came from
// ignoring left-associativity or from mis-sorting the output). A
// standard two-level recursive-descent parser (expr handles +/-, term
// handles *, factor handles a literal set or a parenthesized expr)
// evaluates each line directly against the bitmask; letters are already
// listed in increasing order inside every set literal, so scanning bits
// 0..25 on output naturally yields alphabetical order. No ambiguity in
// the statement itself; the board's posted official test data (message
// 160284) was used as an extra regression, including the {} / ({}) empty
// set edge cases.
#include <cstdio>
#include <cstring>

static char line[300];
static int pos;

unsigned int parseExpr();

unsigned int parseFactor() {
    if (line[pos] == '(') {
        ++pos; // consume '('
        unsigned int v = parseExpr();
        // line[pos] should be ')'
        ++pos; // consume ')'
        return v;
    } else {
        // must be '{'
        ++pos; // consume '{'
        unsigned int v = 0;
        while (line[pos] != '}') {
            v |= (1u << (line[pos] - 'A'));
            ++pos;
        }
        ++pos; // consume '}'
        return v;
    }
}

unsigned int parseTerm() {
    unsigned int v = parseFactor();
    while (line[pos] == '*') {
        ++pos;
        unsigned int rhs = parseFactor();
        v &= rhs;
    }
    return v;
}

unsigned int parseExpr() {
    unsigned int v = parseTerm();
    while (line[pos] == '+' || line[pos] == '-') {
        char op = line[pos];
        ++pos;
        unsigned int rhs = parseTerm();
        if (op == '+') v |= rhs;
        else v &= ~rhs;
    }
    return v;
}

int main() {
    while (fgets(line, sizeof(line), stdin)) {
        // strip trailing \r\n and any whitespace
        int len = (int)strlen(line);
        while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
            line[--len] = '\0';
        }
        if (len == 0) continue; // skip blank lines, if any
        pos = 0;
        unsigned int result = parseExpr();
        putchar('{');
        for (int i = 0; i < 26; ++i) {
            if (result & (1u << i)) putchar('A' + i);
        }
        putchar('}');
        putchar('\n');
    }
    return 0;
}
