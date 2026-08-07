// POJ 2246 - Matrix Chain Multiplication
// Model: claude-opus-5
//
// Evaluate a fully-parenthesised matrix product in the order the parentheses
// specify and report the number of elementary multiplications, or "error" if
// two adjacent factors do not conform.
//
// Approach: one linear scan per expression with an explicit operand stack, so
// no recursion depth limit. A capital letter pushes that matrix's (rows, cols);
// ')' pops the two operands B then A, requires A.cols == B.rows, adds
// A.rows * A.cols * B.cols to the running cost and pushes (A.rows, B.cols);
// '(' is ignored. The grammar guarantees a ')' always closes exactly one
// binary product, so the stack reconstructs the parse tree exactly.
//
// Notes on the statement:
//  - The EBNF admits no implicit concatenation ("AB") and no redundant
//    parentheses around a single matrix ("(A)"), but the leftover checks
//    (stack underflow, exactly one operand left at the end) cost nothing.
//  - "error" propagates: the discuss board's test (A(B(ID))) is an error
//    because (ID) already is, even though B's columns would match I's rows.
//    Scanning stops at the first mismatch and the whole line prints "error".
//  - No bound is given on the matrix dimensions, so the cost accumulates in
//    long long; a chain of 10^5-sized matrices already overflows 32 bits.
//
// Expressions are read with operator>>, which skips blank lines and the CR of
// CRLF input on its own, and stops at EOF.

#include <iostream>
#include <string>
#include <vector>

using namespace std;

int R[26], C[26];
bool known[26];

int main() {
    int n;
    if (!(cin >> n)) return 0;
    for (int i = 0; i < 26; i++) known[i] = false;
    for (int i = 0; i < n; i++) {
        string name;
        int r, c;
        cin >> name >> r >> c;
        int id = name[0] - 'A';
        if (id >= 0 && id < 26) { R[id] = r; C[id] = c; known[id] = true; }
    }

    string e;
    while (cin >> e) {
        vector<int> sr(e.size() + 1), sc(e.size() + 1);
        int top = 0;
        long long cost = 0;
        bool bad = false;
        for (size_t i = 0; i < e.size() && !bad; i++) {
            char ch = e[i];
            if (ch >= 'A' && ch <= 'Z') {
                int id = ch - 'A';
                if (!known[id]) { bad = true; break; }
                sr[top] = R[id]; sc[top] = C[id]; top++;
            } else if (ch == ')') {
                if (top < 2) { bad = true; break; }
                int br = sr[top - 1], bc = sc[top - 1];
                int ar = sr[top - 2], ac = sc[top - 2];
                top -= 2;
                if (ac != br) { bad = true; break; }
                cost += (long long)ar * ac * bc;
                sr[top] = ar; sc[top] = bc; top++;
            }
        }
        if (bad || top != 1) cout << "error" << "\n";
        else cout << cost << "\n";
    }
    return 0;
}
