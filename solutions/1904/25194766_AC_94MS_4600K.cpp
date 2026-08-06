// POJ 1904 - King's Quest
// Model: gpt-5.6-terra
// Map every liked girl to the prince she is matched with in the given perfect matching.
// A liked edge can belong to another perfect matching exactly when its endpoints are
// in the same strongly connected component of this directed alternating graph.
#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

const int MAXN = 2005;

int n;
vector<int> likes[MAXN];
vector<int> graph[MAXN];
vector<int> reverseGraph[MAXN];
int order[MAXN], orderSize;
int component[MAXN], componentCount;
bool used[MAXN];

void dfsOrder(int vertex) {
    int i;
    used[vertex] = true;
    for (i = 0; i < (int)graph[vertex].size(); ++i) {
        int to = graph[vertex][i];
        if (!used[to]) dfsOrder(to);
    }
    order[orderSize++] = vertex;
}

void dfsComponent(int vertex) {
    int i;
    component[vertex] = componentCount;
    for (i = 0; i < (int)reverseGraph[vertex].size(); ++i) {
        int to = reverseGraph[vertex][i];
        if (component[to] == -1) dfsComponent(to);
    }
}

int main() {
    int i;
    int matchGirl[MAXN];
    int owner[MAXN];

    if (scanf("%d", &n) != 1) return 0;
    for (i = 0; i < n; ++i) {
        int count, j;
        scanf("%d", &count);
        likes[i].resize(count);
        for (j = 0; j < count; ++j) {
            scanf("%d", &likes[i][j]);
            --likes[i][j];
        }
    }
    for (i = 0; i < n; ++i) {
        scanf("%d", &matchGirl[i]);
        --matchGirl[i];
        owner[matchGirl[i]] = i;
    }
    for (i = 0; i < n; ++i) {
        int j;
        for (j = 0; j < (int)likes[i].size(); ++j) {
            int to = owner[likes[i][j]];
            graph[i].push_back(to);
            reverseGraph[to].push_back(i);
        }
    }

    for (i = 0; i < n; ++i) if (!used[i]) dfsOrder(i);
    for (i = 0; i < n; ++i) component[i] = -1;
    for (i = n - 1; i >= 0; --i) {
        int vertex = order[i];
        if (component[vertex] == -1) {
            dfsComponent(vertex);
            ++componentCount;
        }
    }

    for (i = 0; i < n; ++i) {
        vector<int> answer;
        int j;
        for (j = 0; j < (int)likes[i].size(); ++j) {
            int girl = likes[i][j];
            if (component[i] == component[owner[girl]]) answer.push_back(girl + 1);
        }
        sort(answer.begin(), answer.end());
        printf("%d", (int)answer.size());
        for (j = 0; j < (int)answer.size(); ++j) printf(" %d", answer[j]);
        printf("\n");
    }
    return 0;
}
