// POJ 1419 - Graph Coloring
// Model: gpt-5.6-terra
// Search black choices before white choices in node order, retaining the first optimum.
// A greedy coloring of the complement bounds the independent set still available.
// A separate greedy set seeds the bound; the first optimum is still searched and retained.

#include <cstdio>
#include <cstring>
#include <vector>

using namespace std;

bool edge[101][101];
bool black[101];
bool answer[101];
int n;
int bestCount;
int currentCount;
bool answerFound;

int coloringBound(const vector<int> &candidates) {
    vector<int> remaining = candidates;
    int colors = 0;
    while (!remaining.empty()) {
        vector<int> next;
        vector<int> colorClass;
        size_t index;
        ++colors;
        for (index = 0; index < remaining.size(); ++index) {
            int vertex = remaining[index];
            bool fits = true;
            size_t previous;
            for (previous = 0; previous < colorClass.size(); ++previous) {
                if (!edge[vertex][colorClass[previous]]) {
                    fits = false;
                    break;
                }
            }
            if (fits) {
                colorClass.push_back(vertex);
            } else {
                next.push_back(vertex);
            }
        }
        remaining.swap(next);
    }
    return colors;
}

int greedyLowerBound(vector<int> candidates) {
    int selected = 0;
    while (!candidates.empty()) {
        size_t bestIndex = 0;
        size_t index;
        int bestDegree = n + 1;
        vector<int> next;
        int vertex;
        for (index = 0; index < candidates.size(); ++index) {
            size_t other;
            int degree = 0;
            for (other = 0; other < candidates.size(); ++other) {
                if (edge[candidates[index]][candidates[other]]) {
                    ++degree;
                }
            }
            if (degree < bestDegree) {
                bestDegree = degree;
                bestIndex = index;
            }
        }
        vertex = candidates[bestIndex];
        ++selected;
        for (index = 0; index < candidates.size(); ++index) {
            if (index != bestIndex && !edge[vertex][candidates[index]]) {
                next.push_back(candidates[index]);
            }
        }
        candidates.swap(next);
    }
    return selected;
}

void search(const vector<int> &candidates) {
    vector<int> withoutVertex;
    vector<int> withVertex;
    size_t index;
    int vertex;
    int bound = coloringBound(candidates);
    if (currentCount + bound < bestCount ||
        (answerFound && currentCount + bound <= bestCount)) {
        return;
    }
    if (candidates.empty()) {
        if (currentCount > bestCount || !answerFound) {
            bestCount = currentCount;
            answerFound = true;
            for (vertex = 1; vertex <= n; ++vertex) {
                answer[vertex] = black[vertex];
            }
        }
        return;
    }
    vertex = candidates[0];
    for (index = 1; index < candidates.size(); ++index) {
        int other = candidates[index];
        withoutVertex.push_back(other);
        if (!edge[vertex][other]) {
            withVertex.push_back(other);
        }
    }
    black[vertex] = true;
    ++currentCount;
    search(withVertex);
    --currentCount;
    black[vertex] = false;
    search(withoutVertex);
}

int main() {
    int tests;
    if (scanf("%d", &tests) != 1) {
        return 0;
    }
    while (tests-- > 0) {
        vector<int> candidates;
        int edges;
        int index;
        int a;
        int b;
        bool first;
        scanf("%d%d", &n, &edges);
        memset(edge, 0, sizeof(edge));
        memset(black, 0, sizeof(black));
        memset(answer, 0, sizeof(answer));
        while (edges-- > 0) {
            scanf("%d%d", &a, &b);
            edge[a][b] = true;
            edge[b][a] = true;
        }
        for (index = 1; index <= n; ++index) {
            candidates.push_back(index);
        }
        bestCount = greedyLowerBound(candidates);
        currentCount = 0;
        answerFound = false;
        search(candidates);
        printf("%d\n", bestCount);
        first = true;
        for (index = 1; index <= n; ++index) {
            if (answer[index]) {
                if (!first) {
                    printf(" ");
                }
                printf("%d", index);
                first = false;
            }
        }
        printf("\n");
    }
    return 0;
}
