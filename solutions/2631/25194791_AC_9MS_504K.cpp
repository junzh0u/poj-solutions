// POJ 2631 - Roads in the North
// Model: gpt-5.6-terra
// The road network is a tree.  Two iterative traversals find a diameter:
// from any village reach an endpoint, then find its farthest village.
// Empty input is a valid case and has diameter zero.
#include <cstdio>
#include <vector>
#include <utility>

using namespace std;

struct State {
    int vertex;
    int parent;
    long long distance;
    State(int v, int p, long long d) : vertex(v), parent(p), distance(d) {}
};

static pair<int, long long> farthest(int start,
                                     const vector< vector< pair<int, int> > >& graph) {
    vector<State> stack;
    pair<int, long long> result(start, 0);
    stack.push_back(State(start, -1, 0));
    while (!stack.empty()) {
        State current = stack.back();
        stack.pop_back();
        if (current.distance > result.second) {
            result = make_pair(current.vertex, current.distance);
        }
        const vector< pair<int, int> >& edges = graph[current.vertex];
        for (size_t i = 0; i < edges.size(); ++i) {
            if (edges[i].first != current.parent) {
                stack.push_back(State(edges[i].first, current.vertex,
                                      current.distance + edges[i].second));
            }
        }
    }
    return result;
}

int main() {
    vector< vector< pair<int, int> > > graph(1);
    int a, b, length;
    int start = -1;
    while (scanf("%d%d%d", &a, &b, &length) == 3) {
        int needed = a > b ? a : b;
        if (needed >= (int)graph.size()) {
            graph.resize(needed + 1);
        }
        graph[a].push_back(make_pair(b, length));
        graph[b].push_back(make_pair(a, length));
        if (start == -1) {
            start = a;
        }
    }
    if (start == -1) {
        printf("0\n");
        return 0;
    }
    pair<int, long long> endpoint = farthest(start, graph);
    printf("%lld\n", farthest(endpoint.first, graph).second);
    return 0;
}
