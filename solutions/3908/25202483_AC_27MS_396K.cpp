// POJ 3908 - Quick answer
// Model: claude-sonnet-5
//
// Union-find with "soft delete": each real town i maps to a current
// virtual id (real[i]), initially i itself. A "c a b" command unions
// real[a] and real[b] in the DSU. A "q a b" checks whether real[a] and
// real[b] are in the same DSU set. A "d a" command does NOT split the
// DSU set that a currently belongs to (the other members of that set
// keep their history and connectivity intact, per the statement); it
// only gives town a a brand new, never-before-used virtual id, so from
// now on a starts fresh as an isolated singleton while everyone else's
// connectivity is unaffected. This matches the sample: after
// c1-2, c3-4, c2-3 all four are one component; "d 2" isolates town 2
// but towns 1,3,4 remain mutually connected (q 4 1 -> yes), while town
// 2 is now alone (q 2 4 -> no), giving the sample's 2 yes / 2 no.
//
// Multiple data sets in one file, terminated by EOF (confirmed on the
// problem's discuss board), each data set itself terminated by a
// command "e".
#include <cstdio>
#include <vector>
using namespace std;

vector<int> parent_, sz_;

int find(int x) {
    while (parent_[x] != x) {
        parent_[x] = parent_[parent_[x]];
        x = parent_[x];
    }
    return x;
}

void unite(int a, int b) {
    a = find(a);
    b = find(b);
    if (a == b) return;
    if (sz_[a] < sz_[b]) { int t = a; a = b; b = t; }
    parent_[b] = a;
    sz_[a] += sz_[b];
}

int main() {
    int n;
    while (scanf("%d", &n) == 1) {
        vector<int> real(n + 1);
        // reserve some extra capacity for virtual ids created by 'd'
        parent_.assign(n + 1, 0);
        sz_.assign(n + 1, 1);
        for (int i = 1; i <= n; ++i) { real[i] = i; parent_[i] = i; }
        int nextId = n; // next virtual id to hand out is nextId+1

        long yes = 0, no = 0;
        char cmd[8];
        for (;;) {
            if (scanf("%s", cmd) != 1) break; // shouldn't happen if input well-formed
            if (cmd[0] == 'e') {
                break;
            } else if (cmd[0] == 'c') {
                int a, b;
                scanf("%d %d", &a, &b);
                unite(real[a], real[b]);
            } else if (cmd[0] == 'd') {
                int a;
                scanf("%d", &a);
                ++nextId;
                parent_.push_back(nextId);
                sz_.push_back(1);
                real[a] = nextId;
            } else if (cmd[0] == 'q') {
                int a, b;
                scanf("%d %d", &a, &b);
                if (find(real[a]) == find(real[b])) ++yes;
                else ++no;
            }
        }
        printf("%ld , %ld\n", yes, no);
    }
    return 0;
}
