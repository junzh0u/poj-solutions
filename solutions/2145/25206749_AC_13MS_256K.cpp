// POJ 2145 - Pathological Paths
// Model: claude-sonnet-5
//
// Build the directory/file tree implied by the N canonical file pathnames
// (every non-final segment is a directory, the final segment is a leaf
// file). For each query pathname, walk its segments from the root:
//   - "." keeps the current directory unchanged.
//   - ".." moves to the parent directory (invalid at the root, which has
//     none).
//   - any other segment must name an existing child of the current
//     directory; if it is not the last segment it must be a directory
//     (you cannot descend "through" a file).
// After the walk, if the final position is a file, the pathname is valid
// only when it has no trailing slash (a trailing slash always forces a
// directory reading, per the statement's two independent, chainable
// shortenings: dropping a trailing "index.html" and dropping the slash
// after a directory name). If the final position is a directory
// (including the bare "/" query and any query ending in "." or ".." ,
// which are "always regarded as directory names" regardless of a
// trailing slash), the page is that directory's "index.html" child, if
// one exists and is itself a file; otherwise the pathname resolves to no
// page at all.
//
// A pair is "not found" whenever either side fails to resolve to an
// existing page, "yes" when both resolve to the same file node, "no"
// otherwise. Verified against the official sample and against the
// board's own worked case (message 30437): N=1 file "/index.html" (a
// file directly under root), pair "/" vs "/index.html/index.html" ->
// "/" reaches the file via the index.html shortcut, but
// "/index.html/index.html" is invalid because "index.html" resolved to
// a file and cannot be descended into further, so the pair is
// "not found" -- exactly the case that confused several posters there.
//
// Language kept at the default (C++) since no long long / 64-bit
// arithmetic is needed; all sizes are tiny (N, M <= 100, paths <= 100
// chars).

#include <cstdio>
#include <string>
#include <vector>
#include <map>
using namespace std;

struct Node {
    map<string, int> children;
    int parent;
    bool isFile;
};

static vector<Node> nodes;

static int newNode(int parent, bool isFile) {
    Node n;
    n.parent = parent;
    n.isFile = isFile;
    nodes.push_back(n);
    return (int)nodes.size() - 1;
}

// Split a pathname (always starting with '/') into non-empty segments and
// report whether it ends with a trailing slash. The bare "/" pathname is
// treated as zero segments with an implicit trailing slash (a directory
// reference to the root itself).
static void parsePath(const string &path, vector<string> &segs, bool &trailingSlash) {
    segs.clear();
    if (path == "/") {
        trailingSlash = true;
        return;
    }
    string s = path.substr(1);
    trailingSlash = false;
    if (!s.empty() && s[s.size() - 1] == '/') {
        trailingSlash = true;
        s = s.substr(0, s.size() - 1);
    }
    size_t start = 0;
    for (size_t i = 0; i <= s.size(); ++i) {
        if (i == s.size() || s[i] == '/') {
            segs.push_back(s.substr(start, i - start));
            start = i + 1;
        }
    }
}

// Resolve a query pathname to the index of the file node it refers to, or
// -1 if it does not point to any existing web page.
static int resolve(const string &path) {
    vector<string> segs;
    bool trailingSlash;
    parsePath(path, segs, trailingSlash);

    int cur = 0; // root
    for (size_t i = 0; i < segs.size(); ++i) {
        const string &seg = segs[i];
        bool isLast = (i + 1 == segs.size());
        if (seg == ".") {
            // stays in the same directory
        } else if (seg == "..") {
            if (cur == 0) return -1; // root has no parent
            cur = nodes[cur].parent;
        } else {
            map<string, int>::iterator it = nodes[cur].children.find(seg);
            if (it == nodes[cur].children.end()) return -1;
            int child = it->second;
            if (!isLast && nodes[child].isFile) return -1; // can't descend into a file
            cur = child;
        }
    }

    if (nodes[cur].isFile) {
        if (trailingSlash) return -1;
        return cur;
    } else {
        map<string, int>::iterator it = nodes[cur].children.find("index.html");
        if (it == nodes[cur].children.end()) return -1;
        if (!nodes[it->second].isFile) return -1;
        return it->second;
    }
}

int main() {
    int N, M;
    char buf[210];
    while (scanf("%d %d", &N, &M) == 2) {
        if (N == 0 && M == 0) break;

        nodes.clear();
        newNode(-1, false); // root is node 0, a directory

        for (int i = 0; i < N; ++i) {
            scanf("%s", buf);
            string path(buf);
            vector<string> segs;
            bool trailingSlash;
            parsePath(path, segs, trailingSlash);
            int cur = 0;
            for (size_t j = 0; j < segs.size(); ++j) {
                bool isLast = (j + 1 == segs.size());
                map<string, int>::iterator it = nodes[cur].children.find(segs[j]);
                if (it == nodes[cur].children.end()) {
                    int nd = newNode(cur, isLast);
                    nodes[cur].children[segs[j]] = nd;
                    cur = nd;
                } else {
                    cur = it->second;
                }
            }
        }

        for (int i = 0; i < M; ++i) {
            char b1[210], b2[210];
            scanf("%s", b1);
            scanf("%s", b2);
            int r1 = resolve(string(b1));
            int r2 = resolve(string(b2));
            if (r1 == -1 || r2 == -1) {
                printf("not found\n");
            } else if (r1 == r2) {
                printf("yes\n");
            } else {
                printf("no\n");
            }
        }
    }
    return 0;
}
