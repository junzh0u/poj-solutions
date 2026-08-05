// POJ 3694 - Network
// Model: gpt-5.6-terra
// Approach: Find the initial bridges with Tarjan's low-link DFS, contract every
// non-bridge-connected component, and root the resulting bridge tree.  A new
// edge makes every still-present bridge on its component-tree path non-bridges.
// Binary lifting finds the static LCA; a disjoint-set "next" pointer skips
// component-tree edges already removed, so each initial bridge is removed once.
// Parallel initial links are distinguished by edge id during low-link DFS.

#include <cstdio>
#include <cstring>

const int MAXN = 100005;
const int MAXE = 400005;
const int LOG = 17;

int n, m;
int head[MAXN], to[MAXE], next_edge[MAXE], edge_count;
int tin[MAXN], low[MAXN], timer;
bool bridge[MAXE];
int dfs_stack[MAXN], dfs_iter[MAXN], dfs_parent_edge[MAXN];
int component[MAXN], component_count;
int component_stack[MAXN];
int tree_head[MAXN], tree_to[MAXE], tree_next[MAXE], tree_edge_count;
int parent_node[MAXN], depth_node[MAXN], ancestor[LOG][MAXN];
int jump_parent[MAXN];
int tree_order[MAXN];

void add_edge(int u, int v) {
    to[edge_count] = v;
    next_edge[edge_count] = head[u];
    head[u] = edge_count++;
}

void tarjan() {
    int top = 0;
    dfs_stack[top++] = 1;
    dfs_parent_edge[1] = -1;
    dfs_iter[1] = head[1];
    tin[1] = low[1] = ++timer;
    while (top) {
        int u = dfs_stack[top - 1];
        int e = dfs_iter[u];
        if (e == -1) {
            --top;
            if (dfs_parent_edge[u] != -1) {
                int parent = to[dfs_parent_edge[u] ^ 1];
                if (low[u] < low[parent]) low[parent] = low[u];
                if (low[u] > tin[parent]) {
                    bridge[dfs_parent_edge[u]] = true;
                    bridge[dfs_parent_edge[u] ^ 1] = true;
                }
            }
            continue;
        }
        dfs_iter[u] = next_edge[e];
        if (e == (dfs_parent_edge[u] ^ 1)) continue;
        int v = to[e];
        if (!tin[v]) {
            dfs_parent_edge[v] = e;
            dfs_iter[v] = head[v];
            tin[v] = low[v] = ++timer;
            dfs_stack[top++] = v;
        } else if (tin[v] < low[u]) {
            low[u] = tin[v];
        }
    }
}

void add_tree_edge(int u, int v) {
    tree_to[tree_edge_count] = v;
    tree_next[tree_edge_count] = tree_head[u];
    tree_head[u] = tree_edge_count++;
}

int find_jump(int x) {
    if (jump_parent[x] == x) return x;
    jump_parent[x] = find_jump(jump_parent[x]);
    return jump_parent[x];
}

int lca(int u, int v) {
    if (depth_node[u] < depth_node[v]) {
        int t = u; u = v; v = t;
    }
    int diff = depth_node[u] - depth_node[v];
    int k;
    for (k = 0; k < LOG; ++k) {
        if (diff & (1 << k)) u = ancestor[k][u];
    }
    if (u == v) return u;
    for (k = LOG - 1; k >= 0; --k) {
        if (ancestor[k][u] != ancestor[k][v]) {
            u = ancestor[k][u];
            v = ancestor[k][v];
        }
    }
    return parent_node[u];
}

void erase_to_ancestor(int u, int a, int &answer) {
    u = find_jump(u);
    while (depth_node[u] > depth_node[a]) {
        --answer;
        jump_parent[u] = find_jump(parent_node[u]);
        u = find_jump(u);
    }
}

int main() {
    int case_number = 0;
    while (std::scanf("%d%d", &n, &m) == 2 && (n || m)) {
        std::memset(head, -1, sizeof(head));
        std::memset(tin, 0, sizeof(tin));
        std::memset(bridge, 0, sizeof(bridge));
        edge_count = timer = 0;
        int i, u, v;
        for (i = 0; i < m; ++i) {
            std::scanf("%d%d", &u, &v);
            add_edge(u, v);
            add_edge(v, u);
        }
        tarjan();

        std::memset(component, 0, sizeof(component));
        component_count = 0;
        for (i = 1; i <= n; ++i) {
            if (component[i]) continue;
            ++component_count;
            int top = 0;
            component_stack[top++] = i;
            component[i] = component_count;
            while (top) {
                u = component_stack[--top];
                for (int e = head[u]; e != -1; e = next_edge[e]) {
                    v = to[e];
                    if (!bridge[e] && !component[v]) {
                        component[v] = component_count;
                        component_stack[top++] = v;
                    }
                }
            }
        }

        std::memset(tree_head, -1, sizeof(tree_head));
        tree_edge_count = 0;
        int answer = 0;
        for (i = 0; i < edge_count; i += 2) {
            if (bridge[i]) {
                ++answer;
                u = component[to[i]];
                v = component[to[i ^ 1]];
                add_tree_edge(u, v);
                add_tree_edge(v, u);
            }
        }

        std::memset(parent_node, 0, sizeof(parent_node));
        int count = 0;
        tree_order[count++] = 1;
        parent_node[1] = -1;
        depth_node[1] = 0;
        for (i = 0; i < count; ++i) {
            u = tree_order[i];
            for (int e = tree_head[u]; e != -1; e = tree_next[e]) {
                v = tree_to[e];
                if (v == parent_node[u]) continue;
                parent_node[v] = u;
                depth_node[v] = depth_node[u] + 1;
                ancestor[0][v] = u;
                tree_order[count++] = v;
            }
        }
        parent_node[1] = 0;
        ancestor[0][1] = 0;
        for (int k = 1; k < LOG; ++k) {
            for (i = 1; i <= component_count; ++i) {
                ancestor[k][i] = ancestor[k - 1][ancestor[k - 1][i]];
            }
        }
        for (i = 1; i <= component_count; ++i) jump_parent[i] = i;

        int q;
        std::scanf("%d", &q);
        std::printf("Case %d:\n", ++case_number);
        while (q--) {
            std::scanf("%d%d", &u, &v);
            u = component[u];
            v = component[v];
            int a = lca(u, v);
            erase_to_ancestor(u, a, answer);
            erase_to_ancestor(v, a, answer);
            std::printf("%d\n", answer);
        }
        std::printf("\n");
    }
    return 0;
}
