// POJ 1311 - Doing Windows
// Model: gpt-6.1-sol
// Every tiling by at most four rectangles has a full horizontal or vertical
// cut. Enumerate its binary slicing trees, preserving window orientation.
// Ratios only propose integral cuts; exact leaf cross-products certify them.
// Resized dimensions must be positive integral pixel counts.
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;
struct Node {
    long double ratio;
    int a, b, leaf;
    bool vertical;
    Node(long double r, int x, int y, int z, bool v)
        : ratio(r), a(x), b(y), leaf(z), vertical(v) {}
};
vector<Node> nodes;
vector<int> tree[16];
long long wi[4], hi[4];
bool check(int id, long long w, long long h) {
    const Node &n = nodes[id];
    if (n.leaf >= 0) return w * hi[n.leaf] == h * wi[n.leaf];
    long double value = n.vertical ? h * nodes[n.a].ratio
                                   : w / nodes[n.a].ratio;
    long long limit = n.vertical ? w : h;
    if (value < 0.5L || value > limit - 0.5L) return false;
    long long cut = (long long)floor(value + 0.5L);
    if (cut <= 0 || cut >= limit) return false;
    if (n.vertical)
        return check(n.a, cut, h) && check(n.b, w-cut, h);
    return check(n.a, w, cut) && check(n.b, w, h-cut);
}
int main() {
    long long w, h;
    int cas = 0;
    while (cin >> w >> h && (w || h)) {
        nodes.clear();
        for (int m=0; m<16; ++m) tree[m].clear();
        for (int i=0; i<4; ++i) {
            cin >> wi[i] >> hi[i];
            tree[1<<i].push_back((int)nodes.size());
            nodes.push_back(Node((long double)wi[i]/hi[i], -1,-1,i,false));
        }
        for (int m=1; m<16; ++m) {
            if (!(m & (m-1))) continue;
            for (int a=(m-1)&m; a; a=(a-1)&m) {
                int b=m^a;
                if (!b || a>b) continue;
                for (size_t i=0; i<tree[a].size(); ++i)
                    for (size_t j=0; j<tree[b].size(); ++j) {
                        int x=tree[a][i], y=tree[b][j];
                        long double r=nodes[x].ratio, s=nodes[y].ratio;
                        tree[m].push_back((int)nodes.size());
                        nodes.push_back(Node(r+s,x,y,-1,true));
                        tree[m].push_back((int)nodes.size());
                        nodes.push_back(Node(r*s/(r+s),x,y,-1,false));
                    }
            }
        }
        bool ok=false;
        for (size_t i=0; i<tree[15].size() && !ok; ++i)
            ok=check(tree[15][i],w,h);
        cout << "Set " << ++cas << ": " << (ok ? "Yes" : "No") << '\n';
    }
}
