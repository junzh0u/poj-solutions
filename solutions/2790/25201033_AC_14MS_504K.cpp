// POJ 2790 - Consecutive ones
// Model: claude-sonnet-5
// Approach: PQ-tree construction (Booth-Lueker style REDUCE), implemented via
// straightforward (non-linear-time but O(n*m), plenty fast for n,m<=400)
// recursive template matching. Each row's set of columns is REDUCEd into the
// tree so that those columns become forced-consecutive.
//
// PQ-tree node types: PNODE (children in arbitrary order/any permutation),
// QNODE (children in a fixed order up to a full reversal), LEAF (a column).
// REDUCE(S): label every leaf FULL (in S) or EMPTY; then, starting from the
// pertinent root PR (the deepest node whose subtree contains all of S),
// recursively restructure PR's subtree so all FULL leaves end up contiguous.
// A P-node that is the pertinent root keeps its empty children individually
// free and just gains the merged run as one more free child (templates
// P2/P4/P6); a non-root P-node must instead present a clean boundary to its
// parent, so it bundles its empty children into one group and converts to a
// Q-node [empty..., (partial), full...] (P3/P5). A Q-node finds the maximal
// contiguous run of non-empty children (anywhere in the list if it is the
// pertinent root, else touching at least one end) and orients any partial
// boundary child so its full side points into the run.
//
// A "partial" node's orientation (which side is the full side) is tracked
// explicitly and honoured everywhere a child is reused, since a Q-node's
// stored order is fixed up to a *global* reversal only: reversing a node
// therefore has to recursively re-flip every nested partial child's
// orientation too (reverseNode does this unconditionally -- always safe,
// since reversal is a symmetry at every node).
//
// The problem's uniqueness guarantee means that after every row has been
// applied, no genuine freedom remains beyond which of the two ends of each
// node's children holds column 0's branch. Extraction (placeCol0First) walks
// down from the root, and at each node swaps (P-node) or fully reverses
// (Q-node, via reverseNode) so that column 0's branch is the first child;
// the plain frontier of the resulting tree is then the answer.

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

enum { LEAF = 0, PNODE = 1, QNODE = 2 };

struct Node {
    int type;
    int leafId;
    vector<int> ch;
    Node() : type(PNODE), leafId(-1) {}
    Node(int t) : type(t), leafId(-1) {}
};

static vector<Node> nodes;
static int root;
static vector<char> inS;
static vector<int> cnt, tot;

int newNode(int type) {
    nodes.push_back(Node(type));
    return (int)nodes.size() - 1;
}

int bundle(vector<int>& v) {
    if (v.empty()) return -1;
    if (v.size() == 1) return v[0];
    int p = newNode(PNODE);
    nodes[p].ch = v;
    return p;
}

// Reversing a node's top-level child order is always a valid symmetry (the
// defining one for a Q-node; a free reordering for a P-node), but any child
// that was itself oriented relative to its neighbours (a partial node built
// during REDUCE, whose "full side" was chosen to face a specific neighbour)
// must be correspondingly re-oriented too -- otherwise its nested full/empty
// split ends up facing the wrong way once its surroundings flip. A full
// recursive reversal (reverse this node's children, then recursively do the
// same to every child) is always safe and keeps every nested orientation
// consistent, so reverseNode always performs it, all the way down.
void reverseNode(int x) {
    if (nodes[x].type == LEAF) return;
    reverse(nodes[x].ch.begin(), nodes[x].ch.end());
    for (size_t i = 0; i < nodes[x].ch.size(); ++i) reverseNode(nodes[x].ch[i]);
}

void computeCntTot(int u) {
    if (nodes[u].type == LEAF) {
        tot[u] = 1;
        cnt[u] = inS[nodes[u].leafId] ? 1 : 0;
        return;
    }
    int c = 0, t = 0;
    // copy children indices first since nodes vector won't be mutated here
    size_t sz = nodes[u].ch.size();
    for (size_t i = 0; i < sz; ++i) {
        int ci = nodes[u].ch[i];
        computeCntTot(ci);
        c += cnt[ci];
        t += tot[ci];
    }
    cnt[u] = c;
    tot[u] = t;
}

enum { LBL_EMPTY = 0, LBL_FULL = 1, LBL_PARTIAL = 2 };

// fullOnRight is only meaningful when label == LBL_PARTIAL: true means this
// node's children, read left to right, go [empty-ish..., full-ish...]
// (canonical); false means [full-ish..., empty-ish...] (mirrored).
struct ProcResult {
    int label;
    int node;
    bool fullOnRight;
};

// Ensure a partial child's orientation matches `wantFullOnRight`; reverses in
// place if not, and updates the flag.
void orient(int nodeIdx, bool& flag, bool wantFullOnRight) {
    if (flag != wantFullOnRight) {
        reverseNode(nodeIdx);
        flag = wantFullOnRight;
    }
}

ProcResult process(int u, bool isRoot) {
    int type = nodes[u].type;
    if (type == PNODE) {
        vector<int> childrenCopy = nodes[u].ch; // snapshot, since nodes vector may grow
        vector<int> Es, Fs, Ps;
        vector<char> Pflag;
        for (size_t i = 0; i < childrenCopy.size(); ++i) {
            int c = childrenCopy[i];
            if (cnt[c] == 0) {
                Es.push_back(c);
            } else if (cnt[c] == tot[c]) {
                Fs.push_back(c);
            } else {
                ProcResult r = process(c, false);
                Ps.push_back(r.node);
                Pflag.push_back(r.fullOnRight ? 1 : 0);
            }
        }
        if (Es.empty() && Ps.empty()) {
            // all children full
            ProcResult rr; rr.label = LBL_FULL; rr.node = u; rr.fullOnRight = true; return rr;
        }
        int Fbundle = bundle(Fs);
        if (isRoot) {
            // P2/P4/P6: X is the pertinent root, so it need not expose a
            // clean boundary to a parent. Keep X itself as a P-node: the
            // empty children stay individually free (they may end up on
            // either side of the merged run, or interleaved among each
            // other), and the merged run (bundling Fs with any partial
            // children) becomes just one more free child of X.
            int runNode;
            if (Ps.empty()) {
                runNode = Fbundle; // Fbundle must exist: Fs nonempty here
            } else if (Ps.size() == 1) {
                bool f0 = (Pflag[0] != 0);
                orient(Ps[0], f0, true);
                Pflag[0] = f0 ? 1 : 0;
                int q = newNode(QNODE);
                nodes[q].ch.push_back(Ps[0]);
                if (Fbundle != -1) nodes[q].ch.push_back(Fbundle);
                runNode = q;
            } else if (Ps.size() >= 2) {
                int p1 = Ps[0];
                int p2 = Ps[1];
                bool f0 = (Pflag[0] != 0);
                orient(p1, f0, true);
                Pflag[0] = f0 ? 1 : 0;
                bool f1 = (Pflag[1] != 0);
                orient(p2, f1, false);
                Pflag[1] = f1 ? 1 : 0;
                int q = newNode(QNODE);
                nodes[q].ch.push_back(p1);
                if (Fbundle != -1) nodes[q].ch.push_back(Fbundle);
                nodes[q].ch.push_back(p2);
                for (size_t i = 2; i < Ps.size(); ++i) nodes[q].ch.push_back(Ps[i]);
                runNode = q;
            }
            nodes[u].ch.clear();
            for (size_t i = 0; i < Es.size(); ++i) nodes[u].ch.push_back(Es[i]);
            nodes[u].ch.push_back(runNode);
            ProcResult r; r.label = LBL_PARTIAL; r.node = u; r.fullOnRight = true; return r;
        }
        // P3/P5 (non-root): X must present a clean [empty..., full...]
        // boundary to its parent, so bundle the empty children together.
        int Ebundle = bundle(Es);
        if (Ps.empty()) {
            int q = newNode(QNODE);
            if (Ebundle != -1) nodes[q].ch.push_back(Ebundle);
            if (Fbundle != -1) nodes[q].ch.push_back(Fbundle);
            ProcResult r; r.label = LBL_PARTIAL; r.node = q; r.fullOnRight = true; return r;
        } else if (Ps.size() == 1) {
            // p1 sits between Ebundle (if any) and Fbundle (if any); its own
            // full side must face right (toward Fbundle / away from Ebundle).
            { bool f0 = (Pflag[0] != 0); orient(Ps[0], f0, true); Pflag[0] = f0 ? 1 : 0; }
            int q = newNode(QNODE);
            if (Ebundle != -1) nodes[q].ch.push_back(Ebundle);
            nodes[q].ch.push_back(Ps[0]);
            if (Fbundle != -1) nodes[q].ch.push_back(Fbundle);
            ProcResult r; r.label = LBL_PARTIAL; r.node = q; r.fullOnRight = true; return r;
        } else {
            // Ps.size() >= 2 shouldn't occur for a non-root node given a
            // feasible instance; handle defensively the same way as root.
            int p1 = Ps[0];
            int p2 = Ps[1];
            { bool f0 = (Pflag[0] != 0); orient(p1, f0, true); Pflag[0] = f0 ? 1 : 0; }
            { bool f1 = (Pflag[1] != 0); orient(p2, f1, false); Pflag[1] = f1 ? 1 : 0; }
            int q = newNode(QNODE);
            nodes[q].ch.push_back(p1);
            if (Fbundle != -1) nodes[q].ch.push_back(Fbundle);
            nodes[q].ch.push_back(p2);
            if (Ebundle != -1) nodes[q].ch.push_back(Ebundle);
            for (size_t i = 2; i < Ps.size(); ++i) nodes[q].ch.push_back(Ps[i]);
            ProcResult r; r.label = LBL_PARTIAL; r.node = q; r.fullOnRight = true; return r;
        }
    } else {
        // QNODE
        vector<int> childrenCopy = nodes[u].ch;
        int n = (int)childrenCopy.size();
        vector<int> lbl(n), resNode(n);
        vector<char> flag(n, 0);
        for (int i = 0; i < n; ++i) {
            int c = childrenCopy[i];
            if (cnt[c] == 0) {
                lbl[i] = LBL_EMPTY; resNode[i] = c;
            } else if (cnt[c] == tot[c]) {
                lbl[i] = LBL_FULL; resNode[i] = c;
            } else {
                ProcResult r = process(c, false);
                lbl[i] = LBL_PARTIAL; resNode[i] = r.node; flag[i] = r.fullOnRight ? 1 : 0;
            }
        }
        int i = 0; while (i < n && lbl[i] == LBL_EMPTY) i++;
        int j = n - 1; while (j >= 0 && lbl[j] == LBL_EMPTY) j--;
        if (i > j) {
            ProcResult r; r.label = LBL_EMPTY; r.node = u; r.fullOnRight = true; return r;
        }
        bool touchesLeft = (i == 0);
        bool touchesRight = (j == n - 1);
        if (i == j) {
            if (lbl[i] == LBL_PARTIAL) {
                bool want;
                if (i > 0) want = true;            // real empty neighbor to the left
                else if (j < n - 1) want = false;   // real empty neighbor to the right
                else want = (flag[i] != 0);          // no real empty neighbor either side
                bool f = (flag[i] != 0);
                orient(resNode[i], f, want);
                flag[i] = f ? 1 : 0;
            }
        } else {
            if (lbl[i] == LBL_PARTIAL) {
                bool f = (flag[i] != 0);
                orient(resNode[i], f, true); // left boundary: full faces right (inward)
                flag[i] = f ? 1 : 0;
            }
            if (lbl[j] == LBL_PARTIAL) {
                bool f = (flag[j] != 0);
                orient(resNode[j], f, false); // right boundary: full faces left (inward)
                flag[j] = f ? 1 : 0;
            }
        }
        nodes[u].ch.assign(resNode.begin(), resNode.end());
        int label;
        bool outFlag = true;
        if (touchesLeft && touchesRight && lbl[i] != LBL_PARTIAL && lbl[j] != LBL_PARTIAL) {
            label = LBL_FULL;
        } else {
            label = LBL_PARTIAL;
            if (touchesRight && !touchesLeft) {
                outFlag = true; // [plain empty head][run] -> empty...full
            } else if (touchesLeft && !touchesRight) {
                outFlag = false; // [run][plain empty tail] -> full...empty
            } else {
                // touches both ends: no external plain-empty anchor other than
                // whichever boundary(ies) are partial.
                if (i == j) {
                    outFlag = (flag[i] != 0);
                } else if (lbl[i] == LBL_PARTIAL && lbl[j] != LBL_PARTIAL) {
                    outFlag = true;
                } else if (lbl[j] == LBL_PARTIAL && lbl[i] != LBL_PARTIAL) {
                    outFlag = false;
                } else {
                    outFlag = true; // both partial (root-only) or neither: unused upstream
                }
            }
        }
        ProcResult r; r.label = label; r.node = u; r.fullOnRight = outFlag; return r;
    }
}

struct PRInfo { int node, parent, idx; };

PRInfo findPR(int Ssize) {
    int cur = root, par = -1, idx = -1;
    while (true) {
        if (nodes[cur].type == LEAF) {
            PRInfo pr; pr.node = cur; pr.parent = par; pr.idx = idx; return pr;
        }
        int nxt = -1, nxtIdx = -1;
        vector<int>& ch = nodes[cur].ch;
        for (size_t i = 0; i < ch.size(); ++i) {
            if (cnt[ch[i]] == Ssize) { nxt = ch[i]; nxtIdx = (int)i; break; }
        }
        if (nxt == -1) {
            PRInfo pr; pr.node = cur; pr.parent = par; pr.idx = idx; return pr;
        }
        par = cur; idx = nxtIdx; cur = nxt;
    }
}

void reduceRow(vector<int>& cols, int m) {
    int Ssize = (int)cols.size();
    if (Ssize <= 1) return;
    for (int i = 0; i < m; ++i) inS[i] = 0;
    for (size_t i = 0; i < cols.size(); ++i) inS[cols[i]] = 1;
    size_t need = nodes.size();
    if (cnt.size() < need) { cnt.resize(need + 16); tot.resize(need + 16); }
    computeCntTot(root);
    PRInfo pr = findPR(Ssize);
    if (nodes[pr.node].type == LEAF) return; // single element, nothing to do
    ProcResult r = process(pr.node, true);
    if (pr.parent == -1) {
        root = r.node;
    } else {
        nodes[pr.parent].ch[pr.idx] = r.node;
    }
}

vector<char> hasCol0;

void computeHasCol0(int u) {
    if (nodes[u].type == LEAF) {
        hasCol0[u] = (nodes[u].leafId == 0) ? 1 : 0;
        return;
    }
    int has = 0;
    for (size_t i = 0; i < nodes[u].ch.size(); ++i) {
        int c = nodes[u].ch[i];
        computeHasCol0(c);
        if (hasCol0[c]) has = 1;
    }
    hasCol0[u] = has;
}

// Ensures that, within u's subtree (if it contains column 0), column 0 ends
// up at the very front of u's own frontier -- using only operations that
// preserve validity (free P-node reordering, or a full Q-node reversal).
void placeCol0First(int u) {
    if (nodes[u].type == LEAF) return;
    vector<int>& ch = nodes[u].ch;
    int colIdx = -1;
    for (size_t i = 0; i < ch.size(); ++i) {
        if (hasCol0[ch[i]]) { colIdx = (int)i; break; }
    }
    if (colIdx == -1) return; // column 0 not in this subtree
    if (nodes[u].type == PNODE) {
        if (colIdx != 0) swap(ch[0], ch[colIdx]);
    } else {
        if (colIdx != 0) {
            // Column 0's branch must already sit at one end of a Q-node's
            // child list (guaranteed by the input's promised feasibility);
            // a full reversal brings it to the front while keeping every
            // nested orientation consistent.
            reverseNode(u);
        }
    }
    placeCol0First(ch[0]);
}

void frontier(int u, vector<int>& out) {
    if (nodes[u].type == LEAF) {
        out.push_back(nodes[u].leafId);
        return;
    }
    vector<int>& ch = nodes[u].ch;
    for (size_t i = 0; i < ch.size(); ++i) frontier(ch[i], out);
}

int main() {
    int n, m;
    if (scanf("%d", &n) != 1) return 0;
    scanf("%d", &m);
    vector<string> rows(n);
    for (int i = 0; i < n; ++i) {
        char buf[420];
        scanf("%s", buf);
        rows[i] = buf;
    }

    nodes.clear();
    nodes.reserve(8192);
    root = newNode(PNODE);
    for (int j = 0; j < m; ++j) {
        int lf = newNode(LEAF);
        nodes[lf].leafId = j;
        nodes[root].ch.push_back(lf);
    }
    inS.assign(m, 0);
    cnt.assign(nodes.size() + 16, 0);
    tot.assign(nodes.size() + 16, 0);

    for (int i = 0; i < n; ++i) {
        vector<int> cols;
        cols.reserve(m);
        for (int j = 0; j < m; ++j) if (rows[i][j] == '1') cols.push_back(j);
        reduceRow(cols, m);
    }

    hasCol0.assign(nodes.size() + 16, 0);
    computeHasCol0(root);
    placeCol0First(root);
    vector<int> out;
    out.reserve(m);
    frontier(root, out);

    for (size_t i = 0; i < out.size(); ++i) {
        printf("%d\n", out[i]);
    }
    return 0;
}
