// POJ 1539 - Evaluating Simple C Expressions
// Model: claude-sonnet-5
//
// Approach: strip spaces, tokenize into VAR (single letter), UNARY (++ or --,
// always exactly two identical chars in a row), and BINOP (a lone + or -).
// Group tokens into terms separated by BINOP tokens; each term is
// [UNARY?] VAR [UNARY?] with a sign taken from the preceding BINOP (or + for
// the first term). Per the statement's own algorithmic description: all
// prefix ++/-- are applied first (using the new value in the expression),
// then the expression is evaluated with the (now prefix-adjusted) values,
// then all postfix ++/-- are applied (so postfix ops use the old value in
// the expression but still mutate the variable afterward). Verified by hand
// against the discuss board's stress case (message 344171:
// "-- c + b ++ - ++f - ++ d - a++" -> value=-9, a=2 b=3 c=2 d=5 f=7), which
// exercises mixed pre/post ops on multiple variables in one expression.
// Variables that never appear in the expression are not printed; those that
// do are printed in alphabetical order (confirmed by sample 4: input order
// c,f,a but output order a,c,f; sample 5: input order f,c,d,e but output
// order c,d,e,f).
// No ambiguous input (a+++b) or an operator on both sides of one variable is
// guaranteed by the statement, so the greedy two-char-first tokenizer is safe.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

struct Token {
    int type; // 0=VAR,1=UNARY,2=BINOP
    char ch;  // for VAR: the letter; for UNARY/BINOP: '+' or '-'
};

struct Term {
    int sign;
    char var;
    int preOp;  // 0, +1, -1
    int postOp; // 0, +1, -1
};

int main() {
    string line;
    while (getline(cin, line)) {
        // strip trailing \r if present
        while (!line.empty() && (line[line.size() - 1] == '\r' || line[line.size() - 1] == '\n')) {
            line.erase(line.size() - 1);
        }
        // check blank (ignoring whitespace)
        bool blank = true;
        for (size_t i = 0; i < line.size(); ++i) {
            if (line[i] != ' ' && line[i] != '\t') { blank = false; break; }
        }
        if (blank) break;

        // build compact string without spaces/tabs
        string s;
        for (size_t i = 0; i < line.size(); ++i) {
            char c = line[i];
            if (c != ' ' && c != '\t') s.push_back(c);
        }

        // tokenize
        vector<Token> tokens;
        int n = (int)s.size();
        int i = 0;
        while (i < n) {
            char c = s[i];
            if (c == '+' || c == '-') {
                if (i + 1 < n && s[i + 1] == c) {
                    Token t; t.type = 1; t.ch = c;
                    tokens.push_back(t);
                    i += 2;
                } else {
                    Token t; t.type = 2; t.ch = c;
                    tokens.push_back(t);
                    i += 1;
                }
            } else {
                Token t; t.type = 0; t.ch = c;
                tokens.push_back(t);
                i += 1;
            }
        }

        // group into terms
        vector<Term> terms;
        int sign = +1;
        size_t idx = 0;
        size_t ntok = tokens.size();
        while (idx < ntok) {
            if (tokens[idx].type == 2) {
                sign = (tokens[idx].ch == '+') ? +1 : -1;
                idx++;
                continue;
            }
            Term t; t.sign = sign; t.preOp = 0; t.postOp = 0; t.var = 0;
            if (tokens[idx].type == 1) {
                t.preOp = (tokens[idx].ch == '+') ? +1 : -1;
                idx++;
            }
            // now expect VAR
            t.var = tokens[idx].ch;
            idx++;
            if (idx < ntok && tokens[idx].type == 1) {
                t.postOp = (tokens[idx].ch == '+') ? +1 : -1;
                idx++;
            }
            terms.push_back(t);
        }

        int val[26];
        for (int k = 0; k < 26; ++k) val[k] = k + 1;
        bool used[26];
        for (int k = 0; k < 26; ++k) used[k] = false;

        for (size_t k = 0; k < terms.size(); ++k) {
            used[terms[k].var - 'a'] = true;
        }

        // step 1: apply prefix ops
        for (size_t k = 0; k < terms.size(); ++k) {
            if (terms[k].preOp != 0) {
                val[terms[k].var - 'a'] += terms[k].preOp;
            }
        }

        // step 2: evaluate (values stay tiny -- at most 26 terms, each in
        // roughly 0..27 -- so plain int is ample, no long long needed)
        int total = 0;
        for (size_t k = 0; k < terms.size(); ++k) {
            total += terms[k].sign * val[terms[k].var - 'a'];
        }

        // step 3: apply postfix ops
        for (size_t k = 0; k < terms.size(); ++k) {
            if (terms[k].postOp != 0) {
                val[terms[k].var - 'a'] += terms[k].postOp;
            }
        }

        printf("Expression: %s\n", line.c_str());
        printf("    value = %d\n", total);
        for (int k = 0; k < 26; ++k) {
            if (used[k]) {
                printf("    %c = %d\n", 'a' + k, val[k]);
            }
        }
    }
    return 0;
}
