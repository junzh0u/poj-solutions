// POJ 1145 - Tree Summing
// Model: claude-opus-5
//
// Each test case is an integer target followed by a binary tree written as the
// LISP S-expression   tree ::= () | (integer tree tree).  Answer "yes" iff some
// root-to-leaf path sums to the target.  Input is a stream of such pairs read
// to EOF; a tree may be spread over several lines with arbitrary whitespace,
// so nothing here is line-oriented -- the whole input is one character stream.
//
// Parsed with an explicit stack rather than recursion, so an adversarially deep
// chain cannot blow the judge's stack.  A frame is pushed for every non-empty
// node, carrying the path sum down to that node; "()" pushes nothing and only
// bumps its parent's empty-child counter.  On the closing ')' of a node, it is
// a leaf exactly when both of its children were the empty tree, and only then
// is its path sum compared against the target.
//
// Pitfalls (statement + discuss board):
//   * Node values may be negative, so no pruning on the running sum is valid.
//   * "()" is not a node.  Counting each "()" as a leaf invents leaves that do
//     not exist -- e.g. 9 (5(4(3(2()())())())()) must print "no", since the
//     only leaf is 2 and the single path sums to 14.
//   * An empty tree has no root-to-leaf paths at all, so "0 ()" is "no", not
//     "yes": the statement says such a query "must be answered negatively".
//   * A node with one empty and one non-empty child is not a leaf.
// Sums are accumulated in long long, since neither the values nor the tree
// depth are bounded by the statement.

#include <cstdio>
#include <vector>

using namespace std;

static const int BUFSZ = 1 << 16;
static char ibuf[BUFSZ];
static int ipos = 0, ilen = 0;

static int peekc() {
    if (ipos == ilen) {
        ilen = (int)fread(ibuf, 1, BUFSZ, stdin);
        ipos = 0;
        if (ilen <= 0) return -1;
    }
    return (unsigned char)ibuf[ipos];
}

static bool isws(int c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v'; }

static int skipws() {
    int c = peekc();
    while (c != -1 && isws(c)) { ipos++; c = peekc(); }
    return c;
}

/* Reads an optionally signed decimal integer.  Returns false at EOF. */
static bool readInt(long long &out) {
    int c = skipws();
    if (c == -1) return false;
    bool neg = false;
    if (c == '-' || c == '+') { neg = (c == '-'); ipos++; c = peekc(); }
    long long v = 0;
    while (c != -1 && c >= '0' && c <= '9') { v = v * 10 + (c - '0'); ipos++; c = peekc(); }
    out = neg ? -v : v;
    return true;
}

struct Frame {
    long long sum;      /* path sum from the root down to and including this node */
    int emptyChildren;  /* how many of this node's children were the empty tree */
};

int main() {
    long long target;
    vector<Frame> st;
    while (readInt(target)) {
        st.clear();
        bool found = false;
        bool done = false;
        while (!done) {
            int c = skipws();
            if (c == -1) { done = true; break; }
            if (c == '(') {
                ipos++;
                int c2 = skipws();
                if (c2 == ')') {
                    /* empty tree: contributes no node and no root-to-leaf path */
                    ipos++;
                    if (st.empty()) done = true;       /* the whole tree was "()" */
                    else st.back().emptyChildren++;
                } else {
                    long long v;
                    readInt(v);
                    Frame f;
                    f.sum = (st.empty() ? 0LL : st.back().sum) + v;
                    f.emptyChildren = 0;
                    st.push_back(f);
                }
            } else if (c == ')') {
                ipos++;
                if (st.empty()) { done = true; break; }  /* defensive */
                Frame f = st.back();
                st.pop_back();
                /* a leaf is exactly a node whose two subtrees are both empty */
                if (f.emptyChildren == 2 && f.sum == target) found = true;
                if (st.empty()) done = true;
                /* a non-empty child does not bump emptyChildren of the parent */
            } else {
                ipos++; /* defensive: skip anything unexpected */
            }
        }
        printf(found ? "yes\n" : "no\n");
    }
    return 0;
}
