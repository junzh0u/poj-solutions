// POJ 3677 - Chemical Weighing
// Model: gpt-6.1-sol
// Legal stacks form a tree: the parent removes the top (smallest) poise.
// For each request, two tree passes compute the minimum previous cost plus
// tree distance, then retain only stacks having the requested weight.
// The initial stack is empty; no final clearing is required.
#include <cstdio>
#include <algorithm>
using namespace std;
const int INF = 1000000;
const int MAXS = 30000;
int weights[15], n, limit, countStates;
int parentNode[MAXS], total[MAXS], cost[MAXS];
void generate(int p, int sum, int top) {
    for (int i = 0; i < n; ++i) {
        if (weights[i] > top || sum + weights[i] > limit) break;
        int v = countStates++;
        parentNode[v] = p;
        total[v] = sum + weights[i];
        generate(v, total[v], weights[i]);
    }
}
int main() {
    int tests;
    if (scanf("%d", &tests) != 1) return 0;
    while (tests--) {
        scanf("%d", &n);
        for (int i = 0; i < n; ++i) scanf("%d", &weights[i]);
        sort(weights, weights + n);
        n = int(unique(weights, weights + n) - weights);
        int m, requests[500];
        scanf("%d", &m);
        limit = 0;
        for (int i = 0; i < m; ++i) {
            scanf("%d", &requests[i]);
            limit = max(limit, requests[i]);
        }
        countStates = 1;
        total[0] = 0;
        generate(0, 0, limit);
        fill(cost, cost + countStates, INF);
        cost[0] = 0;
        int answer = INF;
        for (int k = 0; k < m; ++k) {
            for (int v = countStates - 1; v > 0; --v)
                cost[parentNode[v]] = min(cost[parentNode[v]], cost[v] + 1);
            for (int v = 1; v < countStates; ++v)
                cost[v] = min(cost[v], cost[parentNode[v]] + 1);
            answer = INF;
            for (int v = 0; v < countStates; ++v) {
                if (total[v] != requests[k]) cost[v] = INF;
                else answer = min(answer, cost[v]);
            }
        }
        printf("%d\n", answer == INF ? -1 : answer);
    }
    return 0;
}
