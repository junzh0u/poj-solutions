// POJ 1669 - It's Logical
// Model: claude-sonnet-5
// Approach: The line for each dataset contains two logic expressions
// concatenated with no explicit separator; the statement says it is up to
// the program to determine where one ends and the next begins. A recursive-
// descent parser with the stated C-like precedence (parenthesis, ~, &, ^, |)
// naturally stops consuming input the instant the current expression is
// syntactically complete (a binary-operator loop at each precedence level
// only continues when the very next meaningful character is that level's
// operator), so parsing the line twice in a row splits it correctly without
// any explicit delimiter. Non-token characters (spaces, stray punctuation,
// anything outside a-z/&|^~()) are skipped while scanning. Each expression
// uses at most 10 distinct variables (statement-guaranteed), so equivalence
// is decided by brute-force enumeration of all assignments to the variables
// actually referenced by either expression in the dataset (<=2^10 = 1024
// cases) and comparing results.
#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>
using namespace std;

static string line_;
static size_t pos_;
static bool used[26];

static char peekc() {
    while (pos_ < line_.size()) {
        char c = line_[pos_];
        if ((c >= 'a' && c <= 'z') || c == '&' || c == '|' || c == '^' ||
            c == '~' || c == '(' || c == ')')
            return c;
        pos_++;
    }
    return 0;
}

static char getch() {
    char c = peekc();
    if (c) pos_++;
    return c;
}

// Nodes are stored in a flat pool in creation order. Since a node is always
// built strictly after both of its children (recursive-descent parsing
// bottoms out before combining), the pool index order is already a valid
// postorder/topological order for the whole forest built during a dataset.
// That lets evaluation for each of the up to 2^20 variable assignments be a
// single forward linear pass over the pool instead of a recursive tree walk,
// which matters at the statement's worst case (two expressions, 10 distinct
// variables and 100 operations each, so up to 20 variables total).
struct Node {
    int type; // 0=var,1=not,2=and,3=xor,4=or
    int var;  // for type==0: index 0..25; unused otherwise
    int l, r; // pool indices of children (r unused for var/not)
};

static const int MAXNODES = 4096;
static Node pool[MAXNODES];
static int poolSize;

static int mkNode(int type, int var, int l, int r) {
    Node &n = pool[poolSize];
    n.type = type;
    n.var = var;
    n.l = l;
    n.r = r;
    return poolSize++;
}

// Recursive evaluation (kept, rather than an always-visit-every-node linear
// pass) specifically to keep && / || short-circuiting: on the statement's
// worst case (100 ops, 10 vars per expression, up to 20 total) this measured
// noticeably faster than a flat forward pass that evaluates every node for
// every assignment regardless of whether the result is already decided.
static bool evalIdx(int i, const bool vals[26]) {
    const Node &nd = pool[i];
    switch (nd.type) {
        case 0: return vals[nd.var];
        case 1: return !evalIdx(nd.l, vals);
        case 2: return evalIdx(nd.l, vals) && evalIdx(nd.r, vals);
        case 3: return evalIdx(nd.l, vals) != evalIdx(nd.r, vals);
        case 4: return evalIdx(nd.l, vals) || evalIdx(nd.r, vals);
    }
    return false;
}

static int parseExpr();

static int parsePrimary() {
    char c = peekc();
    if (c == '(') {
        getch();
        int e = parseExpr();
        getch(); // consume matching ')'
        return e;
    }
    getch(); // consume variable letter
    used[c - 'a'] = true;
    return mkNode(0, c - 'a', 0, 0);
}

static int parseNot() {
    if (peekc() == '~') {
        getch();
        int n = parseNot();
        return mkNode(1, 0, n, 0);
    }
    return parsePrimary();
}

static int parseAnd() {
    int n = parseNot();
    while (peekc() == '&') {
        getch();
        int r = parseNot();
        n = mkNode(2, 0, n, r);
    }
    return n;
}

static int parseXor() {
    int n = parseAnd();
    while (peekc() == '^') {
        getch();
        int r = parseAnd();
        n = mkNode(3, 0, n, r);
    }
    return n;
}

static int parseExpr() {
    int n = parseXor();
    while (peekc() == '|') {
        getch();
        int r = parseXor();
        n = mkNode(4, 0, n, r);
    }
    return n;
}

int main() {
    int n;
    if (!(cin >> n)) return 0;
    cin.ignore(1 << 20, '\n');
    for (int tc = 1; tc <= n; tc++) {
        if (!getline(cin, line_)) line_ = "";
        while (!line_.empty() &&
               (line_[line_.size() - 1] == '\r' || line_[line_.size() - 1] == '\n'))
            line_.erase(line_.size() - 1);
        pos_ = 0;
        memset(used, 0, sizeof(used));
        poolSize = 0;
        int e1Root = parseExpr();
        int e2Root = parseExpr();

        int idx[26], k = 0;
        for (int i = 0; i < 26; i++)
            if (used[i]) idx[k++] = i;

        bool vals[26];
        memset(vals, 0, sizeof(vals));
        bool equivalent = true;
        int total = 1 << k;
        for (int mask = 0; mask < total && equivalent; mask++) {
            for (int i = 0; i < k; i++) vals[idx[i]] = (mask >> i) & 1;
            bool r1 = evalIdx(e1Root, vals);
            bool r2 = evalIdx(e2Root, vals);
            if (r1 != r2) equivalent = false;
        }
        printf("Data set %d: %s\n", tc, equivalent ? "Equivalent" : "Different");
    }
    return 0;
}
