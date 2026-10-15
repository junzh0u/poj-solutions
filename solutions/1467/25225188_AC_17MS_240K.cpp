// POJ 1467 - Symbolic Derivation
// Model: gpt-6.1-sol
// Recursive descent preserves each operand's original spelling and parentheses.
// Apply the prescribed rules without simplification; merge adjacent signs when
// inserting the second product-rule term, as demonstrated by the samples.
#include <iostream>
#include <string>
#include <cctype>
using namespace std;
struct Expr {
    string text, deriv;
    Expr(string t = "", string d = "") : text(t), deriv(d) {}
};
string s;
unsigned int pos;
string attach(char op, string t) {
    if (!t.empty() && (t[0] == '+' || t[0] == '-')) {
        if (t[0] == '-') op = op == '+' ? '-' : '+';
        t.erase(0, 1);
    }
    return string(1, op) + t;
}
Expr expression();
Expr factor() {
    unsigned int start = pos;
    if (s[pos] == '(') {
        ++pos;
        Expr a = expression();
        ++pos;
        return Expr(s.substr(start, pos-start), "(" + a.deriv + ")");
    }
    if (s[pos] == 'l') {
        pos += 3;
        Expr a = expression();
        ++pos;
        return Expr(s.substr(start, pos-start), "(" + a.deriv + ")/(" + a.text + ")");
    }
    if (s[pos] == 'x') { ++pos; return Expr("x", "1"); }
    if (s[pos] == '+' || s[pos] == '-') ++pos;
    while (pos < s.size() && (isdigit(s[pos]) || s[pos] == '.')) ++pos;
    return Expr(s.substr(start, pos-start), "0");
}
Expr term() {
    Expr a = factor();
    while (pos < s.size() && (s[pos] == '*' || s[pos] == '/')) {
        char op = s[pos++];
        Expr b = factor();
        string d = "(" + a.deriv + "*" + b.text + attach(op == '*' ? '+' : '-', a.text) + "*" + b.deriv + ")";
        if (op == '/') d += "/" + b.text + "^2";
        a = Expr(a.text + op + b.text, d);
    }
    return a;
}
Expr expression() {
    Expr a = term();
    while (pos < s.size() && (s[pos] == '+' || s[pos] == '-')) {
        char op = s[pos++];
        Expr b = term();
        a = Expr(a.text + op + b.text, a.deriv + op + b.deriv);
    }
    return a;
}
int main() {
    while (cin >> s) { pos = 0; cout << expression().deriv << '\n'; }
    return 0;
}
