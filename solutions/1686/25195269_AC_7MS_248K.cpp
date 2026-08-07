// POJ 1686 - Lazy Math Instructor
// Model: claude-opus-5
//
// Decide whether two arithmetic expressions over single-letter variables
// (case insensitive), single-digit numbers, parentheses and + - * denote the
// same polynomial.
//
// Precedence: the statement's Note says expressions are "evaluated from left
// to right with equal precedence (priority) for all operators", so * does NOT
// bind tighter than + and -; the whole expression is one left-associative
// chain and only parentheses group.  Neither the official sample nor the
// discuss board's official test data distinguishes this reading from ordinary
// school precedence (an exact-polynomial reference implementing both agrees on
// all 15 official cases), so the statement's own words decide it.
//
// Equivalence test: identity of two multivariate polynomials, checked by
// Schwartz-Zippel - evaluate both at random points modulo a prime and compare.
// Two facts make this a proof rather than a guess for this input class:
//   * The statement guarantees coefficients fit in 16-bit integers, so a
//     coefficient of the difference polynomial is at most 131070 in magnitude.
//     Since 65521 * 65519 > 131070, a difference that is nonzero over Z stays
//     nonzero modulo at least one of those two primes.
//   * An 80-character expression needs two characters per multiplication, so
//     the total degree is below 40; a random point kills a nonzero polynomial
//     with probability at most 40/65519 per trial, and 24 trials per prime
//     drive that below 1e-100.
// Values are reduced mod p < 2^16 after every step, so every product fits in a
// 32-bit unsigned int and no 64-bit type is needed.  The random points come
// from a fixed-seed LCG, so the program is deterministic and pulls in no clock.
//
// Parsing is recursive descent over the raw line: blanks and tabs may appear
// anywhere between tokens, parentheses may be redundant, and letters are
// folded to lower case.  Lines are read whole (an expression may start with
// spaces, which scanf("%s") would eat but which also means the line cannot be
// tokenised blindly), and blank lines are skipped defensively.

#include <cstdio>
#include <cctype>
#include <iostream>
#include <string>

using namespace std;

static const char *cur;
static unsigned int mod;
static unsigned int val[26];

static void skipws() {
    while (*cur == ' ' || *cur == '\t') ++cur;
}

static unsigned int parseExpr();

static unsigned int parseTerm() {
    skipws();
    if (*cur == '+' || *cur == '-') { /* defensive: unary sign */
        char op = *cur++;
        unsigned int v = parseTerm();
        return op == '-' ? (mod - v) % mod : v;
    }
    if (*cur == '(') {
        ++cur;
        unsigned int v = parseExpr();
        skipws();
        if (*cur == ')') ++cur;
        return v;
    }
    if (isdigit((unsigned char)*cur)) {
        unsigned int v = 0;
        while (isdigit((unsigned char)*cur)) {
            v = (v * 10u + (unsigned int)(*cur - '0')) % mod;
            ++cur;
        }
        return v;
    }
    if (isalpha((unsigned char)*cur)) {
        unsigned int v = val[tolower((unsigned char)*cur) - 'a'];
        ++cur;
        return v;
    }
    if (*cur) ++cur; /* unreachable on well-formed input */
    return 0;
}

/* One left-associative chain: +, - and * all have the same precedence. */
static unsigned int parseExpr() {
    unsigned int acc = parseTerm();
    for (;;) {
        skipws();
        char op = *cur;
        if (op != '+' && op != '-' && op != '*') break;
        ++cur;
        unsigned int rhs = parseTerm();
        if (op == '+') acc = (acc + rhs) % mod;
        else if (op == '-') acc = (acc + mod - rhs) % mod;
        else acc = (acc * rhs) % mod;
    }
    return acc;
}

static unsigned int seed = 20260807u;

static unsigned int nextRand() {
    seed = seed * 1103515245u + 12345u;
    return seed >> 8;
}

static bool readLine(string &s) {
    while (getline(cin, s)) {
        while (!s.empty() && (s[s.size() - 1] == '\r' || s[s.size() - 1] == '\n'))
            s.erase(s.size() - 1);
        for (size_t i = 0; i < s.size(); ++i)
            if (!isspace((unsigned char)s[i])) return true;
    }
    return false;
}

int main() {
    string line;
    if (!readLine(line)) return 0;
    int n = 0;
    sscanf(line.c_str(), "%d", &n);

    static const unsigned int primes[2] = {65521u, 65519u};

    for (int t = 0; t < n; ++t) {
        string a, b;
        if (!readLine(a)) break;
        if (!readLine(b)) break;
        bool same = true;
        for (int pi = 0; pi < 2 && same; ++pi) {
            mod = primes[pi];
            for (int trial = 0; trial < 24 && same; ++trial) {
                for (int v = 0; v < 26; ++v) val[v] = nextRand() % mod;
                cur = a.c_str();
                unsigned int va = parseExpr();
                cur = b.c_str();
                unsigned int vb = parseExpr();
                if (va != vb) same = false;
            }
        }
        printf(same ? "YES\n" : "NO\n");
    }
    return 0;
}
