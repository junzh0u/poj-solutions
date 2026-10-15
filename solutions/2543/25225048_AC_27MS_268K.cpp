// POJ 2543 - Spreadsheet
// Model: gpt-6.1-sol
// Recursive-descent expression evaluation with memoized cell DFS.
// Only cycles reachable from A1 invalidate its value, including self references.
// Read exactly N physical lines: the judge includes blank definition lines.
#include <iostream>
#include <string>
#include <cctype>
using namespace std;
string formula[234];
int state[234], value[234];
bool circular = false;
int cell(int id);
struct Parser {
    const string &s;
    size_t p;
    Parser(const string &text): s(text), p(0) {}
    int expression() {
        int a = term();
        while (p < s.size() && (s[p] == '+' || s[p] == '-')) {
            char op = s[p++]; int b = term();
            if (op == '+') a += b; else a -= b;
        }
        return a;
    }
    int term() {
        int a = factor();
        while (p < s.size() && (s[p] == '*' || s[p] == '/')) {
            char op = s[p++]; int b = factor();
            if (op == '*') a *= b; else a = b == 0 ? 0 : a / b;
        }
        return a;
    }
    int factor() {
        if (p == s.size()) return 0;
        if (s[p] == '+' || s[p] == '-') {
            char op = s[p++]; int a = factor(); return op == '-' ? -a : a;
        }
        if (s[p] == '(') {
            ++p; int a = expression(); ++p; return a;
        }
        if (s[p] >= 'A' && s[p] <= 'Z') {
            int col = s[p++] - 'A'; int row = s[p++] - '1';
            return cell(col * 9 + row);
        }
        int a = 0;
        while (p < s.size() && isdigit(s[p])) a = a * 10 + s[p++] - '0';
        return a;
    }
};
int cell(int id) {
    if (state[id] == 1) { circular = true; return 0; }
    if (state[id] == 2) return value[id];
    state[id] = 1;
    Parser parser(formula[id]);
    value[id] = parser.expression();
    state[id] = 2;
    return value[id];
}
int main() {
    int n; if (!(cin >> n)) return 0;
    string line; getline(cin, line);
    for (int i = 0; i < n && getline(cin, line); ++i) {
        string text;
        for (size_t j = 0; j < line.size(); ++j)
            if (!isspace(line[j])) text += line[j];
        if (text.size() >= 3 && text[2] == '=')
            formula[(text[0] - 'A') * 9 + text[1] - '1'] = text.substr(3);
    }
    int answer = cell(0);
    cout << (circular ? 1000000 : answer) << endl;
    return 0;
}
