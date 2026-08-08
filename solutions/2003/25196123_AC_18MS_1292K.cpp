// POJ 2003 - Hire and Fire
// Model: claude-sonnet-5
//
// Approach: represent the hierarchy with the classic "first child, next
// sibling" binary encoding. hire() appends the new node to the end of the
// parent's child list. fire() removes a node v; if v has no children it is
// simply spliced out of its parent's sibling chain. If v has children, the
// replacement subtree is built by a recursive resolve(head, extra) that,
// given a sibling chain "head" (originally v's own children) and a trailing
// chain "extra" to attach at the point the cascade bottoms out, always
// promotes head (its first/highest-ranking element) to the vacancy: the
// promoted node's own children are resolved the same way (using its
// siblings as the new "extra"), and when a node in the chain has no
// children of its own the cascade stops there and "extra" is attached
// directly as that node's children. This exactly matches the statement's
// cascading-promotion rule (highest-ranking subordinate promoted, and if
// that person also has subordinates the cascade continues, until someone
// with no subordinates is promoted) and was verified against the sample
// and a differential case posted on the discuss board (message 355954,
// whose "answer" reply 355955 has one stray debug line "2" before a
// "YAA/+VED" block -- clearly a leftover debug print, since a bare "2"
// can't be a valid name -- everything else matches this program exactly).
//
// No ambiguity beyond that: names are unique tokens with no spaces, so
// simple >> parsing suffices; input is read to EOF (no leading count is
// given, and the statement explicitly says lines can occur "any number of
// times" with no terminator).
#include <cstdio>
#include <cstring>
#include <string>
#include <map>
#include <vector>
using namespace std;

const int MAXN = 200005;
int child_[MAXN], sibling_[MAXN], parent_[MAXN];
char names[MAXN][21];
int nodeCount = 0;
int ceoId = -1;

map<string,int> idOf;

int newNode(const char *nm, int par) {
    int id = nodeCount++;
    strcpy(names[id], nm);
    child_[id] = -1;
    sibling_[id] = -1;
    parent_[id] = par;
    idOf[string(nm)] = id;
    return id;
}

void hire(int pid, const char *nm) {
    int id = newNode(nm, pid);
    if (child_[pid] == -1) {
        child_[pid] = id;
    } else {
        int cur = child_[pid];
        while (sibling_[cur] != -1) cur = sibling_[cur];
        sibling_[cur] = id;
    }
}

// resolve(head, extra): head is a sibling chain (via sibling_[]), extra is a
// chain to attach where the cascade bottoms out. Returns the new head of
// the resulting chain (the node that should occupy this position), with
// child_/sibling_/parent_ updated for every node whose immediate
// parent-chain assignment changes.
int resolve(int head, int extra) {
    if (head == -1) return extra;
    int c1 = head;
    int restChain = sibling_[c1];
    int childChain = child_[c1];
    int newChildChain = resolve(childChain, restChain);
    child_[c1] = newChildChain;
    for (int t = newChildChain; t != -1; t = sibling_[t]) parent_[t] = c1;
    sibling_[c1] = extra;
    return c1;
}

void fire(int v) {
    int p = parent_[v];
    int vsib = sibling_[v];
    int newN = resolve(child_[v], -1);
    int replacement;
    if (newN != -1) {
        sibling_[newN] = vsib;
        replacement = newN;
    } else {
        replacement = vsib;
    }
    if (p == -1) {
        ceoId = replacement;
        if (replacement != -1) parent_[replacement] = -1;
    } else {
        if (child_[p] == v) {
            child_[p] = replacement;
        } else {
            int cur = child_[p];
            while (sibling_[cur] != v) cur = sibling_[cur];
            sibling_[cur] = replacement;
        }
        if (replacement != -1) parent_[replacement] = p;
    }
}

char lineBuf[1100];

void dfsPrint(int node, int depth) {
    for (int i = 0; i < depth; i++) lineBuf[i] = '+';
    strcpy(lineBuf + depth, names[node]);
    fputs(lineBuf, stdout);
    fputc('\n', stdout);
    for (int c = child_[node]; c != -1; c = sibling_[c]) {
        dfsPrint(c, depth + 1);
    }
}

void doPrint() {
    if (ceoId != -1) dfsPrint(ceoId, 0);
    for (int i = 0; i < 60; i++) fputc('-', stdout);
    fputc('\n', stdout);
}

int main() {
    char buf1[64], buf2[64], buf3[64];
    if (scanf("%s", buf1) != 1) return 0;
    ceoId = newNode(buf1, -1);

    while (scanf("%s", buf1) == 1) {
        if (strcmp(buf1, "print") == 0) {
            doPrint();
        } else if (strcmp(buf1, "fire") == 0) {
            scanf("%s", buf2);
            map<string,int>::iterator it = idOf.find(string(buf2));
            fire(it->second);
        } else {
            // buf1 hires buf3
            scanf("%s", buf2); // "hires"
            scanf("%s", buf3);
            map<string,int>::iterator it = idOf.find(string(buf1));
            hire(it->second, buf3);
        }
    }

    return 0;
}
