// POJ 2732 - Countdown
// Model: claude-sonnet-5
// Approach: build the family tree from the (unordered) parent-child lines,
// find the root (the one node with no parent), then do a preorder DFS to
// assign each node a preorder index (tin) and subtree size, and bucket the
// preorder indices of every node by its depth from the root. For a query
// depth d, the descendant count of node u at exactly distance d is the
// number of preorder indices in bucket[depth[u]+d] that fall inside u's
// subtree range [tin[u], tin[u]+size[u]-1]; this is answered by binary
// search since bucket entries are inserted in increasing tin order. Then
// rank all nodes with count>0 by (count desc, name asc) and print the top
// three, extending past three whenever the third-place count ties with
// further entries (a tie at the cutoff pulls in everyone sharing that
// count, per the statement and confirmed by sample 2 where all five
// children of Phillip tie at count 1 and all print).
// Ambiguity: none found in the statement itself; the "print more than
// three on a tie near the bottom" rule was verified against sample 2.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

int getId(map<string,int> &id, vector<string> &names, vector<vector<int> > &children, vector<int> &parent, const string &s) {
    map<string,int>::iterator it = id.find(s);
    if (it != id.end()) return it->second;
    int nid = (int)names.size();
    id[s] = nid;
    names.push_back(s);
    children.push_back(vector<int>());
    parent.push_back(-1);
    return nid;
}

struct Node {
    string name;
    int count;
};

bool cmpNode(const Node &a, const Node &b) {
    if (a.count != b.count) return a.count > b.count;
    return a.name < b.name;
}

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    for (int tc = 1; tc <= T; tc++) {
        int n, d;
        scanf("%d %d", &n, &d);

        map<string,int> id;
        vector<string> names;
        vector<vector<int> > children;
        vector<int> parent;

        char buf[32];
        for (int i = 0; i < n; i++) {
            scanf("%s", buf);
            int pid = getId(id, names, children, parent, string(buf));
            int m;
            scanf("%d", &m);
            for (int j = 0; j < m; j++) {
                scanf("%s", buf);
                int cid = getId(id, names, children, parent, string(buf));
                children[pid].push_back(cid);
                parent[cid] = pid;
            }
        }

        int cnt = (int)names.size();
        int root = -1;
        for (int i = 0; i < cnt; i++) {
            if (parent[i] == -1) { root = i; break; }
        }

        vector<int> tin(cnt, -1), depth(cnt, 0), sz(cnt, 1);
        vector<int> order; order.reserve(cnt);

        // iterative preorder DFS using explicit stack, tracking child index
        vector<int> stackNode; stackNode.reserve(cnt);
        vector<size_t> stackIdx; stackIdx.reserve(cnt);
        int timer = 0;
        tin[root] = timer++;
        depth[root] = 0;
        order.push_back(root);
        stackNode.push_back(root);
        stackIdx.push_back(0);

        while (!stackNode.empty()) {
            int u = stackNode.back();
            size_t &idx = stackIdx.back();
            if (idx < children[u].size()) {
                int v = children[u][idx];
                idx++;
                tin[v] = timer++;
                depth[v] = depth[u] + 1;
                order.push_back(v);
                stackNode.push_back(v);
                stackIdx.push_back(0);
            } else {
                stackNode.pop_back();
                stackIdx.pop_back();
            }
        }

        // compute subtree sizes by processing order in reverse
        for (int i = (int)order.size() - 1; i >= 0; i--) {
            int u = order[i];
            for (size_t j = 0; j < children[u].size(); j++) {
                sz[u] += sz[children[u][j]];
            }
        }

        // bucket tin values by depth, inserted in increasing tin order automatically
        int maxDepth = 0;
        for (int i = 0; i < cnt; i++) if (depth[i] > maxDepth) maxDepth = depth[i];
        vector<vector<int> > bucket(maxDepth + 1);
        for (int i = 0; i < (int)order.size(); i++) {
            int u = order[i];
            bucket[depth[u]].push_back(tin[u]);
        }

        vector<Node> result;
        for (int u = 0; u < cnt; u++) {
            int target = depth[u] + d;
            int c = 0;
            if (target >= 0 && target <= maxDepth) {
                vector<int> &arr = bucket[target];
                int lo = tin[u], hi = tin[u] + sz[u] - 1;
                int left = (int)(lower_bound(arr.begin(), arr.end(), lo) - arr.begin());
                int right = (int)(upper_bound(arr.begin(), arr.end(), hi) - arr.begin());
                c = right - left;
            }
            if (c > 0) {
                Node nd; nd.name = names[u]; nd.count = c;
                result.push_back(nd);
            }
        }

        sort(result.begin(), result.end(), cmpNode);

        int outCount;
        if ((int)result.size() <= 3) {
            outCount = (int)result.size();
        } else {
            int cutoff = result[2].count;
            outCount = (int)result.size();
            for (int i = 0; i < (int)result.size(); i++) {
                if (result[i].count < cutoff) { outCount = i; break; }
            }
        }

        printf("Tree %d:\n", tc);
        for (int i = 0; i < outCount; i++) {
            printf("%s %d\n", result[i].name.c_str(), result[i].count);
        }
        if (tc != T) printf("\n");
    }
    return 0;
}
