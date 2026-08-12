// POJ 1100 - Dreisam Equations
// Model: gpt-5.6-terra
// Exhaustively assign the at most eleven missing operators in +, -, * order.
// Parentheses are parsed into a tree; every sequence node is evaluated left to
// right, while each parenthesized child is evaluated before its enclosing node.
#include <cctype>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Node {
    bool number;
    long long value;
    int first, last;
    vector<Node *> child;
    Node(long long v, int p) : number(true), value(v), first(p), last(p) {}
    Node() : number(false), value(0), first(-1), last(-1) {}
};

string s;
int pos, numberCount;
vector<char> op;

Node *parseGroup(bool enclosed) {
    Node *result = new Node;
    while (pos < (int)s.size() && s[pos] != ')') {
        if (isspace((unsigned char)s[pos])) {
            ++pos;
            continue;
        }
        Node *item;
        if (s[pos] == '(') {
            ++pos;
            item = parseGroup(true);
        } else {
            long long v = 0;
            while (pos < (int)s.size() && isdigit((unsigned char)s[pos])) {
                v = v * 10 + s[pos++] - '0';
            }
            item = new Node(v, numberCount++);
        }
        result->child.push_back(item);
    }
    if (enclosed && pos < (int)s.size() && s[pos] == ')') ++pos;
    result->first = result->child.front()->first;
    result->last = result->child.back()->last;
    return result;
}

long long evaluate(Node *p) {
    if (p->number) return p->value;
    long long ans = evaluate(p->child[0]);
    for (int i = 1; i < (int)p->child.size(); ++i) {
        long long b = evaluate(p->child[i]);
        char c = op[p->child[i - 1]->last];
        if (c == '+') ans += b;
        else if (c == '-') ans -= b;
        else ans *= b;
    }
    return ans;
}

string render(Node *p) {
    if (p->number) {
        char buf[32];
        sprintf(buf, "%lld", p->value);
        return buf;
    }
    string ans;
    for (int i = 0; i < (int)p->child.size(); ++i) {
        if (i) ans += op[p->child[i - 1]->last];
        if (p->child[i]->number) ans += render(p->child[i]);
        else ans += "(" + render(p->child[i]) + ")";
    }
    return ans;
}

bool search(Node *root, long long target, int at) {
    if (at == numberCount - 1) return evaluate(root) == target;
    const char choices[3] = {'+', '-', '*'};
    for (int i = 0; i < 3; ++i) {
        op[at] = choices[i];
        if (search(root, target, at + 1)) return true;
    }
    return false;
}

int main() {
    string line;
    int tc = 0;
    while (getline(cin, line)) {
        string compact;
        for (int i = 0; i < (int)line.size(); ++i)
            if (!isspace((unsigned char)line[i])) compact += line[i];
        if (compact == "0") break;
        int eq = compact.find('=');
        long long target = 0;
        for (int i = 0; i < eq; ++i) target = target * 10 + compact[i] - '0';
        s = line.substr(line.find('=') + 1);
        pos = numberCount = 0;
        Node *root = parseGroup(false);
        op.assign(numberCount - 1, '+');
        cout << "Equation #" << ++tc << ":\n";
        if (search(root, target, 0)) cout << target << "=" << render(root) << "\n\n";
        else cout << "Impossible\n\n";
    }
    return 0;
}
