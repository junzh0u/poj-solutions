// POJ 1559 - Equation Elation
// Model: gpt-6.1-sol
// Store signed operands separately from binary operators. Repeatedly reduce
// the leftmost multiplication/division, then the leftmost addition/subtraction.
// Unary signs belong to operands; printing numbers normalizes unary plus.
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

void print(const vector<int>& a, const vector<char>& op, const string& var) {
    cout << a[0];
    for (size_t i = 0; i < op.size(); ++i)
        cout << ' ' << op[i] << ' ' << a[i + 1];
    cout << " = " << var << '\n';
}

int main() {
    string line;
    bool first = true;
    while (getline(cin, line)) {
        istringstream in(line);
        vector<int> a;
        vector<char> op;
        int value;
        char c;
        string var;
        if (!(in >> value)) continue;
        a.push_back(value);
        while (in >> c && c != '=') {
            in >> value;
            op.push_back(c);
            a.push_back(value);
        }
        in >> var;
        if (!first) cout << '\n';
        first = false;
        print(a, op, var);
        while (!op.empty()) {
            size_t k = 0;
            while (k < op.size() && op[k] != '*' && op[k] != '/') ++k;
            if (k == op.size()) k = 0;
            if (op[k] == '+') a[k] += a[k + 1];
            else if (op[k] == '-') a[k] -= a[k + 1];
            else if (op[k] == '*') a[k] *= a[k + 1];
            else a[k] /= a[k + 1];
            a.erase(a.begin() + k + 1);
            op.erase(op.begin() + k);
            print(a, op, var);
        }
    }
    return 0;
}
