// POJ 2252 - Equation Solver
// Model: claude-sonnet-5
//
// Parse a linear equation over x by recursive-descent following the given
// EBNF grammar, evaluating every subexpression as a pair (a,b) meaning the
// linear form a*x+b. Multiplication of two such pairs is
// (a1*b2 + a2*b1, b1*b2); the problem guarantees a1*a2 is always 0 (no
// genuine x^2 term survives), so this formula is exact even though it
// silently discards a1*a2*x^2. Equation LHS=RHS reduces to (a,b) with
// a = aL-aR, b = bL-bR: a==0 && b==0 -> infinitely many solutions;
// a==0 && b!=0 -> no solution; else x = -b/a.
//
// Numbers in the input are non-negative integers; since the whole equation
// is under 100 characters the accumulated double values stay well inside
// double's exact-integer range, so plain double arithmetic with a small
// epsilon (1e-6) for the zero tests is safe. -b/a can legitimately compute
// as -0.0 (e.g. "x=100000000" style equations with b=0), so a genuine zero
// result is normalized to +0 before printing to avoid "-0.000000".
//
// Board (showmessage 96862) notes the real judge data adds a bare
// "x=<number>" case beyond the sample; the grammar already covers a lone
// 'x' factor so no special casing was needed.
#include <cstdio>
#include <cstring>
#include <cmath>
#include <cctype>
#include <string>
#include <iostream>
using namespace std;

static string s;
static int pos_;

struct LX { double a, b; };

static LX parseExpr();

static void skipSpaces() {
    while (pos_ < (int)s.size() && (s[pos_] == ' ' || s[pos_] == '\t')) pos_++;
}

static LX parseFactor() {
    skipSpaces();
    if (s[pos_] == '(') {
        pos_++;
        LX e = parseExpr();
        skipSpaces();
        if (pos_ < (int)s.size() && s[pos_] == ')') pos_++;
        return e;
    } else if (s[pos_] == 'x') {
        pos_++;
        LX r; r.a = 1; r.b = 0;
        return r;
    } else {
        double val = 0;
        while (pos_ < (int)s.size() && isdigit((unsigned char)s[pos_])) {
            val = val * 10 + (s[pos_] - '0');
            pos_++;
        }
        LX r; r.a = 0; r.b = val;
        return r;
    }
}

static LX mul(LX p, LX q) {
    LX r;
    r.a = p.a * q.b + p.b * q.a;
    r.b = p.b * q.b;
    return r;
}

static LX parseTerm() {
    LX val = parseFactor();
    skipSpaces();
    while (pos_ < (int)s.size() && s[pos_] == '*') {
        pos_++;
        LX f = parseFactor();
        val = mul(val, f);
        skipSpaces();
    }
    return val;
}

static LX parseExpr() {
    LX val = parseTerm();
    skipSpaces();
    while (pos_ < (int)s.size() && (s[pos_] == '+' || s[pos_] == '-')) {
        char op = s[pos_]; pos_++;
        LX t = parseTerm();
        if (op == '+') { val.a += t.a; val.b += t.b; }
        else { val.a -= t.a; val.b -= t.b; }
        skipSpaces();
    }
    return val;
}

int main() {
    string line;
    int cas = 0;
    while (getline(cin, line)) {
        while (!line.empty() && (line[line.size() - 1] == '\r' || line[line.size() - 1] == '\n'))
            line.erase(line.size() - 1);
        if (line.find_first_not_of(" \t") == string::npos) continue;
        s = line;
        pos_ = 0;
        LX left = parseExpr();
        skipSpaces();
        if (pos_ < (int)s.size() && s[pos_] == '=') pos_++;
        LX right = parseExpr();
        double a = left.a - right.a;
        double b = left.b - right.b;
        cas++;
        printf("Equation #%d\n", cas);
        if (fabs(a) < 1e-6) {
            if (fabs(b) < 1e-6) printf("Infinitely many solutions.\n");
            else printf("No solution.\n");
        } else {
            double x = -b / a;
            if (fabs(x) < 1e-6) x = 0;
            printf("x = %.6f\n", x);
        }
        printf("\n");
    }
    return 0;
}
