// POJ 1108 - Split Windows
// Model: claude-sonnet-5
//
// Parse the preorder traversal into a binary tree: a leaf is a label, an
// internal '-' node splits into top(left subtree)/bottom(right subtree),
// and an internal '|' node splits into left(left subtree)/right(right
// subtree). Bottom-up compute each subtree's natural minimum size in grid
// units (leaf = 2x2; '-' node sums heights and takes the max of widths;
// '|' node sums widths and takes the max of heights). Then top-down assign
// each node its final size starting from the root's own natural size (the
// overall minimum), passing the shared dimension through unchanged and
// distributing the summed dimension proportionally to the two children's
// natural values, always rounding the first (top / left) child up to the
// next integer and giving the remainder to the second, per the statement's
// example (D1=ceil(d1*D/d), D2=D-D1). A grid of (H+1) x (W+1) characters is
// then rendered leaf by leaf: pass 1 draws every leaf's four border lines
// (dashes / bars), pass 2 stamps every leaf's four corners with '*'
// (overriding any line character that coincidentally lands on a corner
// from a neighboring, larger leaf's border — this reproduces the
// statement's T-junction corners), and pass 3 stamps each leaf's own
// upper-left corner with its label, overriding the '*' placed there.
//
// The statement's own worked example (Tree 4 / Window 4, D=10, d=6,
// d1=2, d2=4 -> D1=4, D2=6) was used to validate the stretching rule
// directly against the text before trusting it on the samples.
#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;

const int MAXN = 60;
int type_[MAXN]; // 0 leaf, '-' or '|'
char label_[MAXN];
int lft[MAXN], rgt[MAXN];
int W_[MAXN], H_[MAXN];
int nodeCount;
string s;
int pos_;

int newNode() { return nodeCount++; }

int parseTree() {
    int node = newNode();
    char c = s[pos_];
    if (c == '|' || c == '-') {
        pos_++;
        int l = parseTree();
        int r = parseTree();
        type_[node] = c;
        lft[node] = l;
        rgt[node] = r;
        if (c == '-') {
            H_[node] = H_[l] + H_[r];
            W_[node] = max(W_[l], W_[r]);
        } else {
            W_[node] = W_[l] + W_[r];
            H_[node] = max(H_[l], H_[r]);
        }
    } else {
        pos_++;
        type_[node] = 0;
        label_[node] = c;
        W_[node] = 2;
        H_[node] = 2;
    }
    return node;
}

struct Leaf {
    int r0, c0, w, h;
    char label;
};
vector<Leaf> leaves;

void assignRect(int node, int r0, int c0, int W, int H) {
    if (type_[node] == 0) {
        Leaf lf;
        lf.r0 = r0; lf.c0 = c0; lf.w = W; lf.h = H; lf.label = label_[node];
        leaves.push_back(lf);
        return;
    }
    if (type_[node] == '-') {
        int l = lft[node], r = rgt[node];
        int h1 = H_[l], h2 = H_[r];
        int H1 = (h1 * H + (h1 + h2 - 1)) / (h1 + h2);
        int H2 = H - H1;
        assignRect(l, r0, c0, W, H1);
        assignRect(r, r0 + H1, c0, W, H2);
    } else {
        int l = lft[node], r = rgt[node];
        int w1 = W_[l], w2 = W_[r];
        int W1 = (w1 * W + (w1 + w2 - 1)) / (w1 + w2);
        int W2 = W - W1;
        assignRect(l, r0, c0, W1, H);
        assignRect(r, r0, c0 + W1, W2, H);
    }
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    for (int tc = 1; tc <= n; tc++) {
        char buf[200];
        scanf("%s", buf);
        s = string(buf);
        pos_ = 0;
        nodeCount = 0;
        leaves.clear();
        int root = parseTree();
        int totalW = W_[root], totalH = H_[root];
        assignRect(root, 0, 0, totalW, totalH);

        int rows = totalH + 1, cols = totalW + 1;
        vector<string> grid(rows, string(cols, ' '));

        // Pass 1: border lines
        for (size_t i = 0; i < leaves.size(); i++) {
            Leaf &lf = leaves[i];
            for (int c = lf.c0; c <= lf.c0 + lf.w; c++) {
                grid[lf.r0][c] = '-';
                grid[lf.r0 + lf.h][c] = '-';
            }
            for (int r = lf.r0; r <= lf.r0 + lf.h; r++) {
                grid[r][lf.c0] = '|';
                grid[r][lf.c0 + lf.w] = '|';
            }
        }
        // Pass 2: corners
        for (size_t i = 0; i < leaves.size(); i++) {
            Leaf &lf = leaves[i];
            grid[lf.r0][lf.c0] = '*';
            grid[lf.r0][lf.c0 + lf.w] = '*';
            grid[lf.r0 + lf.h][lf.c0] = '*';
            grid[lf.r0 + lf.h][lf.c0 + lf.w] = '*';
        }
        // Pass 3: labels
        for (size_t i = 0; i < leaves.size(); i++) {
            Leaf &lf = leaves[i];
            grid[lf.r0][lf.c0] = lf.label;
        }

        printf("%d\n", tc);
        for (int r = 0; r < rows; r++) {
            printf("%s\n", grid[r].c_str());
        }
    }
    return 0;
}
