// POJ 3645 - Smeech
// Model: claude-sonnet-5
// Approach: recursive-descent evaluation of expected value.
// An expression is either an integer, or (p e1 e2) representing
// x+y with probability p and x-y otherwise, so its expectation is
// E1 + (2p-1)*E2. Tokens are read with a custom tokenizer that
// treats '(' and ')' as standalone tokens even when glued to
// adjacent characters (e.g. "(.5" or "9)"), since the sample shows
// no guaranteed whitespace around parentheses. Input is a stream of
// expressions terminated by the literal expression "()", which is
// detected by peeking the token right after '(' before trying to
// parse a probability there.
#include <cstdio>
#include <cstdlib>
#include <string>
using namespace std;

static string nextToken() {
    int c;
    do {
        c = getchar();
    } while (c == ' ' || c == '\n' || c == '\r' || c == '\t');
    if (c == EOF) return "";
    if (c == '(' || c == ')') {
        string s;
        s += (char)c;
        return s;
    }
    string s;
    s += (char)c;
    while (true) {
        c = getchar();
        if (c == EOF || c == ' ' || c == '\n' || c == '\r' || c == '\t' ||
            c == '(' || c == ')') {
            if (c != EOF) ungetc(c, stdin);
            break;
        }
        s += (char)c;
    }
    return s;
}

static double parseExpr() {
    string t = nextToken();
    if (t == "(") {
        string tp = nextToken();
        double p = atof(tp.c_str());
        double e1 = parseExpr();
        double e2 = parseExpr();
        nextToken(); // consume ")"
        return e1 + (2.0 * p - 1.0) * e2;
    } else {
        return atof(t.c_str());
    }
}

int main() {
    while (true) {
        string t = nextToken();
        if (t.empty()) break;
        double val;
        if (t == "(") {
            string t2 = nextToken();
            if (t2 == ")") break; // terminator "()"
            double p = atof(t2.c_str());
            double e1 = parseExpr();
            double e2 = parseExpr();
            nextToken(); // consume ")"
            val = e1 + (2.0 * p - 1.0) * e2;
        } else {
            val = atof(t.c_str());
        }
        if (val < 0 && val > -0.005) val = 0.0;
        printf("%.2f\n", val);
    }
    return 0;
}
