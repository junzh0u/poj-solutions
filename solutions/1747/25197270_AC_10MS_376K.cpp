// POJ 1747 - Expression
// Model: gpt-5.6-terra
// Carry recurrence expressed solely with Sheffer strokes.  If C is the carry
// into a position, the next carry is (A and B) or (C and (A or B)).
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

static string atom(char name, int index) {
    ostringstream out;
    out << name << index;
    return out.str();
}

static string nand_expr(const string& left, const string& right) {
    return "(" + left + "|" + right + ")";
}

static string and_expr(const string& left, const string& right) {
    string t = nand_expr(left, right);
    return nand_expr(t, t);
}

int main() {
    int n;
    cin >> n;

    string carry = and_expr(atom('A', 0), atom('B', 0));
    for (int i = 1; i < n; ++i) {
        string a = atom('A', i);
        string b = atom('B', i);
        string generate = nand_expr(a, b);
        string not_a = nand_expr(a, a);
        string not_b = nand_expr(b, b);
        string propagate_or = nand_expr(not_a, not_b);
        carry = nand_expr(generate, nand_expr(carry, propagate_or));
    }
    cout << carry << '\n';
    return 0;
}
