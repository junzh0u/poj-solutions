// POJ 1400 - Nice Expressions
// Model: gpt-5.6-terra
// Parse the expression into its left-associative syntax tree, then print each
// child with parentheses only when precedence or a right-hand non-associative
// operation requires them.
#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct Node {
    char op;
    Node *left;
    Node *right;
    Node(char value) : op(value), left(0), right(0) {}
    Node(char value, Node *a, Node *b) : op(value), left(a), right(b) {}
};

string text;
int pos;

int priority(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 3;
}

Node *parseSum();

Node *parseAtom() {
    if (text[pos] == '(') {
        ++pos;
        Node *node = parseSum();
        ++pos;
        return node;
    }
    return new Node(text[pos++]);
}

Node *parseProduct() {
    Node *node = parseAtom();
    while (pos < (int)text.size() && (text[pos] == '*' || text[pos] == '/')) {
        char op = text[pos++];
        node = new Node(op, node, parseAtom());
    }
    return node;
}

Node *parseSum() {
    Node *node = parseProduct();
    while (pos < (int)text.size() && (text[pos] == '+' || text[pos] == '-')) {
        char op = text[pos++];
        node = new Node(op, node, parseProduct());
    }
    return node;
}

string print(Node *node, char parent, bool isRight) {
    if (node->op >= 'a' && node->op <= 'z') return string(1, node->op);
    string left = print(node->left, node->op, false);
    string right = print(node->right, node->op, true);
    string result = left + node->op + right;
    if (!parent) return result;
    int childPriority = priority(node->op);
    int parentPriority = priority(parent);
    bool needs = childPriority < parentPriority;
    if (childPriority == parentPriority && isRight && (parent == '-' || parent == '/')) needs = true;
    if (needs) return "(" + result + ")";
    return result;
}

void dispose(Node *node) {
    if (!node) return;
    dispose(node->left);
    dispose(node->right);
    delete node;
}

int main() {
    int cases;
    cin >> cases;
    while (cases--) {
        cin >> text;
        pos = 0;
        Node *root = parseSum();
        cout << print(root, 0, false) << '\n';
        dispose(root);
    }
    return 0;
}
